#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout MinimalKickAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // sweep - Pitch envelope amount (0.0 to 24.0 semitones, linear)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "sweep", 1 },
        "Sweep",
        juce::NormalisableRange<float>(0.0f, 24.0f, 0.1f),
        12.0f,
        "st"
    ));

    // time - Pitch envelope decay time (5.0 to 500.0 ms, logarithmic)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "time", 1 },
        "Time",
        juce::NormalisableRange<float>(5.0f, 500.0f, 0.1f, 0.3f),
        50.0f,
        "ms"
    ));

    // attack - Amplitude envelope attack time (0.0 to 50.0 ms, linear)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "attack", 1 },
        "Attack",
        juce::NormalisableRange<float>(0.0f, 50.0f, 0.1f),
        5.0f,
        "ms"
    ));

    // decay - Amplitude envelope decay time (50.0 to 2000.0 ms, logarithmic)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "decay", 1 },
        "Decay",
        juce::NormalisableRange<float>(50.0f, 2000.0f, 1.0f, 0.3f),
        400.0f,
        "ms"
    ));

    // drive - Saturation/distortion amount (0.0 to 100.0%, linear)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "drive", 1 },
        "Drive",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
        20.0f,
        "%"
    ));

    return layout;
}

MinimalKickAudioProcessor::MinimalKickAudioProcessor()
    : AudioProcessor(BusesProperties()
                        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

MinimalKickAudioProcessor::~MinimalKickAudioProcessor()
{
}

void MinimalKickAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialization will be added in Stage 4
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void MinimalKickAudioProcessor::releaseResources()
{
    // Cleanup will be added in Stage 4
}

void MinimalKickAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);

    // Parameter access example (for Stage 4 DSP implementation):
    // auto* sweepParam = parameters.getRawParameterValue("sweep");
    // auto* timeParam = parameters.getRawParameterValue("time");
    // auto* attackParam = parameters.getRawParameterValue("attack");
    // auto* decayParam = parameters.getRawParameterValue("decay");
    // auto* driveParam = parameters.getRawParameterValue("drive");
    //
    // float sweepValue = sweepParam->load();  // Atomic read (real-time safe)
    // float timeValue = timeParam->load();
    // float attackValue = attackParam->load();
    // float decayValue = decayParam->load();
    // float driveValue = driveParam->load();

    // Pass-through for Stage 3 (DSP implementation happens in Stage 4)
    // Instrument outputs silence until DSP implemented
    buffer.clear();
}

juce::AudioProcessorEditor* MinimalKickAudioProcessor::createEditor()
{
    return new MinimalKickAudioProcessorEditor(*this);
}

void MinimalKickAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MinimalKickAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// Factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MinimalKickAudioProcessor();
}
