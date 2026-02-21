#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

/**
 * GrooveScout WebView-based Plugin Editor
 *
 * TEMPLATE FILE — for gui-agent reference during Stage 3 (GUI) integration.
 * Adapt class names, includes, and member details to actual plugin structure.
 *
 * =========================================================================
 * CRITICAL: Member declaration order prevents 90% of release build crashes.
 *
 * Destruction order (reverse of declaration):
 *  1. Attachments destroyed FIRST (stop using relays and WebView)
 *  2. WebView destroyed SECOND (safe, attachments are gone)
 *  3. Relays destroyed LAST (safe, nothing using them)
 *
 * See: troubleshooting/patterns/juce8-critical-patterns.md Pattern #11
 * =========================================================================
 *
 * SPECIAL NOTES for GrooveScout:
 *  - Inherits DragAndDropContainer for MIDI clip performExternalDragDropOfFiles
 *  - Inherits juce::Timer for polling analysis state (~60ms interval)
 *  - Native functions registered in constructor (non-APVTS actions)
 *  - 15 APVTS parameters total: 9 Float + 1 Choice + 5 Bool
 *
 * See: troubleshooting/patterns/juce8-critical-patterns.md
 *       Patterns #8, #11, #12, #13, #21
 */

class GrooveScoutAudioProcessorEditor
    : public juce::AudioProcessorEditor
    , public juce::DragAndDropContainer
    , private juce::Timer
{
public:
    GrooveScoutAudioProcessorEditor(GrooveScoutAudioProcessor& p);
    ~GrooveScoutAudioProcessorEditor() override;

    // AudioProcessorEditor overrides
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // =========================================================================
    // Timer callback — polls analysis state, updates UI via evaluateJavascript
    // 60ms interval (~16fps update rate for progress bar)
    // =========================================================================
    void timerCallback() override;

    // =========================================================================
    // Resource provider (JUCE 8 required pattern)
    // Pattern #8: Explicit URL mapping — ALWAYS REQUIRED
    // Maps request URLs to BinaryData symbols.
    // =========================================================================
    std::optional<juce::WebBrowserComponent::Resource> getResource(
        const juce::String& url
    );

    // =========================================================================
    // Reference to audio processor
    // =========================================================================
    GrooveScoutAudioProcessor& audioProcessor;

    // =========================================================================
    // ⚠️  CRITICAL MEMBER DECLARATION ORDER  ⚠️
    //
    //  Order: Relays  →  WebView  →  Attachments
    //
    //  Members are destroyed in REVERSE order of declaration.
    //  - Attachments must be destroyed BEFORE WebView (they call evaluateJavascript)
    //  - WebView must be destroyed BEFORE Relays (holds relay refs in Options)
    //
    //  DO NOT REORDER without understanding the destructor sequence!
    //  See: juce8-critical-patterns.md #11
    // =========================================================================

    // -------------------------------------------------------------------------
    // 1. RELAYS FIRST  (created first, destroyed last — no dependencies)
    // -------------------------------------------------------------------------
    //
    // 9 Float sliders from v2-ui.yaml:
    //   kickSensitivity   0.0 – 1.0 linear
    //   kickFreqLow       20 – 1000 Hz  logarithmic
    //   kickFreqHigh      20 – 1000 Hz  logarithmic
    //   snareSensitivity  0.0 – 1.0 linear
    //   snareFreqLow      100 – 20000 Hz logarithmic
    //   snareFreqHigh     100 – 20000 Hz logarithmic
    //   hihatSensitivity  0.0 – 1.0 linear
    //   hihatFreqLow      1000 – 20000 Hz logarithmic
    //   hihatFreqHigh     1000 – 20000 Hz logarithmic
    //
    std::unique_ptr<juce::WebSliderRelay> kickSensitivityRelay;
    std::unique_ptr<juce::WebSliderRelay> kickFreqLowRelay;
    std::unique_ptr<juce::WebSliderRelay> kickFreqHighRelay;
    std::unique_ptr<juce::WebSliderRelay> snareSensitivityRelay;
    std::unique_ptr<juce::WebSliderRelay> snareFreqLowRelay;
    std::unique_ptr<juce::WebSliderRelay> snareFreqHighRelay;
    std::unique_ptr<juce::WebSliderRelay> hihatSensitivityRelay;
    std::unique_ptr<juce::WebSliderRelay> hihatFreqLowRelay;
    std::unique_ptr<juce::WebSliderRelay> hihatFreqHighRelay;

    // 1 Choice (combo) from v2-ui.yaml:
    //   bpmMultiplier  options: ["1/2x", "1x", "2x"], default index 1
    //
    std::unique_ptr<juce::WebComboBoxRelay> bpmMultiplierRelay;

    // 5 Bool toggles from v2-ui.yaml:
    //   analyzeBPM    default true
    //   analyzeKey    default true
    //   analyzeKick   default true
    //   analyzeSnare  default true
    //   analyzeHihat  default true
    //
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeBPMRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeKeyRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeKickRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeSnareRelay;
    std::unique_ptr<juce::WebToggleButtonRelay> analyzeHihatRelay;

    // -------------------------------------------------------------------------
    // 2. WEBVIEW SECOND  (created after relays, destroyed before relays)
    // -------------------------------------------------------------------------
    std::unique_ptr<juce::WebBrowserComponent> webView;

    // -------------------------------------------------------------------------
    // 3. PARAMETER ATTACHMENTS LAST  (created last, destroyed first)
    //    Synchronize APVTS parameters with relay state.
    //    MUST be declared AFTER WebView.
    //
    //    Pattern #12: JUCE 8 requires 3 parameters (parameter, relay, undoManager)
    // -------------------------------------------------------------------------

    // Float slider attachments (9)
    std::unique_ptr<juce::WebSliderParameterAttachment> kickSensitivityAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> kickFreqLowAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> kickFreqHighAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> snareSensitivityAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> snareFreqLowAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> snareFreqHighAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> hihatSensitivityAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> hihatFreqLowAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> hihatFreqHighAttachment;

    // Choice attachment (1)
    std::unique_ptr<juce::WebComboBoxParameterAttachment> bpmMultiplierAttachment;

    // Bool toggle attachments (5)
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeBPMAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeKeyAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeKickAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeSnareAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> analyzeHihatAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrooveScoutAudioProcessorEditor)
};
