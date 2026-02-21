#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>   // For juce::dsp::FFT

// Forward declaration — avoids circular include with PluginProcessor.h
class GrooveScoutAudioProcessor;

//==============================================================================
/**
 * GrooveScoutAnalyzer — Background analysis thread
 *
 * Runs on a low-priority background thread. Reads the pre-captured audio
 * from GrooveScoutAudioProcessor::recordingBuffer and performs:
 *   DSP.1: Stub (validates buffer length, simulates progress steps)
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrooveScoutAnalyzer)
};
