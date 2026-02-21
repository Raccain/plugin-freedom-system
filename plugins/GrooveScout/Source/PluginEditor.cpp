#include "PluginEditor.h"

//==============================================================================
GrooveScoutAudioProcessorEditor::GrooveScoutAudioProcessorEditor (
    GrooveScoutAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Fixed size: 750 x 560 pixels — matches the v5 UI mockup.
    // Stage 3 (gui-agent) will call setResizable() if needed, or keep fixed.
    setSize (750, 560);
}

GrooveScoutAudioProcessorEditor::~GrooveScoutAudioProcessorEditor()
{
}

//==============================================================================
void GrooveScoutAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Dark background matching v5 mockup colour #0e0e12
    g.fillAll (juce::Colour (0xff0e0e12));

    // Stage 1 placeholder text
    g.setColour (juce::Colours::white.withAlpha (0.7f));
    g.setFont (juce::Font (22.0f, juce::Font::bold));
    g.drawFittedText ("GrooveScout",
                      getLocalBounds().reduced (40).removeFromTop (80),
                      juce::Justification::centredBottom,
                      1);

    g.setFont (juce::Font (13.0f));
    g.setColour (juce::Colours::white.withAlpha (0.4f));
    g.drawFittedText ("Stage 1 — 10 parameters implemented\nWebView UI added in Stage 3",
                      getLocalBounds().reduced (40).withTrimmedTop (160),
                      juce::Justification::centredTop,
                      3);
}

void GrooveScoutAudioProcessorEditor::resized()
{
    // No child components in Stage 1.
    // Stage 3 (gui-agent) will lay out the WebBrowserComponent here:
    //   webView->setBounds (getLocalBounds());
}
