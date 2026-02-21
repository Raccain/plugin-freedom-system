//==============================================================================
// GrooveScoutAnalyzer.cpp
//
// Phase DSP.2: BPM detection via Onset Strength Signal (OSS) +
// Generalized Autocorrelation (GAC) using juce::dsp::FFT.
//
// Future phases:
//   DSP.3: Key detection (STFT chromagram + Krumhansl-Schmuckler)
//   DSP.4: Drum onset detection + MIDI assembly
//==============================================================================

#include "GrooveScoutAnalyzer.h"
#include "PluginProcessor.h"

#include <cmath>
#include <vector>
#include <algorithm>

GrooveScoutAnalyzer::GrooveScoutAnalyzer (GrooveScoutAudioProcessor& p)
    : juce::Thread ("GrooveScoutAnalyzer"), proc (p)
{
}

GrooveScoutAnalyzer::~GrooveScoutAnalyzer()
{
    stopThread (2000);
}

void GrooveScoutAnalyzer::run()
{
    DBG ("GrooveScoutAnalyzer: analysis started (DSP.2)");

    // -------------------------------------------------------------------------
    // 1. Validate minimum buffer length (2 seconds required)
    // -------------------------------------------------------------------------
    const int numRecorded = proc.recordedSamples.load();
    const int minSamples  = static_cast<int> (proc.currentSampleRate * 2.0);

    if (numRecorded < minSamples)
    {
        proc.analysisError.store (1);  // 1 = buffer too short
        proc.analysisComplete.store (true);
        DBG ("GrooveScoutAnalyzer: buffer too short — need >= 2 seconds, got "
             + juce::String (numRecorded) + " samples");
        return;
    }

    // -------------------------------------------------------------------------
    // 2. BPM Detection (step 1)
    // -------------------------------------------------------------------------
    proc.analysisStep.store (1);      // UI label: "Detecting BPM..."
    proc.analysisProgress.store (10);

    auto* analyzeBpmParam = proc.parameters.getRawParameterValue ("analyzeBPM");
    const bool doBPM = (analyzeBpmParam == nullptr) || (analyzeBpmParam->load() > 0.5f);

    if (doBPM)
    {
        // ---------------------------------------------------------------------
        // 2a. Mix stereo recording buffer to mono
        // ---------------------------------------------------------------------
        std::vector<float> mono (static_cast<size_t> (numRecorded));

        const float* left  = proc.recordingBuffer.getReadPointer (0);
        const float* right = proc.recordingBuffer.getReadPointer (1);

        for (int i = 0; i < numRecorded; ++i)
            mono[static_cast<size_t> (i)] = (left[i] + right[i]) * 0.5f;

        if (threadShouldExit())
        {
            proc.analysisCancelled.store (true);
            return;
        }

        // ---------------------------------------------------------------------
        // 2b. Compute Onset Strength Signal (OSS)
        //     Frame: 2048 samples, hop: 512 samples
        //     OSS[n] = max(0, RMS[n] - RMS[n-1])  (half-wave rectified delta)
        // ---------------------------------------------------------------------
        const int frameSize = 2048;
        const int hopSize   = 512;

        std::vector<float> oss;
        oss.reserve (static_cast<size_t> (numRecorded / hopSize + 1));

        float prevEnergy = 0.0f;

        for (int i = 0; i + frameSize <= numRecorded; i += hopSize)
        {
            // RMS energy of this frame
            float energy = 0.0f;
            for (int j = 0; j < frameSize; ++j)
            {
                const float s = mono[static_cast<size_t> (i + j)];
                energy += s * s;
            }
            energy = std::sqrt (energy / static_cast<float> (frameSize));

            // Half-wave rectified energy delta
            const float delta = energy - prevEnergy;
            oss.push_back (std::max (0.0f, delta));
            prevEnergy = energy;
        }

        if (threadShouldExit())
        {
            proc.analysisCancelled.store (true);
            return;
        }

        proc.analysisProgress.store (15);

        // ---------------------------------------------------------------------
        // 2c. Generalized Autocorrelation via FFT
        //     a. Raise OSS to power 0.5 (sqrt compression)
        //     b. Forward FFT
        //     c. Compute power spectrum |FFT|^2 (in-place)
        //     d. Inverse FFT → autocorrelation
        // ---------------------------------------------------------------------
        const int ossSize = static_cast<int> (oss.size());

        // Find next power-of-2 >= ossSize
        int fftOrder = 0;
        int fftSize  = 1;
        while (fftSize < ossSize)
        {
            fftSize <<= 1;
            ++fftOrder;
        }

        juce::dsp::FFT fft (fftOrder);

        // Build input buffer: interleaved real/imag pairs (juce::dsp::FFT convention)
        // Size must be 2 * fftSize to hold complex pairs
        std::vector<float> ossForward (static_cast<size_t> (fftSize * 2), 0.0f);

        // Fill real input samples at positions 0..ossSize-1.
        // performRealOnlyForwardTransform expects real input packed at the start
        // of the 2*fftSize buffer; the rest is used as scratch space.
        for (int i = 0; i < ossSize; ++i)
            ossForward[static_cast<size_t> (i)] = std::sqrt (oss[static_cast<size_t> (i)]);

        if (threadShouldExit())
        {
            proc.analysisCancelled.store (true);
            return;
        }

        // Forward FFT — result is interleaved complex [re0, im0, re1, im1, ...]
        fft.performRealOnlyForwardTransform (ossForward.data(), true);

        proc.analysisProgress.store (18);

        // Power spectrum |FFT|^2 in-place
        // After performRealOnlyForwardTransform, layout is interleaved complex pairs
        for (int i = 0; i < fftSize * 2; i += 2)
        {
            const float re   = ossForward[static_cast<size_t> (i)];
            const float im   = ossForward[static_cast<size_t> (i + 1)];
            const float mag2 = re * re + im * im;
            ossForward[static_cast<size_t> (i)]     = mag2;
            ossForward[static_cast<size_t> (i + 1)] = 0.0f;  // zero imaginary → real power
        }

        // Inverse FFT → generalized autocorrelation (GAC)
        fft.performRealOnlyInverseTransform (ossForward.data());
        // ossForward[k] now holds autocorrelation at lag k frames

        if (threadShouldExit())
        {
            proc.analysisCancelled.store (true);
            return;
        }

        proc.analysisProgress.store (22);

        // ---------------------------------------------------------------------
        // 2d. Peak-pick: convert lag index → BPM, restrict to 60–200 BPM
        // ---------------------------------------------------------------------
        float bestBpm   = 0.0f;
        float bestScore = -1.0f;

        const float minBpm = 60.0f;
        const float maxBpm = 200.0f;
        const float sr     = static_cast<float> (proc.currentSampleRate);

        // Lag 0 is the trivial self-correlation peak — skip it.
        // Search up to fftSize/2 lags (Nyquist of autocorrelation)
        for (int lagFrames = 1; lagFrames < fftSize / 2; ++lagFrames)
        {
            // lagFrames OSS frames = lagFrames * hopSize audio samples
            const float lagSamples = static_cast<float> (lagFrames) * static_cast<float> (hopSize);
            const float bpm        = 60.0f * sr / lagSamples;

            if (bpm < minBpm || bpm > maxBpm)
                continue;

            // Real part of complex pair at lag k is at index k*2 in interleaved layout
            const float score = ossForward[static_cast<size_t> (lagFrames * 2)];
            if (score > bestScore)
            {
                bestScore = score;
                bestBpm   = bpm;
            }
        }

        // ---------------------------------------------------------------------
        // 2e. Apply bpmMultiplier (0=½×, 1=1×, 2=2×)
        // ---------------------------------------------------------------------
        auto* multiplierParam = proc.parameters.getRawParameterValue ("bpmMultiplier");
        if (multiplierParam != nullptr)
        {
            const int   multiplierIndex = static_cast<int> (multiplierParam->load());
            const float multipliers[]   = { 0.5f, 1.0f, 2.0f };

            if (multiplierIndex >= 0 && multiplierIndex <= 2)
                bestBpm *= multipliers[multiplierIndex];
        }

        // Store result — read by message thread only after analysisComplete==true
        proc.detectedBpm = bestBpm;

        DBG ("GrooveScoutAnalyzer: BPM detected = " + juce::String (bestBpm, 1));
    }

    proc.analysisProgress.store (25);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // -------------------------------------------------------------------------
    // 3. Key detection stub (DSP.3 will replace)
    // -------------------------------------------------------------------------
    proc.analysisStep.store (2);      // UI label: "Detecting Key..."
    proc.analysisProgress.store (50);
    // detectedKey remains empty string until DSP.3

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // -------------------------------------------------------------------------
    // 4. Drum onset detection stub (DSP.4 will replace)
    // -------------------------------------------------------------------------
    proc.analysisStep.store (3);      // UI label: "Detecting Drums..."
    proc.analysisProgress.store (75);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // -------------------------------------------------------------------------
    // 5. MIDI assembly stub (DSP.4 will replace)
    // -------------------------------------------------------------------------
    proc.analysisStep.store (4);      // UI label: "Writing MIDI..."
    proc.analysisProgress.store (100);

    // CRITICAL: set analysisComplete LAST — after all result fields are written.
    // Message thread reads results only after seeing analysisComplete == true.
    proc.analysisComplete.store (true);
    DBG ("GrooveScoutAnalyzer: analysis complete (DSP.2)");
}
