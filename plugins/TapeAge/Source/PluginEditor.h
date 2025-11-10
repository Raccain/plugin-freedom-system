#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 * TapeAge Editor
 *
 * Basic editor placeholder for Stage 2 & 3.
 * Will be replaced with WebView-based UI in Stage 5.
 */
class TapeAgeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TapeAgeAudioProcessorEditor (TapeAgeAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~TapeAgeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TapeAgeAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    // Temporary UI components (Stage 2/3)
    juce::Label titleLabel;
    juce::Label stageLabel;

    // Sliders for testing parameters
    juce::Slider driveSlider;
    juce::Slider ageSlider;
    juce::Slider mixSlider;

    // Labels for sliders
    juce::Label driveLabel;
    juce::Label ageLabel;
    juce::Label mixLabel;

    // Slider attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ageAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeAgeAudioProcessorEditor)
};
