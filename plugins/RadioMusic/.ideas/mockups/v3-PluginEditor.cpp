#include "PluginEditor.h"

RadioMusicAudioProcessorEditor::RadioMusicAudioProcessorEditor (RadioMusicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (700, 500);

    // Create WebView
    webView = std::make_unique<juce::WebBrowserComponent>();
    addAndMakeVisible (webView.get());

    // Load HTML from binary resources
    juce::String htmlContent = juce::String::createStringFromData (
        BinaryData::ui_html,
        BinaryData::ui_htmlSize
    );

    webView->goToURL (juce::WebBrowserComponent::Resource (htmlContent, "text/html"));

    // Set up message handler for communication from JavaScript
    webView->pageLoadHadNetworkError = [this] (const juce::String& error)
    {
        DBG ("WebView load error: " + error);
    };

    // Parameter listeners
    audioProcessor.parameters.addParameterListener ("station", this);
    audioProcessor.parameters.addParameterListener ("start", this);
    audioProcessor.parameters.addParameterListener ("speed_pitch", this);
    audioProcessor.parameters.addParameterListener ("volume", this);

    // Start timer for UI updates (60fps)
    startTimerHz (60);
}

RadioMusicAudioProcessorEditor::~RadioMusicAudioProcessorEditor()
{
    audioProcessor.parameters.removeParameterListener ("station", this);
    audioProcessor.parameters.removeParameterListener ("start", this);
    audioProcessor.parameters.removeParameterListener ("speed_pitch", this);
    audioProcessor.parameters.removeParameterListener ("volume", this);
}

void RadioMusicAudioProcessorEditor::paint (juce::Graphics& g)
{
    // WebView handles all rendering
}

void RadioMusicAudioProcessorEditor::resized()
{
    webView->setBounds (getLocalBounds());
}

void RadioMusicAudioProcessorEditor::timerCallback()
{
    // Update playback position
    float playbackPos = audioProcessor.getPlaybackPosition();

    juce::DynamicObject::Ptr message = new juce::DynamicObject();
    message->setProperty ("type", "playbackPosition");
    message->setProperty ("position", playbackPos);

    sendMessageToUI (juce::JSON::toString (message.get()));
}

void RadioMusicAudioProcessorEditor::updateWaveformData()
{
    auto waveformData = audioProcessor.getCurrentWaveformData();

    juce::DynamicObject::Ptr message = new juce::DynamicObject();
    message->setProperty ("type", "fileChanged");
    message->setProperty ("currentFile", audioProcessor.getCurrentFileIndex());
    message->setProperty ("fileName", audioProcessor.getCurrentFileName());

    // Convert waveform data to JSON array
    juce::Array<juce::var> waveformArray;
    for (float sample : waveformData)
        waveformArray.add (sample);

    message->setProperty ("waveformData", waveformArray);

    sendMessageToUI (juce::JSON::toString (message.get()));
}

void RadioMusicAudioProcessorEditor::updateFileInfo()
{
    juce::DynamicObject::Ptr message = new juce::DynamicObject();
    message->setProperty ("type", "folderLoaded");
    message->setProperty ("totalFiles", audioProcessor.getTotalFiles());
    message->setProperty ("currentFile", audioProcessor.getCurrentFileIndex());
    message->setProperty ("fileName", audioProcessor.getCurrentFileName());

    // Include initial waveform data
    auto waveformData = audioProcessor.getCurrentWaveformData();
    juce::Array<juce::var> waveformArray;
    for (float sample : waveformData)
        waveformArray.add (sample);

    message->setProperty ("waveformData", waveformArray);

    sendMessageToUI (juce::JSON::toString (message.get()));
}

void RadioMusicAudioProcessorEditor::sendMessageToUI (const juce::String& message)
{
    // Execute JavaScript to call updateFromBackend
    juce::String script = "window.updateFromBackend(" + message + ");";
    webView->evaluateJavascript (script);
}

// Handle messages from JavaScript
void RadioMusicAudioProcessorEditor::handleMessage (const juce::String& message)
{
    auto json = juce::JSON::parse (message);

    if (auto* obj = json.getDynamicObject())
    {
        juce::String type = obj->getProperty ("type").toString();

        if (type == "parameterChange")
        {
            juce::String param = obj->getProperty ("parameter").toString();
            float value = static_cast<float> (obj->getProperty ("value"));

            // Update processor parameter
            if (auto* parameter = audioProcessor.parameters.getParameter (param))
            {
                parameter->setValueNotifyingHost (
                    parameter->convertTo0to1 (value)
                );
            }
        }
        else if (type == "action")
        {
            juce::String action = obj->getProperty ("action").toString();

            if (action == "browse")
            {
                // Open folder chooser
                fileChooser = std::make_unique<juce::FileChooser> (
                    "Select Audio Folder",
                    juce::File::getSpecialLocation (juce::File::userMusicDirectory),
                    "",
                    true
                );

                fileChooser->launchAsync (
                    juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories,
                    [this] (const juce::FileChooser& chooser)
                    {
                        auto folder = chooser.getResult();
                        if (folder.exists())
                        {
                            audioProcessor.loadFolder (folder);
                            updateFileInfo();
                        }
                    }
                );
            }
        }
    }
}

// Parameter listener callback
void RadioMusicAudioProcessorEditor::parameterChanged (const juce::String& parameterID, float newValue)
{
    juce::DynamicObject::Ptr message = new juce::DynamicObject();
    message->setProperty ("type", "parameterUpdate");
    message->setProperty ("parameter", parameterID);
    message->setProperty ("value", newValue);

    sendMessageToUI (juce::JSON::toString (message.get()));

    // If station changed, update waveform
    if (parameterID == "station")
    {
        updateWaveformData();
    }
}
