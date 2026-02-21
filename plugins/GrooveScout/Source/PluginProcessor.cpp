#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Parameter layout — EXACT specification from parameter-spec.md
// Order and values are immutable during Stages 1–5.
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
}

//==============================================================================
// Prepare / release
//==============================================================================

void GrooveScoutAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize  = samplesPerBlock;

    // Stage 2 (DSP.1) will:
    // - Pre-allocate recording buffer: max 30s * sampleRate * 2 channels
    // - Initialise preview IIR filter state
    // - Initialise waveform RMS accumulator

    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void GrooveScoutAudioProcessor::releaseResources()
{
    // Stage 2 will reset filter state and cancel any active preview here.
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

    // GrooveScout is an analysis utility: audio passes through unmodified.
    // Stage 2 (DSP.1) will add:
    //   - Recording buffer append (when isCapturing == true)
    //   - Preview playback from recording buffer (when isPreviewActive == true)
    //   - Waveform RMS accumulator update
    //
    // In Stage 1, the buffer is passed through without modification.
    // (JUCE routes the input bus to the output bus automatically when no
    //  modification is made — no explicit copy needed here.)

    // Parameter access example (for Stage 2 DSP implementation):
    // auto* captureDurationParam = parameters.getRawParameterValue ("captureDuration");
    // float captureDurationSec = captureDurationParam->load();  // Atomic read

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
// Public action methods — stubs for Stage 1
// DSP agent fills these in Stage 2 (Phase DSP.1).
//==============================================================================

void GrooveScoutAudioProcessor::startRecording()
{
    // Stage 2 implementation:
    //   recordedSamples.store (0);
    //   isCapturing.store (true);
    //   recordingComplete.store (false);
    //   (cancel any active preview)
    DBG ("GrooveScout: startRecording() called (stub)");
}

void GrooveScoutAudioProcessor::stopCurrentOperation()
{
    // Stage 2 implementation:
    //   isCapturing.store (false);
    //   isPreviewActive.store (false);
    //   analyzerThread.stopThread (2000);  // if analysis running
    DBG ("GrooveScout: stopCurrentOperation() called (stub)");
}

void GrooveScoutAudioProcessor::startAnalysis()
{
    // Stage 2 implementation:
    //   if (recordedSamples.load() == 0) return;
    //   isPreviewActive.store (false);
    //   analyzerThread.stopThread (2000);
    //   analyzerThread.startThread (juce::Thread::Priority::low);
    DBG ("GrooveScout: startAnalysis() called (stub)");
}

void GrooveScoutAudioProcessor::togglePreview (const juce::String& band)
{
    // Stage 2 implementation:
    //   if active band matches and isPreviewActive, deactivate.
    //   Otherwise set previewBand, reset previewPlayhead, set isPreviewActive.
    DBG ("GrooveScout: togglePreview(" + band + ") called (stub)");
}

//==============================================================================
// Public query methods — stubs for Stage 1
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
    // Stage 2 will return the result published by the analysis thread.
    return 0.0f;
}

juce::String GrooveScoutAudioProcessor::getDetectedKey() const
{
    // Stage 2 will return the key string published by the analysis thread.
    return {};
}

//==============================================================================
// Plugin factory — required by JUCE plugin client
//==============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GrooveScoutAudioProcessor();
}
