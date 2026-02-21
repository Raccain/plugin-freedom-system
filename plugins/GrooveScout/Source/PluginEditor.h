#pragma once

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

//==============================================================================
/**
 * GrooveScoutAudioProcessorEditor — Stage 1 stub editor
 *
 * Fixed size: 750 x 560 pixels (matches v5 UI mockup dimensions).
 * Dark background: #0e0e12.
 *
 * Stage 3 (GUI) will replace this with a WebView-based editor that renders
 * the full HTML/CSS/JS interface from the finalized v5 mockup.
 * The WebBrowserComponent, relay objects, and native function bridge are
 * set up by the gui-agent in Stage 3.
 *
 * Include <juce_gui_extra/juce_gui_extra.h> is kept here so the header is
 * already available when the gui-agent adds WebBrowserComponent members.
 */
class GrooveScoutAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit GrooveScoutAudioProcessorEditor (GrooveScoutAudioProcessor&);
    ~GrooveScoutAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    GrooveScoutAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrooveScoutAudioProcessorEditor)
};
