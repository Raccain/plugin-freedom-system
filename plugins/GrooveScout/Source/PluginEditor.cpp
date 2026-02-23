#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
// Constructor — CRITICAL: Initialize in correct order
//   1. Create relays FIRST
//   2. Create WebView with relay options
//   3. Create attachments LAST
//==============================================================================

GrooveScoutAudioProcessorEditor::GrooveScoutAudioProcessorEditor (GrooveScoutAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // =========================================================================
    // STEP 1: CREATE RELAYS (before WebView!)
    // Relay names MUST match APVTS parameter IDs exactly (case-sensitive)
    // =========================================================================

    // Float slider relays
    kickSensitivityRelay  = std::make_unique<juce::WebSliderRelay> ("kickSensitivity");
    kickFreqLowRelay      = std::make_unique<juce::WebSliderRelay> ("kickFreqLow");
    kickFreqHighRelay     = std::make_unique<juce::WebSliderRelay> ("kickFreqHigh");
    snareSensitivityRelay = std::make_unique<juce::WebSliderRelay> ("snareSensitivity");
    snareFreqLowRelay     = std::make_unique<juce::WebSliderRelay> ("snareFreqLow");
    snareFreqHighRelay    = std::make_unique<juce::WebSliderRelay> ("snareFreqHigh");
    hihatSensitivityRelay = std::make_unique<juce::WebSliderRelay> ("hihatSensitivity");
    hihatFreqLowRelay     = std::make_unique<juce::WebSliderRelay> ("hihatFreqLow");
    hihatFreqHighRelay    = std::make_unique<juce::WebSliderRelay> ("hihatFreqHigh");

    // Choice combo relay
    bpmMultiplierRelay = std::make_unique<juce::WebComboBoxRelay> ("bpmMultiplier");

    // Bool toggle relays
    analyzeBPMRelay   = std::make_unique<juce::WebToggleButtonRelay> ("analyzeBPM");
    analyzeKeyRelay   = std::make_unique<juce::WebToggleButtonRelay> ("analyzeKey");
    analyzeKickRelay  = std::make_unique<juce::WebToggleButtonRelay> ("analyzeKick");
    analyzeSnareRelay = std::make_unique<juce::WebToggleButtonRelay> ("analyzeSnare");
    analyzeHihatRelay = std::make_unique<juce::WebToggleButtonRelay> ("analyzeHihat");

    // =========================================================================
    // STEP 2: CREATE WEBVIEW (after relays, before attachments)
    // =========================================================================
    webView = std::make_unique<juce::WebBrowserComponent> (
        juce::WebBrowserComponent::Options {}
            // REQUIRED: Enable JUCE frontend JavaScript library
            .withNativeIntegrationEnabled()

            // REQUIRED: Serve embedded HTML/JS/CSS files
            .withResourceProvider ([this] (const auto& url) { return getResource (url); })

            // OPTIONAL: FL Studio fix (prevents blank screen on focus loss)
            .withKeepPageLoadedWhenBrowserIsHidden()

            // REQUIRED: Register all relays so JS can find them
            // Float sliders
            .withOptionsFrom (*kickSensitivityRelay)
            .withOptionsFrom (*kickFreqLowRelay)
            .withOptionsFrom (*kickFreqHighRelay)
            .withOptionsFrom (*snareSensitivityRelay)
            .withOptionsFrom (*snareFreqLowRelay)
            .withOptionsFrom (*snareFreqHighRelay)
            .withOptionsFrom (*hihatSensitivityRelay)
            .withOptionsFrom (*hihatFreqLowRelay)
            .withOptionsFrom (*hihatFreqHighRelay)
            // Choice combo
            .withOptionsFrom (*bpmMultiplierRelay)
            // Bool toggles
            .withOptionsFrom (*analyzeBPMRelay)
            .withOptionsFrom (*analyzeKeyRelay)
            .withOptionsFrom (*analyzeKickRelay)
            .withOptionsFrom (*analyzeSnareRelay)
            .withOptionsFrom (*analyzeHihatRelay)

            // Native functions called from JavaScript via getNativeFunction('name')
            // Called on the message thread — safe to call processor methods directly

            // REC / STOP — wiring that was missing from Stage 3 GUI.1
            .withNativeFunction (
                juce::Identifier ("setCaptureDuration"),
                [this] (const juce::Array<juce::var>& args, auto complete)
                {
                    // args[0] = duration in seconds (1–30)
                    if (args.size() > 0)
                    {
                        const float secs = juce::jlimit (1.0f, 30.0f,
                            static_cast<float> (static_cast<double> (args[0])));

                        if (auto* p = dynamic_cast<juce::AudioParameterFloat*> (
                                processorRef.parameters.getParameter ("captureDuration")))
                        {
                            *p = secs;
                            DBG ("GrooveScout: setCaptureDuration(" + juce::String (secs, 1) + ")");
                        }
                    }
                    complete (juce::var {});
                })

            .withNativeFunction (
                juce::Identifier ("startRecording"),
                [this] (const juce::Array<juce::var>&, auto complete)
                {
                    processorRef.startRecording();
                    complete (juce::var {});
                })

            .withNativeFunction (
                juce::Identifier ("stopCurrentOperation"),
                [this] (const juce::Array<juce::var>&, auto complete)
                {
                    processorRef.stopCurrentOperation();
                    complete (juce::var {});
                })

            // Audition preview: separate start/stop so switching bands works cleanly
            .withNativeFunction (
                juce::Identifier ("startPreview"),
                [this] (const juce::Array<juce::var>& args, auto complete)
                {
                    juce::String band;
                    if (args.size() > 0)
                        band = args[0].toString();

                    int bandIndex = 0;
                    if (band == "snare") bandIndex = 1;
                    else if (band == "hihat") bandIndex = 2;

                    // Stop any existing preview, then start new band
                    processorRef.isPreviewActive.store (false);
                    processorRef.previewBand.store (bandIndex);
                    processorRef.previewPlayhead.store (0);
                    processorRef.isPreviewActive.store (true);
                    DBG ("GrooveScout: startPreview(" + band + ")");
                    complete (juce::var {});
                })

            .withNativeFunction (
                juce::Identifier ("stopPreview"),
                [this] (const juce::Array<juce::var>&, auto complete)
                {
                    processorRef.isPreviewActive.store (false);
                    processorRef.previewPlayhead.store (0);
                    DBG ("GrooveScout: stopPreview()");
                    complete (juce::var {});
                })

            .withNativeFunction (
                juce::Identifier ("analyzeButtonPressed"),
                [this] (const juce::Array<juce::var>&, auto complete)
                {
                    processorRef.startAnalysis();
                    complete (juce::var {});
                })

            .withNativeFunction (
                juce::Identifier ("cancelAnalysis"),
                [this] (const juce::Array<juce::var>&, auto complete)
                {
                    processorRef.cancelAnalysis();
                    complete (juce::var {});
                })

            .withNativeFunction (
                juce::Identifier ("setBpmToDaw"),
                [this] (const juce::Array<juce::var>&, auto complete)
                {
                    // JUCE does not expose a direct "set DAW tempo" API from plugins.
                    // This is a stub — host-specific implementation goes here if needed.
                    juce::ignoreUnused (this);
                    DBG ("GrooveScout: setBpmToDaw() called — stub");
                    complete (juce::var {});
                })

            .withNativeFunction (
                juce::Identifier ("startMidiDrag"),
                [this] (const juce::Array<juce::var>& args, auto complete)
                {
                    // args[0] = clip type: "chord" / "kick" / "snare" / "hihat"
                    juce::String clipType;
                    if (args.size() > 0)
                        clipType = args[0].toString();

                    // Resolve temp MIDI file path and clip-available flag
                    const juce::File tempDir = juce::File::getSpecialLocation (
                        juce::File::SpecialLocationType::tempDirectory).getChildFile ("GrooveScout");

                    juce::File  midiFile;
                    bool        clipAvailable = false;

                    if (clipType == "chord")
                    {
                        midiFile      = tempDir.getChildFile ("groovescout_chord.mid");
                        clipAvailable = processorRef.chordClipAvailable.load();
                    }
                    else if (clipType == "kick")
                    {
                        midiFile      = tempDir.getChildFile ("groovescout_kick.mid");
                        clipAvailable = processorRef.kickClipAvailable.load();
                    }
                    else if (clipType == "snare")
                    {
                        midiFile      = tempDir.getChildFile ("groovescout_snare.mid");
                        clipAvailable = processorRef.snareClipAvailable.load();
                    }
                    else if (clipType == "hihat")
                    {
                        midiFile      = tempDir.getChildFile ("groovescout_hihat.mid");
                        clipAvailable = processorRef.hihatClipAvailable.load();
                    }
                    else
                    {
                        DBG ("GrooveScout: startMidiDrag — unknown clip type: " + clipType);
                        complete (juce::var {});
                        return;
                    }

                    if (clipAvailable && midiFile.existsAsFile())
                    {
                        DBG ("GrooveScout: startMidiDrag — dragging " + midiFile.getFullPathName());
                        // Pass 'this' as source component so JUCE can find the correct
                        // NSView for the OS-level drag operation on macOS.
                        juce::DragAndDropContainer::performExternalDragDropOfFiles (
                            juce::StringArray { midiFile.getFullPathName() },
                            false /* canMoveFiles */,
                            this);
                    }
                    else
                    {
                        DBG ("GrooveScout: startMidiDrag — clip not ready: " + clipType);
                    }

                    complete (juce::var {});
                })
    );

    // =========================================================================
    // STEP 3: CREATE PARAMETER ATTACHMENTS (after WebView!)
    // Pattern #12: WebSliderParameterAttachment requires 3 params: (parameter, relay, nullptr)
    // =========================================================================

    // Float slider attachments
    kickSensitivityAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("kickSensitivity"),
        *kickSensitivityRelay,
        nullptr);

    kickFreqLowAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("kickFreqLow"),
        *kickFreqLowRelay,
        nullptr);

    kickFreqHighAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("kickFreqHigh"),
        *kickFreqHighRelay,
        nullptr);

    snareSensitivityAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("snareSensitivity"),
        *snareSensitivityRelay,
        nullptr);

    snareFreqLowAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("snareFreqLow"),
        *snareFreqLowRelay,
        nullptr);

    snareFreqHighAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("snareFreqHigh"),
        *snareFreqHighRelay,
        nullptr);

    hihatSensitivityAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("hihatSensitivity"),
        *hihatSensitivityRelay,
        nullptr);

    hihatFreqLowAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("hihatFreqLow"),
        *hihatFreqLowRelay,
        nullptr);

    hihatFreqHighAttachment = std::make_unique<juce::WebSliderParameterAttachment> (
        *processorRef.parameters.getParameter ("hihatFreqHigh"),
        *hihatFreqHighRelay,
        nullptr);

    // Choice combo attachment
    bpmMultiplierAttachment = std::make_unique<juce::WebComboBoxParameterAttachment> (
        *processorRef.parameters.getParameter ("bpmMultiplier"),
        *bpmMultiplierRelay,
        nullptr);

    // Bool toggle attachments
    analyzeBPMAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment> (
        *processorRef.parameters.getParameter ("analyzeBPM"),
        *analyzeBPMRelay,
        nullptr);

    analyzeKeyAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment> (
        *processorRef.parameters.getParameter ("analyzeKey"),
        *analyzeKeyRelay,
        nullptr);

    analyzeKickAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment> (
        *processorRef.parameters.getParameter ("analyzeKick"),
        *analyzeKickRelay,
        nullptr);

    analyzeSnareAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment> (
        *processorRef.parameters.getParameter ("analyzeSnare"),
        *analyzeSnareRelay,
        nullptr);

    analyzeHihatAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment> (
        *processorRef.parameters.getParameter ("analyzeHihat"),
        *analyzeHihatRelay,
        nullptr);

    // =========================================================================
    // WEBVIEW SETUP
    // =========================================================================

    // Navigate to root — triggers resource provider to serve index.html
    webView->goToURL (juce::WebBrowserComponent::getResourceProviderRoot());

    // Make WebView visible and fill the editor
    addAndMakeVisible (*webView);

    // =========================================================================
    // WINDOW SIZE & TIMER
    // =========================================================================

    // Fixed size: 750 x 560 px — matches the v2 UI mockup
    setSize (750, 560);
    setResizable (false, false);

    // Poll processor atomics at ~10 Hz to update analysis state in WebView
    startTimerHz (10);
}

//==============================================================================
// Destructor
//==============================================================================

GrooveScoutAudioProcessorEditor::~GrooveScoutAudioProcessorEditor()
{
    stopTimer();

    // Members automatically destroyed in reverse order of declaration:
    //   1. All attachments (stop sending evaluateJavascript to WebView)
    //   2. webView (safe, attachments are gone)
    //   3. All relays (safe, nothing using them)
}

//==============================================================================
// AudioProcessorEditor overrides
//==============================================================================

void GrooveScoutAudioProcessorEditor::paint (juce::Graphics& g)
{
    // WebView fills the entire editor — no custom JUCE painting needed
    juce::ignoreUnused (g);
}

void GrooveScoutAudioProcessorEditor::resized()
{
    if (webView != nullptr)
        webView->setBounds (getLocalBounds());
}

//==============================================================================
// Timer callback — polls processor state and pushes to WebView
//==============================================================================

void GrooveScoutAudioProcessorEditor::timerCallback()
{
    if (webView == nullptr)
        return;

    // Pre-compute recording state used in both JSON payloads below
    const int   nRecordedSamples = processorRef.recordedSamples.load();
    const float recSecs          = static_cast<float> (nRecordedSamples)
                                   / static_cast<float> (processorRef.currentSampleRate);
    const bool  recComplete      = processorRef.recordingComplete.load();

    // Read current analysis state from processor atomics (message-thread safe)
    const bool   isAnalyzing  = processorRef.analyzeTriggered.load()
                                && !processorRef.analysisComplete.load()
                                && !processorRef.analysisCancelled.load();
    const bool   isComplete   = processorRef.analysisComplete.load();
    const float  progress     = static_cast<float> (processorRef.analysisProgress.load()) / 100.0f;
    const float  bpm          = isComplete ? processorRef.detectedBpm : 0.0f;
    const juce::String key    = isComplete ? processorRef.detectedKey : juce::String();

    // Build a JSON payload for the JS function groovescout_updateAnalysisState({...})
    juce::String json = "{";
    json += "\"isAnalyzing\":" + juce::String (isAnalyzing ? "true" : "false") + ",";
    json += "\"isComplete\":"  + juce::String (isComplete  ? "true" : "false") + ",";
    json += "\"progress\":"    + juce::String (progress, 3) + ",";

    if (isComplete && bpm > 0.0f)
        json += "\"bpm\":"  + juce::String (bpm, 1) + ",";
    else
        json += "\"bpm\":null,";

    if (isComplete && key.isNotEmpty())
        json += "\"key\":\"" + key.replace ("\"", "\\\"") + "\",";
    else
        json += "\"key\":null,";

    // Clip availability — JS uses these to make only real tiles draggable
    json += "\"kickAvail\":"  + juce::String (processorRef.kickClipAvailable.load()  ? "true" : "false") + ",";
    json += "\"snareAvail\":" + juce::String (processorRef.snareClipAvailable.load() ? "true" : "false") + ",";
    json += "\"hihatAvail\":" + juce::String (processorRef.hihatClipAvailable.load() ? "true" : "false") + ",";
    json += "\"chordAvail\":" + juce::String (processorRef.chordClipAvailable.load() ? "true" : "false") + ",";

    // Recording state — JS uses these to auto-stop recording UI and show duration
    json += "\"recordingComplete\":" + juce::String (recComplete ? "true" : "false") + ",";
    json += "\"recordedSecs\":"      + juce::String (recSecs, 1);

    json += "}";

    // Call the JS function defined in index.html:
    //   window.groovescout_updateAnalysisState({ ... })
    const juce::String script = "if(window.groovescout_updateAnalysisState){window.groovescout_updateAnalysisState("
                                + json + ");}";

    webView->evaluateJavascript (script, [] (juce::WebBrowserComponent::EvaluationResult) {});

    // -------------------------------------------------------------------------
    // Waveform data push — sends real RMS peak data + time-based fill fraction
    // to JS so the waveform canvas shows recording progress even for silence.
    // Sent whenever recordedSamples changes (recording in progress or complete).
    // -------------------------------------------------------------------------
    const int nSamples = nRecordedSamples;  // use pre-computed value from above

    if (nSamples > 0 && (processorRef.waveformDirty.load() || nSamples != lastSentWaveformSamples))
    {
        processorRef.waveformDirty.store (false);
        lastSentWaveformSamples = nSamples;

        constexpr int BAR_COUNT = 250;

        // Read pointers — safe: audio thread only writes to positions >= nSamples.
        // We captured nSamples first, so 0..nSamples-1 are fully written.
        const float* L = processorRef.recordingBuffer.getReadPointer (0);
        const float* R = processorRef.recordingBuffer.getReadPointer (1);

        juce::String rmsJson = "[";

        for (int bar = 0; bar < BAR_COUNT; ++bar)
        {
            const int start = (int) ((int64_t) bar       * nSamples / BAR_COUNT);
            const int end   = (int) ((int64_t) (bar + 1) * nSamples / BAR_COUNT);

            float peak = 0.0f;
            if (start < nSamples && start < end)
            {
                // Sample at most ~10 points per bucket to keep CPU light
                const int step = std::max (1, (end - start) / 10);
                for (int i = start; i < end && i < nSamples; i += step)
                {
                    const float s = std::abs ((L[i] + R[i]) * 0.5f);
                    if (s > peak) peak = s;
                }
            }

            rmsJson += juce::String (peak, 4);
            if (bar < BAR_COUNT - 1) rmsJson += ",";
        }

        rmsJson += "]";

        // Time-based fill fraction: shows waveform progress even for silent recordings.
        // fillFrac = recordedSamples / capacitySamples (clamped 0-1).
        const float durSecs = processorRef.getCaptureDurationSeconds();
        const int capacitySamples = static_cast<int> (durSecs * processorRef.currentSampleRate);
        const float fillFrac = (capacitySamples > 0)
            ? juce::jmin (1.0f, static_cast<float> (nSamples) / static_cast<float> (capacitySamples))
            : 0.0f;

        // Send as object {bars, fill, secs} so JS has all info it needs
        const juce::String waveScript =
            "if(window.groovescout_updateWaveform){"
            "window.groovescout_updateWaveform({"
            "\"bars\":" + rmsJson + ","
            "\"fill\":" + juce::String (fillFrac, 3) + ","
            "\"secs\":" + juce::String (recSecs, 1)
            + "});}";

        webView->evaluateJavascript (waveScript, [] (juce::WebBrowserComponent::EvaluationResult) {});
    }
    else if (nSamples == 0 && lastSentWaveformSamples != 0)
    {
        // Buffer was cleared (new recording started) — reset tracker
        lastSentWaveformSamples = 0;
    }
}

//==============================================================================
// Resource provider (Pattern #8: explicit URL mapping — ALWAYS REQUIRED)
//==============================================================================

std::optional<juce::WebBrowserComponent::Resource>
GrooveScoutAudioProcessorEditor::getResource (const juce::String& url)
{
    // Helper: convert const char* + size to vector<std::byte>
    auto makeVector = [] (const char* data, int size)
    {
        return std::vector<std::byte> (
            reinterpret_cast<const std::byte*> (data),
            reinterpret_cast<const std::byte*> (data) + size);
    };

    // "/" or "/index.html" — main HTML file
    if (url == "/" || url == "/index.html")
    {
        return juce::WebBrowserComponent::Resource {
            makeVector (BinaryData::index_html, BinaryData::index_htmlSize),
            juce::String ("text/html")
        };
    }

    // JUCE 8 frontend library (provides getSliderState, getToggleState, etc.)
    if (url == "/js/juce/index.js")
    {
        return juce::WebBrowserComponent::Resource {
            makeVector (BinaryData::index_js, BinaryData::index_jsSize),
            juce::String ("text/javascript")
        };
    }

    // JUCE native interop checker (Pattern #13: required for correct bridge init)
    if (url == "/js/juce/check_native_interop.js")
    {
        return juce::WebBrowserComponent::Resource {
            makeVector (BinaryData::check_native_interop_js,
                        BinaryData::check_native_interop_jsSize),
            juce::String ("text/javascript")
        };
    }

    // 404 — resource not found
    DBG ("GrooveScout: resource not found: " + url);
    return std::nullopt;
}
