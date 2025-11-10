#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class RadioMusicAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       private juce::Timer
{
public:
    RadioMusicAudioProcessorEditor (RadioMusicAudioProcessor&);
    ~RadioMusicAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void updateWaveformData();
    void updateFileInfo();
    void sendMessageToUI (const juce::String& message);

    RadioMusicAudioProcessor& audioProcessor;

    // WebView component
    std::unique_ptr<juce::WebBrowserComponent> webView;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stationAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> startAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> speedPitchAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    // File browser
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadioMusicAudioProcessorEditor)
};
