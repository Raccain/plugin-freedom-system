//==============================================================================
// GrooveScoutAnalyzer.cpp
//
// Phase DSP.2: BPM detection via Onset Strength Signal (OSS) +
// Generalized Autocorrelation (GAC) using juce::dsp::FFT.
//
// Phase DSP.3: Key detection via STFT chromagram + Krumhansl-Schmuckler
// profile correlation. Includes 100 Hz high-pass pre-filter, 4096-point
// FFT with Hann window, pitch class profile accumulation, Pearson
// correlation against 24 key profiles, and root chord MIDI derivation.
//
// Future phases:
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
    DBG ("GrooveScoutAnalyzer: analysis started (DSP.3)");

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
    // 3. Key Detection (DSP.3) — STFT chromagram + Krumhansl-Schmuckler
    // -------------------------------------------------------------------------
    proc.analysisStep.store (2);      // UI label: "Detecting Key..."
    proc.analysisProgress.store (26);

    auto* analyzeKeyParam = proc.parameters.getRawParameterValue ("analyzeKey");
    const bool doKey = (analyzeKeyParam == nullptr) || (analyzeKeyParam->load() > 0.5f);

    if (doKey)
    {
        // -----------------------------------------------------------------
        // 3a. Prepare mono buffer for key detection
        //     Re-create mono mix (or reuse if BPM step already made one —
        //     but we cannot rely on that since analyzeBPM may be disabled).
        // -----------------------------------------------------------------
        const int keyNumRecorded = proc.recordedSamples.load();
        std::vector<float> keyMono (static_cast<size_t> (keyNumRecorded));

        {
            const float* leftCh  = proc.recordingBuffer.getReadPointer (0);
            const float* rightCh = proc.recordingBuffer.getReadPointer (1);

            for (int i = 0; i < keyNumRecorded; ++i)
                keyMono[static_cast<size_t> (i)] = (leftCh[i] + rightCh[i]) * 0.5f;
        }

        if (threadShouldExit())
        {
            proc.analysisCancelled.store (true);
            return;
        }

        proc.analysisProgress.store (28);

        // -----------------------------------------------------------------
        // 3b. High-pass filter at 100 Hz to reduce kick drum contamination
        //     Simple first-order IIR high-pass applied in-place.
        //     y[n] = alpha * (y[n-1] + x[n] - x[n-1])
        //     where alpha = RC / (RC + dt), RC = 1/(2*pi*fc)
        // -----------------------------------------------------------------
        {
            const double sr = proc.currentSampleRate;
            const double cutoff = 100.0;
            const double rc = 1.0 / (2.0 * juce::MathConstants<double>::pi * cutoff);
            const double dt = 1.0 / sr;
            const float alpha = static_cast<float> (rc / (rc + dt));

            float prevX = keyMono[0];
            float prevY = keyMono[0];

            for (size_t i = 1; i < keyMono.size(); ++i)
            {
                const float x = keyMono[i];
                prevY = alpha * (prevY + x - prevX);
                prevX = x;
                keyMono[i] = prevY;
            }
        }

        if (threadShouldExit())
        {
            proc.analysisCancelled.store (true);
            return;
        }

        proc.analysisProgress.store (30);

        // -----------------------------------------------------------------
        // 3c. STFT Chromagram computation
        //     4096-point FFT (order 12), 2048-sample hop (50% overlap),
        //     Hann window. Map FFT bins to 12 pitch classes.
        // -----------------------------------------------------------------
        const int chromaFftOrder = 12;
        const int chromaFftSize  = 1 << chromaFftOrder;  // 4096
        const int chromaHop      = chromaFftSize / 2;     // 2048

        juce::dsp::FFT chromaFft (chromaFftOrder);
        juce::dsp::WindowingFunction<float> hannWindow (
            static_cast<size_t> (chromaFftSize),
            juce::dsp::WindowingFunction<float>::hann);

        // Working buffer for FFT — needs 2 * fftSize for interleaved complex
        std::vector<float> fftBuffer (static_cast<size_t> (chromaFftSize * 2), 0.0f);

        // Accumulated pitch class profile (12 pitch classes: C, C#, D, ..., B)
        float pcp[12] = {};

        const double keySr = proc.currentSampleRate;
        int frameCount = 0;

        for (int frameStart = 0;
             frameStart + chromaFftSize <= keyNumRecorded;
             frameStart += chromaHop)
        {
            // Copy frame into fft buffer and zero-pad imaginary part
            for (int j = 0; j < chromaFftSize; ++j)
                fftBuffer[static_cast<size_t> (j)] = keyMono[static_cast<size_t> (frameStart + j)];

            std::fill (fftBuffer.begin() + chromaFftSize, fftBuffer.end(), 0.0f);

            // Apply Hann window
            hannWindow.multiplyWithWindowingTable (fftBuffer.data(),
                                                   static_cast<size_t> (chromaFftSize));

            // Forward FFT — result is interleaved complex [re0, im0, re1, im1, ...]
            chromaFft.performRealOnlyForwardTransform (fftBuffer.data(), true);

            // Map FFT bins to pitch classes
            // Only use bins corresponding to frequencies above ~32 Hz (MIDI ~24)
            // and below Nyquist/2 to avoid aliasing artefacts.
            // Bin 0 is DC — skip. Bin k corresponds to freq = k * sr / fftSize.
            const int minBin = std::max (1, static_cast<int> (std::ceil (32.0 * chromaFftSize / keySr)));
            const int maxBin = chromaFftSize / 2;  // Nyquist bin

            for (int k = minBin; k < maxBin; ++k)
            {
                const double freq = static_cast<double> (k) * keySr / static_cast<double> (chromaFftSize);

                // Skip frequencies below 32 Hz (too low for pitch class mapping)
                if (freq < 32.0)
                    continue;

                // Compute magnitude from interleaved complex pair
                const float re  = fftBuffer[static_cast<size_t> (k * 2)];
                const float im  = fftBuffer[static_cast<size_t> (k * 2 + 1)];
                const float mag = std::sqrt (re * re + im * im);

                // Map to MIDI pitch, then to pitch class (0-11)
                const double midiPitch = 12.0 * std::log2 (freq / 440.0) + 69.0;
                int pitchClass = static_cast<int> (std::round (midiPitch)) % 12;
                if (pitchClass < 0)
                    pitchClass += 12;

                pcp[pitchClass] += mag;
            }

            ++frameCount;

            // Periodic cancellation check (every 64 frames to avoid overhead)
            if ((frameCount & 63) == 0 && threadShouldExit())
            {
                proc.analysisCancelled.store (true);
                return;
            }
        }

        if (threadShouldExit())
        {
            proc.analysisCancelled.store (true);
            return;
        }

        proc.analysisProgress.store (40);

        // -----------------------------------------------------------------
        // 3d. Normalize PCP to unit length
        // -----------------------------------------------------------------
        float pcpNorm = 0.0f;
        for (int i = 0; i < 12; ++i)
            pcpNorm += pcp[i] * pcp[i];

        pcpNorm = std::sqrt (pcpNorm);

        if (pcpNorm < 1e-10f)
        {
            // Silent audio or no tonal content — return "Unknown"
            proc.detectedKey = "Unknown";
            proc.rootChordValid = false;
            DBG ("GrooveScoutAnalyzer: key detection — silent/atonal audio, returning Unknown");
        }
        else
        {
            for (int i = 0; i < 12; ++i)
                pcp[i] /= pcpNorm;

            // ---------------------------------------------------------
            // 3e. Krumhansl-Schmuckler key profile correlation
            //     Major profile (Krumhansl 1990):
            //     [6.35, 2.23, 3.48, 2.33, 4.38, 4.09, 2.52, 5.19, 2.39, 3.66, 2.29, 2.88]
            //     Minor profile:
            //     [6.33, 2.68, 3.52, 5.38, 2.60, 3.53, 2.54, 4.75, 3.98, 2.69, 3.34, 3.17]
            //
            //     For each of 24 keys (12 major + 12 minor), rotate the
            //     profile and compute Pearson correlation with PCP.
            // ---------------------------------------------------------
            const float majorProfile[12] = {
                6.35f, 2.23f, 3.48f, 2.33f, 4.38f, 4.09f,
                2.52f, 5.19f, 2.39f, 3.66f, 2.29f, 2.88f
            };

            const float minorProfile[12] = {
                6.33f, 2.68f, 3.52f, 5.38f, 2.60f, 3.53f,
                2.54f, 4.75f, 3.98f, 2.69f, 3.34f, 3.17f
            };

            const char* noteNames[12] = {
                "C", "C#", "D", "D#", "E", "F",
                "F#", "G", "G#", "A", "A#", "B"
            };

            // Lambda: Pearson correlation between PCP and a rotated profile.
            // Rotation by `shift` means the profile starting note is at
            // pitch class `shift`. We rotate PCP indices to align.
            auto pearsonCorrelation = [] (const float* pcpVec,
                                          const float* profile,
                                          int shift) -> float
            {
                // Compute means
                float meanP = 0.0f, meanQ = 0.0f;
                for (int i = 0; i < 12; ++i)
                {
                    meanP += pcpVec[(i + shift) % 12];
                    meanQ += profile[i];
                }
                meanP /= 12.0f;
                meanQ /= 12.0f;

                // Compute correlation
                float num = 0.0f, denP = 0.0f, denQ = 0.0f;
                for (int i = 0; i < 12; ++i)
                {
                    const float p = pcpVec[(i + shift) % 12] - meanP;
                    const float q = profile[i] - meanQ;
                    num  += p * q;
                    denP += p * p;
                    denQ += q * q;
                }

                const float den = std::sqrt (denP * denQ);
                if (den < 1e-10f)
                    return 0.0f;

                return num / den;
            };

            float bestCorr = -2.0f;
            int bestKeyIndex = 0;    // 0-11 = C..B major, 12-23 = C..B minor
            bool bestIsMajor = true;

            // Test all 12 major keys
            for (int root = 0; root < 12; ++root)
            {
                const float corr = pearsonCorrelation (pcp, majorProfile, root);
                if (corr > bestCorr)
                {
                    bestCorr = corr;
                    bestKeyIndex = root;
                    bestIsMajor = true;
                }
            }

            // Test all 12 minor keys
            for (int root = 0; root < 12; ++root)
            {
                const float corr = pearsonCorrelation (pcp, minorProfile, root);
                if (corr > bestCorr)
                {
                    bestCorr = corr;
                    bestKeyIndex = root;
                    bestIsMajor = false;
                }
            }

            // ---------------------------------------------------------
            // 3f. Format key string and derive root chord MIDI
            // ---------------------------------------------------------
            juce::String keyString = juce::String (noteNames[bestKeyIndex])
                                   + (bestIsMajor ? " major" : " minor");

            proc.detectedKey = keyString;

            // Root chord MIDI: place in octave 4 (C4 = MIDI 60)
            const int rootMidi = 60 + bestKeyIndex;

            if (bestIsMajor)
            {
                // Major triad: root, +4, +7
                proc.rootChordMidi[0] = rootMidi;
                proc.rootChordMidi[1] = rootMidi + 4;
                proc.rootChordMidi[2] = rootMidi + 7;
            }
            else
            {
                // Minor triad: root, +3, +7
                proc.rootChordMidi[0] = rootMidi;
                proc.rootChordMidi[1] = rootMidi + 3;
                proc.rootChordMidi[2] = rootMidi + 7;
            }

            proc.rootChordValid = true;

            DBG ("GrooveScoutAnalyzer: key detected = " + keyString
                 + " (corr=" + juce::String (bestCorr, 3)
                 + ", chord MIDI=" + juce::String (proc.rootChordMidi[0])
                 + "," + juce::String (proc.rootChordMidi[1])
                 + "," + juce::String (proc.rootChordMidi[2]) + ")");
        }
    }
    else
    {
        // Key detection disabled — clear results
        proc.detectedKey = {};
        proc.rootChordValid = false;
    }

    proc.analysisProgress.store (50);

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
    DBG ("GrooveScoutAnalyzer: analysis complete (DSP.3)");
}
