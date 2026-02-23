#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>          // For juce::dsp::FFT, IIR filters
#include <juce_audio_basics/juce_audio_basics.h>  // For MidiFile, MidiMessage

// Forward declaration — avoids circular include with PluginProcessor.h
class GrooveScoutAudioProcessor;

//==============================================================================
/**
 * GrooveScoutAnalyzer — Background analysis thread
 *
 * Runs on a low-priority background thread. Reads the pre-captured audio
 * from GrooveScoutAudioProcessor::recordingBuffer and performs:
 *   DSP.1: Buffer capture infrastructure (validated)
 *   DSP.2: BPM detection (OSS + autocorrelation using juce::dsp::FFT)
 *   DSP.3: Key detection (chromagram + Krumhansl-Schmuckler)
 *   DSP.4: Drum onset detection + MIDI assembly
 *
 * Thread-safety contract:
 *   - Reads recordingBuffer ONLY after isCapturing == false
 *   - Writes analysisProgress and analysisStep atomically
 *   - Sets analysisComplete = true LAST, after all results are written
 *   - Checks threadShouldExit() between analysis steps for cancellation
 */
class GrooveScoutAnalyzer : public juce::Thread
{
public:
    explicit GrooveScoutAnalyzer (GrooveScoutAudioProcessor& processor);
    ~GrooveScoutAnalyzer() override;

    void run() override;

private:
    GrooveScoutAudioProcessor& proc;

    //==========================================================================
    // DSP.4 — Onset detection data
    //==========================================================================

    /** A single detected onset event with timing and strength. */
    struct OnsetEvent
    {
        int    sampleOffset;   ///< Position in the recording buffer (samples)
        float  strength;       ///< Half-wave rectified energy delta (onset function value)
    };

    /**
     * Detect onsets in a mono audio buffer using band-pass filtering +
     * adaptive energy-based onset detection.
     *
     * @param monoBuffer   Input mono audio (will be modified in-place by filtering)
     * @param numSamples   Number of valid samples in monoBuffer
     * @param sampleRate   Sample rate of the audio
     * @param freqLow      High-pass cutoff for band isolation
     * @param freqHigh     Low-pass cutoff for band isolation
     * @param sensitivity  Onset sensitivity (0.0 = least sensitive, 1.0 = most sensitive)
     * @return Vector of detected onset events (sample offset + strength)
     */
    std::vector<OnsetEvent> detectOnsetsInBand (std::vector<float>& monoBuffer,
                                                 int numSamples,
                                                 double sampleRate,
                                                 float freqLow,
                                                 float freqHigh,
                                                 float sensitivity,
                                                 int minGapMs = 30);

    /**
     * Write a single-track MIDI file (format 0, 480 PPQ) containing note events
     * derived from onset events for a specific drum.
     *
     * @param onsets       Detected onset events
     * @param midiNote     GM drum note number (36=kick, 38=snare, 42=hihat)
     * @param bpm          Detected BPM (used for tempo meta-event and beat conversion)
     * @param sampleRate   Sample rate of the audio
     * @param destFile     Output .mid file path
     * @return true if file was written successfully
     */
    bool writeDrumMidiFile (const std::vector<OnsetEvent>& onsets,
                            int midiNote,
                            float bpm,
                            double sampleRate,
                            const juce::File& destFile);

    /**
     * Write a root chord MIDI file from the key detection results.
     *
     * @param chordMidi    Array of 3 MIDI note numbers (root triad)
     * @param bpm          Detected BPM (for tempo meta-event)
     * @param destFile     Output .mid file path
     * @return true if file was written successfully
     */
    bool writeChordMidiFile (const int chordMidi[3],
                             float bpm,
                             const juce::File& destFile);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrooveScoutAnalyzer)
};
