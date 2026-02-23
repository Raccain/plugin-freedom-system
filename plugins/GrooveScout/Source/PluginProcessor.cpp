#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GrooveScoutAnalyzer.h"

//==============================================================================
// Parameter layout — EXACT specification from parameter-spec.md
// Order and values are immutable during Stages 1–5.
// Total: 15 parameters (9 Float, 1 Choice, 5 Bool)
//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout
GrooveScoutAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // -------------------------------------------------------------------------
    // Capture
    // -------------------------------------------------------------------------

    // 1. captureDuration — Float, 1.0–30.0 s, linear, default 15.0
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "captureDuration", 1 },
        "Capture Duration",
        juce::NormalisableRange<float> (1.0f, 30.0f),
        15.0f,
        "s"
    ));

    // -------------------------------------------------------------------------
    // Kick detection
    // -------------------------------------------------------------------------

    // 2. kickSensitivity — Float, 0.0–1.0, linear, default 0.5
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "kickSensitivity", 1 },
        "Kick Sensitivity",
        juce::NormalisableRange<float> (0.0f, 1.0f),
        0.5f
    ));

    // 3. kickFreqLow — Float, 20–1000 Hz, logarithmic (skew 0.4), default 40 Hz
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "kickFreqLow", 1 },
        "Kick Freq Low",
        juce::NormalisableRange<float> (20.0f, 1000.0f, 0.0f, 0.4f),
        40.0f,
        "Hz"
    ));

    // 4. kickFreqHigh — Float, 20–1000 Hz, logarithmic (skew 0.4), default 120 Hz
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "kickFreqHigh", 1 },
        "Kick Freq High",
        juce::NormalisableRange<float> (20.0f, 1000.0f, 0.0f, 0.4f),
        120.0f,
        "Hz"
    ));

    // -------------------------------------------------------------------------
    // Snare detection
    // -------------------------------------------------------------------------

    // 5. snareSensitivity — Float, 0.0–1.0, linear, default 0.5
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "snareSensitivity", 1 },
        "Snare Sensitivity",
        juce::NormalisableRange<float> (0.0f, 1.0f),
        0.5f
    ));

    // 6. snareFreqLow — Float, 100–20000 Hz, logarithmic (skew 0.25), default 200 Hz
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "snareFreqLow", 1 },
        "Snare Freq Low",
        juce::NormalisableRange<float> (100.0f, 20000.0f, 0.0f, 0.25f),
        200.0f,
        "Hz"
    ));

    // 7. snareFreqHigh — Float, 100–20000 Hz, logarithmic (skew 0.25), default 8000 Hz
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "snareFreqHigh", 1 },
        "Snare Freq High",
        juce::NormalisableRange<float> (100.0f, 20000.0f, 0.0f, 0.25f),
        8000.0f,
        "Hz"
    ));

    // -------------------------------------------------------------------------
    // Hihat detection
    // -------------------------------------------------------------------------

    // 8. hihatSensitivity — Float, 0.0–1.0, linear, default 0.5
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "hihatSensitivity", 1 },
        "Hihat Sensitivity",
        juce::NormalisableRange<float> (0.0f, 1.0f),
        0.5f
    ));

    // 9. hihatFreqLow — Float, 1000–20000 Hz, logarithmic (skew 0.4), default 5000 Hz
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "hihatFreqLow", 1 },
        "Hihat Freq Low",
        juce::NormalisableRange<float> (1000.0f, 20000.0f, 0.0f, 0.4f),
        5000.0f,
        "Hz"
    ));

    // 10. hihatFreqHigh — Float, 1000–20000 Hz, logarithmic (skew 0.4), default 16000 Hz
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "hihatFreqHigh", 1 },
        "Hihat Freq High",
        juce::NormalisableRange<float> (1000.0f, 20000.0f, 0.0f, 0.4f),
        16000.0f,
        "Hz"
    ));

    // -------------------------------------------------------------------------
    // BPM Control
    // -------------------------------------------------------------------------

    // 11. bpmMultiplier — Choice: "1/2x", "1x", "2x", default index 1 ("1x")
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { "bpmMultiplier", 1 },
        "BPM Multiplier",
        juce::StringArray { "1/2x", "1x", "2x" },
        1  // default: "1x"
    ));

    // -------------------------------------------------------------------------
    // Analysis Toggles
    // -------------------------------------------------------------------------

    // 12. analyzeBPM — Bool, default true
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "analyzeBPM", 1 },
        "Analyze BPM",
        true
    ));

    // 13. analyzeKey — Bool, default true
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "analyzeKey", 1 },
        "Analyze Key",
        true
    ));

    // 14. analyzeKick — Bool, default true
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "analyzeKick", 1 },
        "Analyze Kick",
        true
    ));

    // 15. analyzeSnare — Bool, default true
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "analyzeSnare", 1 },
        "Analyze Snare",
        true
    ));

    // 16. analyzeHihat — Bool, default true
    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { "analyzeHihat", 1 },
        "Analyze Hihat",
        true
    ));

    return layout;
}

//==============================================================================
// Constructor
//==============================================================================

GrooveScoutAudioProcessor::GrooveScoutAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , parameters (*this, nullptr, "Parameters", createParameterLayout())
{
}

GrooveScoutAudioProcessor::~GrooveScoutAudioProcessor()
{
    // CRITICAL: stop background thread before destruction
    if (analyzerThread)
        analyzerThread->stopThread (2000);
}

//==============================================================================
// Prepare / release
//==============================================================================

void GrooveScoutAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize  = samplesPerBlock;

    // Pre-allocate recording buffer: max 30s × sampleRate × 2 channels.
    // This is the ONLY place we allocate — NEVER allocate in processBlock().
    const int maxCaptureSamples = static_cast<int> (sampleRate * 30.0);
    recordingBuffer.setSize (2, maxCaptureSamples, false, true, false);
    recordedSamples.store (0);

    // Pre-allocate waveform RMS circular buffer (200 buckets for display)
    {
        juce::ScopedLock sl (waveformLock);
        waveformRms.assign (200, 0.0f);
    }
    waveformDirty.store (false);

    // Reset all analysis state on prepare (new session / sample rate change)
    isCapturing.store (false);
    recordingComplete.store (false);
    analyzeTriggered.store (false);
    analysisComplete.store (false);
    analysisCancelled.store (false);
    analysisProgress.store (0);
    analysisStep.store (0);
    analysisError.store (0);
}

void GrooveScoutAudioProcessor::releaseResources()
{
    // Cancel any active analysis thread
    if (analyzerThread && analyzerThread->isThreadRunning())
        analyzerThread->stopThread (2000);

    isCapturing.store (false);
    isPreviewActive.store (false);
}

bool GrooveScoutAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only stereo in + stereo out supported (or mono in/out for flexibility).
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono())
        return false;

    if (layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet())
        return false;

    return true;
}

//==============================================================================
// Process block
//==============================================================================

void GrooveScoutAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused (midiMessages);

    const int numSamples  = buffer.getNumSamples();
    const int numChannels = juce::jmin (buffer.getNumChannels(), 2);

    // Recording — append incoming audio to pre-allocated recording buffer.
    // Audio thread ONLY writes during isCapturing == true.
    // All reads of recordingBuffer happen in background thread AFTER isCapturing == false.
    if (isCapturing.load())
    {
        const int currentHead    = recordedSamples.load();
        const int bufferCapacity = recordingBuffer.getNumSamples();
        const int samplesAvailable = bufferCapacity - currentHead;

        if (samplesAvailable > 0)
        {
            const int samplesToCopy = juce::jmin (numSamples, samplesAvailable);

            for (int ch = 0; ch < numChannels; ++ch)
                recordingBuffer.copyFrom (ch, currentHead,
                                          buffer.getReadPointer (ch), samplesToCopy);

            recordedSamples.store (currentHead + samplesToCopy);

            // Signal waveform update — UI Timer polls waveformDirty at ~60ms
            // (actual RMS value push happens in a Timer callback, not here)
            waveformDirty.store (true);

            if (samplesAvailable <= numSamples)
            {
                // Buffer full — auto-stop capture
                isCapturing.store (false);
                recordingComplete.store (true);
                DBG ("GrooveScout: recording buffer full, auto-stopped");
            }
        }
    }

    // Audio passthrough — GrooveScout is an analysis utility.
    // The buffer already contains input audio and is routed to the output bus
    // by JUCE automatically. No modification needed.
    juce::ignoreUnused (buffer);
}

//==============================================================================
// Editor
//==============================================================================

juce::AudioProcessorEditor* GrooveScoutAudioProcessor::createEditor()
{
    return new GrooveScoutAudioProcessorEditor (*this);
}

//==============================================================================
// State serialization — saves/loads all APVTS parameters via DAW presets
//==============================================================================

void GrooveScoutAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void GrooveScoutAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr
        && xmlState->hasTagName (parameters.state.getType()))
    {
        parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
    }
}

//==============================================================================
// Public action methods — called from UI thread (message thread).
//==============================================================================

void GrooveScoutAudioProcessor::startRecording()
{
    // Stop any running analysis thread first
    if (analyzerThread && analyzerThread->isThreadRunning())
        analyzerThread->stopThread (2000);

    // Reset all state flags
    recordedSamples.store (0);
    analysisComplete.store (false);
    analysisCancelled.store (false);
    analysisError.store (0);
    recordingComplete.store (false);
    kickClipAvailable.store (false);
    snareClipAvailable.store (false);
    hihatClipAvailable.store (false);
    chordClipAvailable.store (false);

    // Begin capture — audio thread starts appending on next processBlock()
    isCapturing.store (true);
    DBG ("GrooveScout: startRecording()");
}

void GrooveScoutAudioProcessor::stopCurrentOperation()
{
    isCapturing.store (false);
    isPreviewActive.store (false);

    if (analyzerThread && analyzerThread->isThreadRunning())
        analyzerThread->stopThread (2000);

    DBG ("GrooveScout: stopCurrentOperation()");
}

void GrooveScoutAudioProcessor::startAnalysis()
{
    if (recordedSamples.load() == 0)
    {
        DBG ("GrooveScout: startAnalysis() called with empty buffer — setting complete so JS can escape analyzing state");
        analyzeTriggered.store (true);
        analysisError.store (1);   // 1 = buffer empty
        analysisComplete.store (true);
        return;
    }

    // Ensure capture and preview are stopped before analysis begins
    isCapturing.store (false);
    isPreviewActive.store (false);

    // Reset analysis state
    analyzeTriggered.store (true);
    analysisComplete.store (false);
    analysisCancelled.store (false);
    analysisError.store (0);
    analysisProgress.store (0);
    analysisStep.store (0);

    // Create analyzer thread on first use, then start it
    if (!analyzerThread)
        analyzerThread = std::make_unique<GrooveScoutAnalyzer> (*this);

    if (analyzerThread->isThreadRunning())
        analyzerThread->stopThread (2000);

    analyzerThread->startThread (juce::Thread::Priority::low);
    DBG ("GrooveScout: startAnalysis() — thread started");
}

void GrooveScoutAudioProcessor::cancelAnalysis()
{
    if (analyzerThread && analyzerThread->isThreadRunning())
    {
        analyzerThread->stopThread (2000);
        analysisCancelled.store (true);
        analysisProgress.store (0);
        analysisStep.store (0);
        DBG ("GrooveScout: cancelAnalysis()");
    }
}

void GrooveScoutAudioProcessor::togglePreview (const juce::String& band)
{
    // Phase DSP.1: stub — preview playback implemented in a future phase
    // when we have IIR filter for band-isolation.
    DBG ("GrooveScout: togglePreview(" + band + ") — stub");
    juce::ignoreUnused (band);
}

//==============================================================================
// Public query methods
//==============================================================================

float GrooveScoutAudioProcessor::getCaptureDurationSeconds() const
{
    auto* param = parameters.getRawParameterValue ("captureDuration");
    return (param != nullptr) ? param->load() : 15.0f;
}

float GrooveScoutAudioProcessor::getAnalysisProgress() const
{
    return static_cast<float> (analysisProgress.load()) / 100.0f;
}

float GrooveScoutAudioProcessor::getDetectedBpm() const
{
    // Stage 2 (DSP.2) will return the BPM published by the analysis thread.
    // Read is safe: detectedBpm is set once by background thread before
    // analysisComplete becomes true. Caller should check analysisComplete first.
    return detectedBpm;
}

juce::String GrooveScoutAudioProcessor::getDetectedKey() const
{
    // Stage 2 (DSP.3) will return the key string published by the analysis thread.
    return detectedKey;
}

//==============================================================================
// Plugin factory — required by JUCE plugin client
//==============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GrooveScoutAudioProcessor();
}
