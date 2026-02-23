#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>

// Forward declaration — GrooveScoutAnalyzer is defined in GrooveScoutAnalyzer.h
class GrooveScoutAnalyzer;

//==============================================================================
/**
 * GrooveScout — Audio Analysis Utility
 *
 * Plugin type: Effect (stereo in + stereo out)
 * Audio passes through unmodified. Analysis is triggered explicitly by the
 * user via the REC/STOP/Analyze workflow.
 *
 * Stage 1: Foundation + Shell — APVTS parameters declared, stub methods only.
 * Stage 2 (DSP.1): Recording buffer infrastructure + GrooveScoutAnalyzer stub thread.
 * DSP implementation (BPM/key detection, onset detection, MIDI assembly) is
 * added in DSP phases DSP.2–DSP.4.
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
    std::atomic<bool>  isPreviewActive    { false };
    std::atomic<int>   previewPlayhead   { 0 };
    std::atomic<int>   previewBand       { 0 };  // 0=kick, 1=snare, 2=hihat
    std::atomic<bool>  previewJustStarted { false };  // triggers filter reset on new preview

    // Analysis state
    std::atomic<bool>  analysisComplete  { false };
    std::atomic<int>   recordedSamples   { 0 };
    std::atomic<int>   analysisProgress  { 0 };  // 0–100 percent
    std::atomic<int>   analysisStep      { 0 };  // step index for UI label

    // Extended analysis state (Stage 2 DSP.1)
    std::atomic<bool>  analyzeTriggered  { false };
    std::atomic<bool>  analysisCancelled { false };
    std::atomic<int>   analysisError     { 0 };  // 0=none, 1=buffer too short, 2=cancelled

    // Clip availability flags (set by background thread, read by message thread)
    std::atomic<bool>  kickClipAvailable  { false };
    std::atomic<bool>  snareClipAvailable { false };
    std::atomic<bool>  hihatClipAvailable { false };
    std::atomic<bool>  chordClipAvailable { false };

    // Analysis results — set once by background thread, read after analysisComplete
    // NOT atomic: lifecycle is: background thread writes, then analysisComplete=true,
    // then message thread reads. No concurrent access.
    float          detectedBpm { 0.0f };
    juce::String   detectedKey {};

    // Root chord MIDI notes derived from key detection (DSP.3)
    // Major key: root, +4, +7 semitones (major triad)
    // Minor key: root, +3, +7 semitones (minor triad)
    // Stored as MIDI note numbers in octave 4 (C4 = 60)
    int            rootChordMidi[3] { 0, 0, 0 };
    bool           rootChordValid { false };

    // Recording buffer — pre-allocated in prepareToPlay()
    // Written by audio thread ONLY during isCapturing==true.
    // Read by background thread ONLY after isCapturing==false.
    juce::AudioBuffer<float> recordingBuffer;

    // Current sample rate — needed by GrooveScoutAnalyzer
    double currentSampleRate = 44100.0;

    // Waveform dirty flag — set by audio thread when new samples are recorded,
    // cleared by Timer callback in the editor after sending RMS data to JS.
    std::atomic<bool>      waveformDirty { false };

    //==============================================================================
    // Public action methods — called from UI thread (message thread).
    //==============================================================================

    void startRecording();
    void stopCurrentOperation();
    void startAnalysis();
    void cancelAnalysis();
    void togglePreview (const juce::String& band);

    //==============================================================================
    // Public query methods — called from UI thread (message thread).
    //==============================================================================

    float          getCaptureDurationSeconds() const;
    float          getAnalysisProgress() const;
    float          getDetectedBpm() const;
    juce::String   getDetectedKey() const;

private:
    //==============================================================================
    // Internal state
    //==============================================================================

    int    currentBlockSize    = 512;

    // Preview band-pass IIR filters — audio thread only, no locking needed.
    // Applied to recorded-buffer playback so the user hears the freq-filtered band.
    juce::IIRFilter previewHP[2];   // high-pass (L, R)
    juce::IIRFilter previewLP[2];   // low-pass  (L, R)
    float  previewLastFreqLow  = -1.0f;
    float  previewLastFreqHigh = -1.0f;
    int    previewLastBand     = -1;

    // Cached raw parameter pointers — set in prepareToPlay(), used in processBlock()
    // to avoid HashMap lookup on every audio callback.
    std::atomic<float>* p_kickFreqLow      = nullptr;
    std::atomic<float>* p_kickFreqHigh     = nullptr;
    std::atomic<float>* p_snareFreqLow     = nullptr;
    std::atomic<float>* p_snareFreqHigh    = nullptr;
    std::atomic<float>* p_hihatFreqLow     = nullptr;
    std::atomic<float>* p_hihatFreqHigh    = nullptr;
    std::atomic<float>* p_kickSensitivity  = nullptr;
    std::atomic<float>* p_snareSensitivity = nullptr;
    std::atomic<float>* p_hihatSensitivity = nullptr;

    // Sensitivity gate state — audio thread only, no locking needed.
    // Mirrors the adaptive threshold used in detectOnsetsInBand() so the user
    // hears exactly which transients would pass during analysis.
    float previewGateEnv    = 0.0f;   // envelope follower (instant attack, ~75ms release)
    float previewGatePeak   = 0.0f;   // slow-decaying peak reference for threshold
    float previewGateSmooth = 0.0f;   // smoothed gate output (avoids clicks)

    // Background analysis thread
    std::unique_ptr<GrooveScoutAnalyzer> analyzerThread;

    // Waveform RMS data — kept private (no longer used; editor computes RMS inline)
    std::vector<float>     waveformRms;
    juce::CriticalSection  waveformLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrooveScoutAudioProcessor)
};
