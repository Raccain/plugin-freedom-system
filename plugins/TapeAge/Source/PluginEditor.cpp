#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapeAgeAudioProcessorEditor::TapeAgeAudioProcessorEditor (TapeAgeAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    // Set window size
    setSize (500, 450);

    // Configure title label
    titleLabel.setText ("TAPE AGE", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (32.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (titleLabel);

    // Configure stage label
    stageLabel.setText ("Stage 2: Foundation (Temporary UI)", juce::dontSendNotification);
    stageLabel.setFont (juce::Font (14.0f));
    stageLabel.setJustificationType (juce::Justification::centred);
    stageLabel.setColour (juce::Label::textColourId, juce::Colours::grey);
    addAndMakeVisible (stageLabel);

    // Configure DRIVE slider
    driveSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible (driveSlider);
    driveAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "drive", driveSlider));

    driveLabel.setText ("DRIVE", juce::dontSendNotification);
    driveLabel.setJustificationType (juce::Justification::centred);
    driveLabel.attachToComponent (&driveSlider, false);
    addAndMakeVisible (driveLabel);

    // Configure AGE slider
    ageSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    ageSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible (ageSlider);
    ageAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "age", ageSlider));

    ageLabel.setText ("AGE", juce::dontSendNotification);
    ageLabel.setJustificationType (juce::Justification::centred);
    ageLabel.attachToComponent (&ageSlider, false);
    addAndMakeVisible (ageLabel);

    // Configure MIX slider
    mixSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible (mixSlider);
    mixAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "mix", mixSlider));

    mixLabel.setText ("MIX", juce::dontSendNotification);
    mixLabel.setJustificationType (juce::Justification::centred);
    mixLabel.attachToComponent (&mixSlider, false);
    addAndMakeVisible (mixLabel);
}

TapeAgeAudioProcessorEditor::~TapeAgeAudioProcessorEditor()
{
}

//==============================================================================
void TapeAgeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background gradient (vintage tape machine aesthetic)
    g.fillAll (juce::Colour (0xff2a1a0a));

    // Top section for VU meter placeholder
    g.setColour (juce::Colour (0xff1a0a00));
    g.fillRect (50, 80, 400, 80);

    g.setColour (juce::Colours::grey);
    g.drawRect (50, 80, 400, 80, 1);

    g.setFont (12.0f);
    g.drawText ("VU Meter Placeholder (Stage 5)", 50, 80, 400, 80, juce::Justification::centred);
}

void TapeAgeAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    // Title
    titleLabel.setBounds (area.removeFromTop (60));

    // Stage label
    stageLabel.setBounds (area.removeFromTop (20));

    // VU meter placeholder
    area.removeFromTop (100);

    // Sliders section
    auto sliderArea = area.removeFromTop (150);
    auto sliderWidth = sliderArea.getWidth() / 3;

    driveSlider.setBounds (sliderArea.removeFromLeft (sliderWidth).reduced (20));
    ageSlider.setBounds (sliderArea.removeFromLeft (sliderWidth).reduced (20));
    mixSlider.setBounds (sliderArea.removeFromLeft (sliderWidth).reduced (20));
}
