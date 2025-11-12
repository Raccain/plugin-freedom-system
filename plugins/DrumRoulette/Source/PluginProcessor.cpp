#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout DrumRouletteAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Global parameter: RANDOMIZE_ALL (1 parameter)
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID { "RANDOMIZE_ALL", 1 },
        "Randomize All",
        false
    ));

    // Per-slot parameters (9 × 8 = 72 parameters)
    for (int slot = 1; slot <= 8; ++slot)
    {
        juce::String slotNum = juce::String(slot);

        // RANDOMIZE_N - Button trigger
        layout.add(std::make_unique<juce::AudioParameterBool>(
            juce::ParameterID { "RANDOMIZE_" + slotNum, 1 },
            "Randomize " + slotNum,
            false
        ));

        // LOCK_N - Toggle
        layout.add(std::make_unique<juce::AudioParameterBool>(
            juce::ParameterID { "LOCK_" + slotNum, 1 },
            "Lock " + slotNum,
            false
        ));

        // VOLUME_N - Fader (-60.0 to +6.0 dB, skew 2.0)
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID { "VOLUME_" + slotNum, 1 },
            "Volume " + slotNum,
            juce::NormalisableRange<float>(-60.0f, 6.0f, 0.1f, 2.0f),
            0.0f,
            "dB"
        ));

        // DECAY_N - Rotary (10.0 to 2000.0 ms, skew 0.5)
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID { "DECAY_" + slotNum, 1 },
            "Decay " + slotNum,
            juce::NormalisableRange<float>(10.0f, 2000.0f, 0.1f, 0.5f),
            500.0f,
            "ms"
        ));

        // ATTACK_N - Rotary (0.0 to 50.0 ms, skew 0.5)
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID { "ATTACK_" + slotNum, 1 },
            "Attack " + slotNum,
            juce::NormalisableRange<float>(0.0f, 50.0f, 0.1f, 0.5f),
            1.0f,
            "ms"
        ));

        // TILT_FILTER_N - Rotary (-12.0 to +12.0 dB, linear)
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID { "TILT_FILTER_" + slotNum, 1 },
            "Tilt " + slotNum,
            juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f, 1.0f),
            0.0f,
            "dB"
        ));

        // PITCH_N - Rotary (-12.0 to +12.0 semitones, linear)
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID { "PITCH_" + slotNum, 1 },
            "Pitch " + slotNum,
            juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f, 1.0f),
            0.0f,
            "st"
        ));

        // SOLO_N - Toggle
        layout.add(std::make_unique<juce::AudioParameterBool>(
            juce::ParameterID { "SOLO_" + slotNum, 1 },
            "Solo " + slotNum,
            false
        ));

        // MUTE_N - Toggle
        layout.add(std::make_unique<juce::AudioParameterBool>(
            juce::ParameterID { "MUTE_" + slotNum, 1 },
            "Mute " + slotNum,
            false
        ));
    }

    return layout;
}

DrumRouletteAudioProcessor::DrumRouletteAudioProcessor()
    : AudioProcessor(BusesProperties()
                        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

DrumRouletteAudioProcessor::~DrumRouletteAudioProcessor()
{
}

void DrumRouletteAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialization will be added in Stage 4
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void DrumRouletteAudioProcessor::releaseResources()
{
    // Cleanup will be added in Stage 4
}

void DrumRouletteAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);

    // Pass-through for Stage 3 (DSP implementation happens in Stage 4)
    // Instrument needs to clear output buffer (no audio input to pass through)
    buffer.clear();

    // Parameter access example (for Stage 4 DSP implementation):
    // auto* volumeParam = parameters.getRawParameterValue("VOLUME_1");
    // float volumeValue = volumeParam->load();  // Atomic read (real-time safe)
    //
    // auto* lockParam = parameters.getRawParameterValue("LOCK_1");
    // bool isLocked = lockParam->load() >= 0.5f;
}

juce::AudioProcessorEditor* DrumRouletteAudioProcessor::createEditor()
{
    return new DrumRouletteAudioProcessorEditor(*this);
}

void DrumRouletteAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DrumRouletteAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// Factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DrumRouletteAudioProcessor();
}
