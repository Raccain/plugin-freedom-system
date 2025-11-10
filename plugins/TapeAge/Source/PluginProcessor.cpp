#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapeAgeAudioProcessor::TapeAgeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
      parameters (*this, nullptr, juce::Identifier ("TapeAge"), createParameterLayout())
{
}

TapeAgeAudioProcessor::~TapeAgeAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout TapeAgeAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // DRIVE parameter (0-100%, default 50%)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "drive",
        "Drive",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(static_cast<int>(value * 100)) + "%"; }
    ));

    // AGE parameter (0-100%, default 25%)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "age",
        "Age",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.25f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(static_cast<int>(value * 100)) + "%"; }
    ));

    // MIX parameter (0-100%, default 100%)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "mix",
        "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        1.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(static_cast<int>(value * 100)) + "%"; }
    ));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String TapeAgeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TapeAgeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TapeAgeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TapeAgeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TapeAgeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TapeAgeAudioProcessor::getNumPrograms()
{
    return 1;
}

int TapeAgeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TapeAgeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TapeAgeAudioProcessor::getProgramName (int index)
{
    return {};
}

void TapeAgeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TapeAgeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize DSP components here
}

void TapeAgeAudioProcessor::releaseResources()
{
    // Release DSP resources here
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TapeAgeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This plugin supports stereo in/out
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    #if ! JucePlugin_IsSynth
     if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
         return false;
    #endif

    return true;
  #endif
}
#endif

void TapeAgeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get parameter values
    auto drive = parameters.getRawParameterValue("drive")->load();
    auto age = parameters.getRawParameterValue("age")->load();
    auto mix = parameters.getRawParameterValue("mix")->load();

    // TODO: Implement DSP processing in Stage 4
    // For now, this is a pass-through (Stage 3 shell)
}

//==============================================================================
bool TapeAgeAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TapeAgeAudioProcessor::createEditor()
{
    return new TapeAgeAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void TapeAgeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TapeAgeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapeAgeAudioProcessor();
}
