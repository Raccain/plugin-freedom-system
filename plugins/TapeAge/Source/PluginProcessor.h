#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * TapeAge - Vintage Tape Saturator
 *
 * A warm tape saturation plugin with musical degradation effects including
 * wow, flutter, dropout, and tape noise.
 */
class TapeAgeAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    TapeAgeAudioProcessor();
    ~TapeAgeAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    // Parameter layout creation
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Audio parameter value tree state
    juce::AudioProcessorValueTreeState parameters;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeAgeAudioProcessor)
};
