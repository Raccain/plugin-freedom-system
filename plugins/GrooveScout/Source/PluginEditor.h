#pragma once

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

//==============================================================================
/**
 * GrooveScoutAudioProcessorEditor — Stage 3 WebView-based editor
 *
 * Fixed size: 750 x 560 pixels (matches v2 UI mockup dimensions).
 *
 * WebView integration pattern (JUCE 8):
 *   - Relays declared FIRST (no dependencies)
 *   - WebView declared SECOND (depends on relays via .withOptionsFrom())
 *   - Attachments declared LAST (depend on both relays and WebView)
 *
 * Destruction is reverse of declaration order:
 *   1. Attachments destroyed FIRST  → stop using relays/WebView
 *   2. WebView destroyed SECOND     → safe, attachments are gone
 *   3. Relays destroyed LAST        → safe, nothing is using them
 *
 * Timer polls processor atomics every ~100 ms and pushes analysis state
 * to the WebView via evaluateJavascript().
 */
class GrooveScoutAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::DragAndDropContainer,
                                        public juce::Timer
{
public:
    explicit GrooveScoutAudioProcessorEditor (GrooveScoutAudioProcessor&);
    ~GrooveScoutAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // Timer callback — polls processor and updates WebView state
    void timerCallback() override;

private:
    GrooveScoutAudioProcessor& processorRef;

    // =========================================================================
    // CRITICAL MEMBER DECLARATION ORDER
    // Members are destroyed in REVERSE order of declaration.
    // Relays → WebView → Attachments  (DO NOT REORDER)
    // =========================================================================

    // -------------------------------------------------------------------------
    // 1. RELAYS FIRST (created first, destroyed last)
    // -------------------------------------------------------------------------

    // Float sliders (9 parameters)
    std::unique_ptr<juce::WebSliderRelay> kickSensitivityRelay;
    std::unique_ptr<juce::WebSliderRelay> kickFreqLowRelay;
    std::unique_ptr<juce::WebSliderRelay> kickFreqHighRelay;
    std::unique_ptr<juce::WebSliderRelay> snareSensitivityRelay;
    std::unique_ptr<juce::WebSliderRelay> snareFreqLowRelay;
    std::unique_ptr<juce::WebSliderRelay> snareFreqHighRelay;
    std::unique_ptr<juce::WebSliderRelay> hihatSensitivityRelay;
    std::unique_ptr<juce::WebSliderRelay> hihatFreqLowRelay;
    std::unique_ptr<juce::WebSliderRelay> hihatFreqHighRelay;

    // Choice combo (1 parameter)
    std::unique_ptr<juce::WebComboBoxRelay> bpmMultiplierRelay;

    // Bool toggles (5 parameters)
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeBPMRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeKeyRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeKickRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeSnareRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeHihatRelay;

    // -------------------------------------------------------------------------
    // 2. WEBVIEW SECOND (depends on relays via .withOptionsFrom())
    // -------------------------------------------------------------------------
    std::unique_ptr<juce::WebBrowserComponent> webView;

    // -------------------------------------------------------------------------
    // 3. ATTACHMENTS LAST (created last, destroyed first)
    // -------------------------------------------------------------------------

    // Float slider attachments
    std::unique_ptr<juce::WebSliderParameterAttachment> kickSensitivityAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> kickFreqLowAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> kickFreqHighAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> snareSensitivityAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> snareFreqLowAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> snareFreqHighAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> hihatSensitivityAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> hihatFreqLowAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> hihatFreqHighAttachment;

    // Choice combo attachment
    std::unique_ptr<juce::WebComboBoxParameterAttachment> bpmMultiplierAttachment;

    // Bool toggle attachments
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeBPMAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeKeyAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeKickAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeSnareAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeHihatAttachment;

    // -------------------------------------------------------------------------
    // Resource provider helper
    // -------------------------------------------------------------------------
    std::optional<juce::WebBrowserComponent::Resource> getResource (const juce::String& url);

    // Last sample count for which waveform data was sent to JS
    // Used to avoid redundant waveform updates when nothing has changed
    int lastSentWaveformSamples = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrooveScoutAudioProcessorEditor)
};
