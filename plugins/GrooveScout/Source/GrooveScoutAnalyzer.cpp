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
// Phase DSP.4: Band-separated drum onset detection (kick, snare, hihat)
// using cascaded IIR bandpass filtering + adaptive energy-based onset
// detection. MIDI pattern assembly for each drum plus root chord.
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
    DBG ("GrooveScoutAnalyzer: analysis started (DSP.4)");

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
    // 4. Drum Onset Detection (DSP.4)
    //    Per-band IIR bandpass filtering + adaptive onset detection.
    //    Bands: kick, snare, hihat — each gated by its analyze* toggle.
    // -------------------------------------------------------------------------
    proc.analysisStep.store (3);      // UI label: "Detecting Drums..."
    proc.analysisProgress.store (50);

    const int onsetNumRecorded = proc.recordedSamples.load();
    const double onsetSr       = proc.currentSampleRate;

    // Read analyze toggles
    auto* analyzeKickParam  = proc.parameters.getRawParameterValue ("analyzeKick");
    auto* analyzeSnareParam = proc.parameters.getRawParameterValue ("analyzeSnare");
    auto* analyzeHihatParam = proc.parameters.getRawParameterValue ("analyzeHihat");

    const bool doKick  = (analyzeKickParam  == nullptr) || (analyzeKickParam->load()  > 0.5f);
    const bool doSnare = (analyzeSnareParam == nullptr) || (analyzeSnareParam->load() > 0.5f);
    const bool doHihat = (analyzeHihatParam == nullptr) || (analyzeHihatParam->load() > 0.5f);

    // Read frequency and sensitivity parameters
    auto readFloat = [&] (const char* id, float fallback) -> float
    {
        auto* p = proc.parameters.getRawParameterValue (id);
        return (p != nullptr) ? p->load() : fallback;
    };

    const float kickFreqLow   = readFloat ("kickFreqLow",   40.0f);
    const float kickFreqHigh  = readFloat ("kickFreqHigh",  120.0f);
    const float kickSens      = readFloat ("kickSensitivity", 0.5f);

    const float snareFreqLow  = readFloat ("snareFreqLow",  200.0f);
    const float snareFreqHigh = readFloat ("snareFreqHigh", 8000.0f);
    const float snareSens     = readFloat ("snareSensitivity", 0.5f);

    const float hihatFreqLow  = readFloat ("hihatFreqLow",  5000.0f);
    const float hihatFreqHigh = readFloat ("hihatFreqHigh", 16000.0f);
    const float hihatSens     = readFloat ("hihatSensitivity", 0.5f);

    // Mix recording buffer to mono for onset detection.
    // Each band gets its own copy (filtering is destructive).
    auto makeMono = [&] () -> std::vector<float>
    {
        std::vector<float> mono (static_cast<size_t> (onsetNumRecorded));
        const float* left  = proc.recordingBuffer.getReadPointer (0);
        const float* right = proc.recordingBuffer.getReadPointer (1);
        for (int i = 0; i < onsetNumRecorded; ++i)
            mono[static_cast<size_t> (i)] = (left[i] + right[i]) * 0.5f;
        return mono;
    };

    // Onset lists per drum
    std::vector<OnsetEvent> kickOnsets;
    std::vector<OnsetEvent> snareOnsets;
    std::vector<OnsetEvent> hihatOnsets;

    // --- Kick band ---
    if (doKick && kickFreqLow < kickFreqHigh)
    {
        auto mono = makeMono();
        kickOnsets = detectOnsetsInBand (mono, onsetNumRecorded, onsetSr,
                                         kickFreqLow, kickFreqHigh, kickSens,
                                         80);  // 80ms min gap — kick can't repeat faster
        DBG ("GrooveScoutAnalyzer: kick onsets detected = " + juce::String (static_cast<int> (kickOnsets.size())));
    }

    proc.analysisProgress.store (58);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // --- Snare band ---
    if (doSnare && snareFreqLow < snareFreqHigh)
    {
        auto mono = makeMono();
        snareOnsets = detectOnsetsInBand (mono, onsetNumRecorded, onsetSr,
                                          snareFreqLow, snareFreqHigh, snareSens,
                                          60);  // 60ms min gap — snare minimum realistic spacing
        DBG ("GrooveScoutAnalyzer: snare onsets detected = " + juce::String (static_cast<int> (snareOnsets.size())));
    }

    proc.analysisProgress.store (66);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // --- Hihat band ---
    if (doHihat && hihatFreqLow < hihatFreqHigh)
    {
        auto mono = makeMono();
        hihatOnsets = detectOnsetsInBand (mono, onsetNumRecorded, onsetSr,
                                          hihatFreqLow, hihatFreqHigh, hihatSens,
                                          30);  // 30ms min gap — hihats can be dense (16ths)
        DBG ("GrooveScoutAnalyzer: hihat onsets detected = " + juce::String (static_cast<int> (hihatOnsets.size())));
    }

    proc.analysisProgress.store (75);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // -------------------------------------------------------------------------
    // 5. MIDI Pattern Assembly (DSP.4)
    //    Write per-drum MIDI files + root chord MIDI to temp directory.
    // -------------------------------------------------------------------------
    proc.analysisStep.store (4);      // UI label: "Writing MIDI..."

    // Determine BPM for MIDI tempo event. If BPM was not detected (0), use 120 BPM default.
    const float midiTempoBpm = (proc.detectedBpm > 0.0f) ? proc.detectedBpm : 120.0f;

    // Create GrooveScout subdirectory in system temp directory
    juce::File tempDir = juce::File::getSpecialLocation (juce::File::tempDirectory)
                             .getChildFile ("GrooveScout");
    tempDir.createDirectory();

    // --- Write kick MIDI ---
    if (! kickOnsets.empty())
    {
        juce::File kickFile = tempDir.getChildFile ("groovescout_kick.mid");
        if (writeDrumMidiFile (kickOnsets, 36, midiTempoBpm, onsetSr, kickFile))
        {
            proc.kickClipAvailable.store (true);
            DBG ("GrooveScoutAnalyzer: wrote " + kickFile.getFullPathName());
        }
    }

    proc.analysisProgress.store (80);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // --- Write snare MIDI ---
    if (! snareOnsets.empty())
    {
        juce::File snareFile = tempDir.getChildFile ("groovescout_snare.mid");
        if (writeDrumMidiFile (snareOnsets, 38, midiTempoBpm, onsetSr, snareFile))
        {
            proc.snareClipAvailable.store (true);
            DBG ("GrooveScoutAnalyzer: wrote " + snareFile.getFullPathName());
        }
    }

    proc.analysisProgress.store (85);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // --- Write hihat MIDI ---
    if (! hihatOnsets.empty())
    {
        juce::File hihatFile = tempDir.getChildFile ("groovescout_hihat.mid");
        if (writeDrumMidiFile (hihatOnsets, 42, midiTempoBpm, onsetSr, hihatFile))
        {
            proc.hihatClipAvailable.store (true);
            DBG ("GrooveScoutAnalyzer: wrote " + hihatFile.getFullPathName());
        }
    }

    proc.analysisProgress.store (90);

    if (threadShouldExit())
    {
        proc.analysisCancelled.store (true);
        return;
    }

    // --- Write root chord MIDI ---
    if (proc.rootChordValid)
    {
        juce::File chordFile = tempDir.getChildFile ("groovescout_chord.mid");
        if (writeChordMidiFile (proc.rootChordMidi, midiTempoBpm, chordFile))
        {
            proc.chordClipAvailable.store (true);
            DBG ("GrooveScoutAnalyzer: wrote " + chordFile.getFullPathName());
        }
    }

    proc.analysisProgress.store (95);

    // Final progress + completion
    proc.analysisProgress.store (100);

    // CRITICAL: set analysisComplete LAST — after all result fields are written.
    // Message thread reads results only after seeing analysisComplete == true.
    proc.analysisComplete.store (true);
    DBG ("GrooveScoutAnalyzer: analysis complete (DSP.4)");
}

//==============================================================================
// DSP.4 Helper: Band-separated onset detection
//==============================================================================

std::vector<GrooveScoutAnalyzer::OnsetEvent>
GrooveScoutAnalyzer::detectOnsetsInBand (std::vector<float>& monoBuffer,
                                          int numSamples,
                                          double sampleRate,
                                          float freqLow,
                                          float freqHigh,
                                          float sensitivity,
                                          int minGapMs)
{
    std::vector<OnsetEvent> onsets;

    if (numSamples < 256 || freqLow >= freqHigh)
        return onsets;

    // -----------------------------------------------------------------
    // Step 1: Apply bandpass filter (HP then LP in series, offline)
    //         Using juce::dsp::IIR::Filter processSample() per sample.
    // -----------------------------------------------------------------

    // High-pass at freqLow (Butterworth Q = 0.707)
    {
        juce::dsp::IIR::Filter<float> hpFilter;
        hpFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (
            sampleRate, freqLow, 0.707f);
        hpFilter.reset();

        for (int i = 0; i < numSamples; ++i)
            monoBuffer[static_cast<size_t> (i)] = hpFilter.processSample (monoBuffer[static_cast<size_t> (i)]);
    }

    // Low-pass at freqHigh (Butterworth Q = 0.707)
    {
        juce::dsp::IIR::Filter<float> lpFilter;
        lpFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (
            sampleRate, freqHigh, 0.707f);
        lpFilter.reset();

        for (int i = 0; i < numSamples; ++i)
            monoBuffer[static_cast<size_t> (i)] = lpFilter.processSample (monoBuffer[static_cast<size_t> (i)]);
    }

    // -----------------------------------------------------------------
    // Step 2: Compute per-frame RMS energy and onset function
    //         Window: 256 samples, hop: 128 samples
    //         O[n] = max(0, E[n] - E[n-1])  (half-wave rectified delta)
    // -----------------------------------------------------------------
    const int windowSize = 256;
    const int hopSize    = 128;

    const int numFrames = (numSamples - windowSize) / hopSize + 1;
    if (numFrames < 2)
        return onsets;

    std::vector<float> energy (static_cast<size_t> (numFrames));
    std::vector<float> onsetFunc (static_cast<size_t> (numFrames));

    // Compute RMS energy per frame
    for (int f = 0; f < numFrames; ++f)
    {
        const int start = f * hopSize;
        float sum = 0.0f;

        for (int j = 0; j < windowSize; ++j)
        {
            const float s = monoBuffer[static_cast<size_t> (start + j)];
            sum += s * s;
        }

        energy[static_cast<size_t> (f)] = std::sqrt (sum / static_cast<float> (windowSize));
    }

    // Compute onset function: half-wave rectified energy delta
    onsetFunc[0] = 0.0f;
    for (int f = 1; f < numFrames; ++f)
        onsetFunc[static_cast<size_t> (f)] = std::max (0.0f,
                                                         energy[static_cast<size_t> (f)]
                                                       - energy[static_cast<size_t> (f - 1)]);

    // -----------------------------------------------------------------
    // Step 3: Adaptive threshold + peak detection
    //         T[n] = mean(O[n-w..n]) + (1 - sensitivity) * 4 * std(O[n-w..n])
    //         w = 40 frames (~500ms window)
    //         Minimum inter-onset interval: band-specific (ms → frames)
    // -----------------------------------------------------------------
    const int threshWindow = 40;

    // Convert caller-specified minimum gap (ms) to frames
    // frames = ms * sampleRate / (1000 * hopSize)
    const int minOnsetGap = std::max (1,
        static_cast<int> (std::round (minGapMs * sampleRate / (1000.0 * hopSize))));

    // Find peak onset strength for velocity normalization
    float peakOnset = 0.0f;
    for (int f = 0; f < numFrames; ++f)
        peakOnset = std::max (peakOnset, onsetFunc[static_cast<size_t> (f)]);

    if (peakOnset < 1e-10f)
        return onsets;  // No energy in this band

    // Sensitivity-scaled strength floor: at low sensitivity only the strongest
    // transients (dominant hits) survive; at high sensitivity most hits are kept.
    //   sens=0.0 → floor = 55% of peak  (only top-tier hits)
    //   sens=0.5 → floor = 30% of peak  (reasonable selectivity)
    //   sens=1.0 → floor =  5% of peak  (catch nearly everything)
    const float strengthFloor = peakOnset * (0.05f + (1.0f - sensitivity) * 0.50f);

    int cooldown = 0;  // frames remaining in suppression window

    for (int f = 1; f < numFrames; ++f)
    {
        if (cooldown > 0)
        {
            --cooldown;
            continue;
        }

        // Compute adaptive threshold over the previous `threshWindow` frames
        const int wStart = std::max (0, f - threshWindow);
        const int wLen   = f - wStart;

        float mean = 0.0f;
        for (int k = wStart; k < f; ++k)
            mean += onsetFunc[static_cast<size_t> (k)];
        mean /= static_cast<float> (wLen);

        float variance = 0.0f;
        for (int k = wStart; k < f; ++k)
        {
            const float diff = onsetFunc[static_cast<size_t> (k)] - mean;
            variance += diff * diff;
        }
        variance /= static_cast<float> (wLen);
        const float stdDev = std::sqrt (variance);

        // Multiplier raised to 6× (was 4×) so low-sensitivity settings demand
        // a much larger energy increase relative to local variance.
        const float threshold = mean + (1.0f - sensitivity) * 6.0f * stdDev;

        // Check if current frame exceeds threshold AND the absolute strength floor
        const float val = onsetFunc[static_cast<size_t> (f)];
        if (val <= threshold || val < strengthFloor)
            continue;

        // Local maximum check: O[f] > O[f-1] and O[f] >= O[f+1]
        const float prev = onsetFunc[static_cast<size_t> (f - 1)];
        const float next = (f + 1 < numFrames) ? onsetFunc[static_cast<size_t> (f + 1)] : 0.0f;

        if (val > prev && val >= next)
        {
            // Detected onset
            OnsetEvent ev;
            ev.sampleOffset = f * hopSize + windowSize / 2;  // Center of the window
            ev.strength     = val;
            onsets.push_back (ev);

            cooldown = minOnsetGap;
        }
    }

    return onsets;
}

//==============================================================================
// DSP.4 Helper: Write drum MIDI file
//==============================================================================

bool GrooveScoutAnalyzer::writeDrumMidiFile (const std::vector<OnsetEvent>& onsets,
                                              int midiNote,
                                              float bpm,
                                              double sampleRate,
                                              const juce::File& destFile)
{
    if (onsets.empty())
        return false;

    const int ppq = 480;  // Pulses per quarter note
    const int noteDurationTicks = 60;  // 1/32nd note at 480 PPQ (480 / 8 = 60)

    // Tempo meta-event: microseconds per beat = 60,000,000 / BPM
    const int microsPerBeat = static_cast<int> (60000000.0 / static_cast<double> (bpm));

    // Find peak onset strength for velocity normalization
    float peakStrength = 0.0f;
    for (const auto& ev : onsets)
        peakStrength = std::max (peakStrength, ev.strength);

    if (peakStrength < 1e-10f)
        peakStrength = 1.0f;

    juce::MidiMessageSequence seq;

    // Add tempo meta-event at tick 0
    seq.addEvent (juce::MidiMessage::tempoMetaEvent (microsPerBeat), 0.0);

    // Add note events for each detected onset (channel 10 = drum channel, 0-indexed = 9)
    for (const auto& ev : onsets)
    {
        // Convert sample offset to beat position
        // beatPosition = (sampleOffset / sampleRate) * (bpm / 60.0)
        const double timeSeconds = static_cast<double> (ev.sampleOffset) / sampleRate;
        const double beatPosition = timeSeconds * (static_cast<double> (bpm) / 60.0);

        // Convert beat position to MIDI ticks
        const int tick = static_cast<int> (std::round (beatPosition * ppq));
        if (tick < 0)
            continue;

        // Onset strength to velocity (1-127)
        int velocity = static_cast<int> (std::round ((ev.strength / peakStrength) * 127.0f));
        velocity = juce::jlimit (1, 127, velocity);

        // Note on at tick, note off at tick + noteDurationTicks
        // MIDI channel 10 (0-indexed = 9) for drums
        seq.addEvent (juce::MidiMessage::noteOn  (10, midiNote, static_cast<juce::uint8> (velocity)),
                      static_cast<double> (tick));
        seq.addEvent (juce::MidiMessage::noteOff (10, midiNote, static_cast<juce::uint8> (0)),
                      static_cast<double> (tick + noteDurationTicks));
    }

    seq.updateMatchedPairs();

    // Create MIDI file (format 0, 480 PPQ)
    juce::MidiFile mf;
    mf.setTicksPerQuarterNote (ppq);
    mf.addTrack (seq);

    // Write to disk (overwrite existing)
    destFile.deleteFile();
    juce::FileOutputStream fos (destFile);

    if (fos.failedToOpen())
    {
        DBG ("GrooveScoutAnalyzer: failed to open " + destFile.getFullPathName());
        return false;
    }

    mf.writeTo (fos, 0);  // Format 0 = single track
    fos.flush();
    return true;
}

//==============================================================================
// DSP.4 Helper: Write root chord MIDI file
//==============================================================================

bool GrooveScoutAnalyzer::writeChordMidiFile (const int chordMidi[3],
                                               float bpm,
                                               const juce::File& destFile)
{
    const int ppq = 480;
    const int chordDurationTicks = 1920;  // 4 beats at 480 PPQ

    // Tempo meta-event: microseconds per beat = 60,000,000 / BPM
    const int microsPerBeat = static_cast<int> (60000000.0 / static_cast<double> (bpm));

    juce::MidiMessageSequence seq;

    // Add tempo meta-event at tick 0
    seq.addEvent (juce::MidiMessage::tempoMetaEvent (microsPerBeat), 0.0);

    // Add 3 simultaneous notes (channel 1, 0-indexed = 0) at tick 0
    for (int i = 0; i < 3; ++i)
    {
        seq.addEvent (juce::MidiMessage::noteOn  (1, chordMidi[i], static_cast<juce::uint8> (100)),
                      0.0);
        seq.addEvent (juce::MidiMessage::noteOff (1, chordMidi[i], static_cast<juce::uint8> (0)),
                      static_cast<double> (chordDurationTicks));
    }

    seq.updateMatchedPairs();

    // Create MIDI file (format 0, 480 PPQ)
    juce::MidiFile mf;
    mf.setTicksPerQuarterNote (ppq);
    mf.addTrack (seq);

    // Write to disk (overwrite existing)
    destFile.deleteFile();
    juce::FileOutputStream fos (destFile);

    if (fos.failedToOpen())
    {
        DBG ("GrooveScoutAnalyzer: failed to open " + destFile.getFullPathName());
        return false;
    }

    mf.writeTo (fos, 0);  // Format 0 = single track
    fos.flush();
    return true;
}
