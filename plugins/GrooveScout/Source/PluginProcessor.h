#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>

//==============================================================================
/**
 * GrooveScout — Audio Analysis Utility
 *
 * Plugin type: Effect (stereo in + stereo out)
 * Audio passes through unmodified. Analysis is triggered explicitly by the
 * user via the REC/STOP/Analyze workflow.
 *
 * Stage 1: Foundation + Shell — APVTS parameters declared, stub methods only.
 * DSP implementation (recording buffer, BPM/key detection, onset detection,
 * MIDI assembly) is added in Stage 2 (DSP phases DSP.1–DSP.4).
 */
class GrooveScoutAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    GrooveScoutAudioProcessor();
    ~GrooveScoutAudioProcessor() override;

    //==============================================================================
    // AudioProcessor interface
    //==============================================================================

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    // Editor
    //==============================================================================

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    // Plugin metadata
    //==============================================================================

    const juce::String getName() const override { return "GrooveScout"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getLatencySamples() const { return 0; }

    //==============================================================================
    // Programs
    //==============================================================================

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    //==============================================================================
    // State serialization
    //==============================================================================

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // APVTS — parameter management
    //==============================================================================

    juce::AudioProcessorValueTreeState parameters;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    // Atomic state flags — written by audio thread or message thread,
    // read by the other. No mutexes needed (see architecture.md thread-safety notes).
    //==============================================================================

    // Recording state
    std::atomic<bool>  isCapturing       { false };
    std::atomic<bool>  recordingComplete { false };

    // Preview state
    std::atomic<bool>  isPreviewActive   { false };
    std::atomic<int>   previewPlayhead   { 0 };
    std::atomic<int>   previewBand       { 0 };  // 0=kick, 1=snare, 2=hihat

    // Analysis state
    std::atomic<bool>  analysisComplete  { false };
    std::atomic<int>   recordedSamples   { 0 };
    std::atomic<int>   analysisProgress  { 0 };  // 0–100 percent
    std::atomic<int>   analysisStep      { 0 };  // step index for UI label

    //==============================================================================
    // Public action methods — called from UI thread (message thread).
    // Implementations are stubs in Stage 1; DSP agent fills them in Stage 2.
    //==============================================================================

    void startRecording();
    void stopCurrentOperation();
    void startAnalysis();
    void togglePreview (const juce::String& band);

    //==============================================================================
    // Public query methods — called from UI thread (message thread).
    // Implementations return stub values in Stage 1.
    //==============================================================================

    float          getCaptureDurationSeconds() const;
    float          getAnalysisProgress() const;
    float          getDetectedBpm() const;
    juce::String   getDetectedKey() const;

private:
    //==============================================================================
    // Internal state (Stage 1 stubs — populated by DSP agent in Stage 2)
    //==============================================================================

    double currentSampleRate   = 44100.0;
    int    currentBlockSize    = 512;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrooveScoutAudioProcessor)
};
