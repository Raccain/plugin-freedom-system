#include "PluginEditor.h"

//==============================================================================
// Constructor
// CRITICAL ORDER: Relays → WebView → Attachments
//==============================================================================

GrooveScoutAudioProcessorEditor::GrooveScoutAudioProcessorEditor(
    GrooveScoutAudioProcessor& p)
    : AudioProcessorEditor(p), audioProcessor(p)
{
    // =========================================================================
    // INITIALIZATION SEQUENCE (CRITICAL ORDER — see header comments)
    //
    //  STEP 1: Create all relays (before WebView construction)
    //  STEP 2: Create WebView (registers all relay options at construction time)
    //  STEP 3: Create parameter attachments (after WebView construction)
    //  STEP 4: Register native functions (non-APVTS callbacks)
    //  STEP 5: Final WebView setup and window size
    //
    // Matches member declaration order. Ensures safe construction/destruction.
    // =========================================================================

    // -------------------------------------------------------------------------
    // STEP 1: CREATE RELAYS
    // Parameter IDs must match APVTS parameter IDs in PluginProcessor.cpp exactly.
    // -------------------------------------------------------------------------

    // Float sliders (9)
    kickSensitivityRelay  = std::make_unique<juce::WebSliderRelay>("kickSensitivity");
    kickFreqLowRelay      = std::make_unique<juce::WebSliderRelay>("kickFreqLow");
    kickFreqHighRelay     = std::make_unique<juce::WebSliderRelay>("kickFreqHigh");
    snareSensitivityRelay = std::make_unique<juce::WebSliderRelay>("snareSensitivity");
    snareFreqLowRelay     = std::make_unique<juce::WebSliderRelay>("snareFreqLow");
    snareFreqHighRelay    = std::make_unique<juce::WebSliderRelay>("snareFreqHigh");
    hihatSensitivityRelay = std::make_unique<juce::WebSliderRelay>("hihatSensitivity");
    hihatFreqLowRelay     = std::make_unique<juce::WebSliderRelay>("hihatFreqLow");
    hihatFreqHighRelay    = std::make_unique<juce::WebSliderRelay>("hihatFreqHigh");

    // Choice (1)
    bpmMultiplierRelay    = std::make_unique<juce::WebComboBoxRelay>("bpmMultiplier");

    // Bool toggles (5)
    analyzeBPMRelay   = std::make_unique<juce::WebToggleButtonRelay>("analyzeBPM");
    analyzeKeyRelay   = std::make_unique<juce::WebToggleButtonRelay>("analyzeKey");
    analyzeKickRelay  = std::make_unique<juce::WebToggleButtonRelay>("analyzeKick");
    analyzeSnareRelay = std::make_unique<juce::WebToggleButtonRelay>("analyzeSnare");
    analyzeHihatRelay = std::make_unique<juce::WebToggleButtonRelay>("analyzeHihat");

    // -------------------------------------------------------------------------
    // STEP 2: CREATE WEBVIEW
    // All relays registered via .withOptionsFrom() before WebView is active.
    // -------------------------------------------------------------------------

    webView = std::make_unique<juce::WebBrowserComponent>(
        juce::WebBrowserComponent::Options{}
            // REQUIRED: Enable JUCE frontend library (getSliderState, etc.)
            .withNativeIntegrationEnabled()

            // REQUIRED: Resource provider for embedded binary files
            .withResourceProvider([this](const auto& url) {
                return getResource(url);
            })

            // Float slider relays (9)
            .withOptionsFrom(*kickSensitivityRelay)
            .withOptionsFrom(*kickFreqLowRelay)
            .withOptionsFrom(*kickFreqHighRelay)
            .withOptionsFrom(*snareSensitivityRelay)
            .withOptionsFrom(*snareFreqLowRelay)
            .withOptionsFrom(*snareFreqHighRelay)
            .withOptionsFrom(*hihatSensitivityRelay)
            .withOptionsFrom(*hihatFreqLowRelay)
            .withOptionsFrom(*hihatFreqHighRelay)

            // Choice relay (1)
            .withOptionsFrom(*bpmMultiplierRelay)

            // Bool toggle relays (5)
            .withOptionsFrom(*analyzeBPMRelay)
            .withOptionsFrom(*analyzeKeyRelay)
            .withOptionsFrom(*analyzeKickRelay)
            .withOptionsFrom(*analyzeSnareRelay)
            .withOptionsFrom(*analyzeHihatRelay)

            // Native function registrations (non-APVTS actions)
            // These allow JavaScript to call C++ methods via getNativeFunction()
            .withNativeFunction(
                juce::Identifier("analyzeButtonPressed"),
                [this](auto, auto complete) {
                    audioProcessor.startAnalysis();
                    complete(juce::var{});
                }
            )
            .withNativeFunction(
                juce::Identifier("cancelAnalysis"),
                [this](auto, auto complete) {
                    audioProcessor.cancelAnalysis();
                    complete(juce::var{});
                }
            )
            .withNativeFunction(
                juce::Identifier("setBpmToDaw"),
                [this](auto, auto complete) {
                    // audioProcessor.getPlayHead()->... or similar DAW tempo API
                    // Implementation: see architecture.md for DAW tempo set approach
                    audioProcessor.setDawTempo();
                    complete(juce::var{});
                }
            )
            .withNativeFunction(
                juce::Identifier("startMidiDrag"),
                [this](const juce::Array<juce::var>& args, auto complete) {
                    // args[0] = clip type string: "chord", "kick", "snare", "hihat"
                    if (args.size() > 0)
                    {
                        juce::String clipType = args[0].toString();
                        // performExternalDragDropOfFiles triggers OS drag
                        // Requires DragAndDropContainer inheritance
                        // Implementation: see architecture.md
                        audioProcessor.prepareMidiDragForClip(clipType);
                    }
                    complete(juce::var{});
                }
            )
    );

    // -------------------------------------------------------------------------
    // STEP 3: CREATE PARAMETER ATTACHMENTS
    // Pattern #12: JUCE 8 requires 3 parameters (parameter, relay, undoManager)
    // nullptr = no undo manager
    // -------------------------------------------------------------------------

    // Float slider attachments (9)
    kickSensitivityAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("kickSensitivity"),
        *kickSensitivityRelay,
        nullptr
    );

    kickFreqLowAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("kickFreqLow"),
        *kickFreqLowRelay,
        nullptr
    );

    kickFreqHighAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("kickFreqHigh"),
        *kickFreqHighRelay,
        nullptr
    );

    snareSensitivityAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("snareSensitivity"),
        *snareSensitivityRelay,
        nullptr
    );

    snareFreqLowAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("snareFreqLow"),
        *snareFreqLowRelay,
        nullptr
    );

    snareFreqHighAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("snareFreqHigh"),
        *snareFreqHighRelay,
        nullptr
    );

    hihatSensitivityAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("hihatSensitivity"),
        *hihatSensitivityRelay,
        nullptr
    );

    hihatFreqLowAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("hihatFreqLow"),
        *hihatFreqLowRelay,
        nullptr
    );

    hihatFreqHighAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
        *audioProcessor.parameters.getParameter("hihatFreqHigh"),
        *hihatFreqHighRelay,
        nullptr
    );

    // Choice attachment (1)
    bpmMultiplierAttachment = std::make_unique<juce::WebComboBoxParameterAttachment>(
        *audioProcessor.parameters.getParameter("bpmMultiplier"),
        *bpmMultiplierRelay,
        nullptr
    );

    // Bool toggle attachments (5)
    analyzeBPMAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
        *audioProcessor.parameters.getParameter("analyzeBPM"),
        *analyzeBPMRelay,
        nullptr
    );

    analyzeKeyAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
        *audioProcessor.parameters.getParameter("analyzeKey"),
        *analyzeKeyRelay,
        nullptr
    );

    analyzeKickAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
        *audioProcessor.parameters.getParameter("analyzeKick"),
        *analyzeKickRelay,
        nullptr
    );

    analyzeSnareAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
        *audioProcessor.parameters.getParameter("analyzeSnare"),
        *analyzeSnareRelay,
        nullptr
    );

    analyzeHihatAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
        *audioProcessor.parameters.getParameter("analyzeHihat"),
        *analyzeHihatRelay,
        nullptr
    );

    // -------------------------------------------------------------------------
    // STEP 4: WEBVIEW NAVIGATION AND DISPLAY
    // -------------------------------------------------------------------------

    // Navigate to root (triggers resource provider for index.html)
    webView->goToURL(juce::WebBrowserComponent::getResourceProviderRoot());

    // Add WebView as visible child component
    addAndMakeVisible(*webView);

    // -------------------------------------------------------------------------
    // STEP 5: WINDOW SIZE (from v2-ui.yaml dimensions: 750 x 560)
    // -------------------------------------------------------------------------
    setSize(750, 560);

    // -------------------------------------------------------------------------
    // STEP 6: START TIMER for analysis state polling
    // 60ms interval matches UI 60fps refresh, avoids flooding evaluateJavascript
    // -------------------------------------------------------------------------
    startTimer(60);
}

//==============================================================================
// Destructor
//==============================================================================

GrooveScoutAudioProcessorEditor::~GrooveScoutAudioProcessorEditor()
{
    // Stop timer before members are destroyed
    stopTimer();

    // Members automatically destroyed in reverse declaration order:
    //  1. Bool toggle attachments destroyed first
    //  2. Choice attachment
    //  3. Float slider attachments
    //  4. WebView destroyed (safe — attachments are gone)
    //  5. Bool relays destroyed
    //  6. Choice relay destroyed
    //  7. Float slider relays destroyed last
    //
    // No manual cleanup needed if member order is correct!
}

//==============================================================================
// AudioProcessorEditor Overrides
//==============================================================================

void GrooveScoutAudioProcessorEditor::paint(juce::Graphics& g)
{
    // WebView fills the entire editor — no custom painting required
    (void)g;
}

void GrooveScoutAudioProcessorEditor::resized()
{
    // Make WebView fill the entire editor bounds (750 x 560, fixed size)
    if (webView)
        webView->setBounds(getLocalBounds());
}

//==============================================================================
// Timer Callback — Analysis State Polling
//==============================================================================

void GrooveScoutAudioProcessorEditor::timerCallback()
{
    // Poll analysis state from processor
    // audioProcessor exposes: isAnalysing(), isComplete(), getProgress(),
    //   getDetectedBpm(), getDetectedKey() — adapt to actual processor API

    if (!webView) return;

    const bool isAnalysing = audioProcessor.isAnalyzing();
    const bool isComplete  = audioProcessor.isAnalysisComplete();
    const float progress   = audioProcessor.getAnalysisProgress();
    const float bpm        = audioProcessor.getDetectedBpm();
    const juce::String key = audioProcessor.getDetectedKey();

    // Build JSON payload for JavaScript
    juce::String bpmStr  = (bpm > 0.0f) ? juce::String(bpm, 1) : "null";
    juce::String keyStr  = key.isNotEmpty() ? ("\"" + key + "\"") : "null";

    juce::String js = juce::String("window.groovescout_updateAnalysisState({"
        "isAnalyzing:") + (isAnalysing ? "true" : "false") +
        ",isComplete:" + (isComplete ? "true" : "false") +
        ",progress:" + juce::String(progress, 3) +
        ",bpm:" + bpmStr +
        ",key:" + keyStr +
        "});";

    webView->evaluateJavascript(js, [](juce::WebBrowserComponent::EvaluationResult) {});
}

//==============================================================================
// Resource Provider  (Pattern #8: Explicit URL mapping — ALWAYS REQUIRED)
//==============================================================================

std::optional<juce::WebBrowserComponent::Resource>
GrooveScoutAudioProcessorEditor::getResource(const juce::String& url)
{
    // =========================================================================
    // BinaryData symbol naming:
    //   Source/ui/public/index.html               → BinaryData::index_html
    //   Source/ui/public/js/juce/index.js         → BinaryData::index_js
    //   Source/ui/public/js/juce/check_native_interop.js
    //                                             → BinaryData::check_native_interop_js
    //
    // Pattern #8: Explicit mapping bridges original paths ↔ BinaryData symbols.
    // Pattern #13: check_native_interop.js is REQUIRED for WebView init.
    // =========================================================================

    auto makeVector = [](const char* data, int size) {
        return std::vector<std::byte>(
            reinterpret_cast<const std::byte*>(data),
            reinterpret_cast<const std::byte*>(data) + size
        );
    };

    // Root → index.html
    if (url == "/" || url == "/index.html") {
        return juce::WebBrowserComponent::Resource {
            makeVector(BinaryData::index_html, BinaryData::index_htmlSize),
            juce::String("text/html")
        };
    }

    // JUCE frontend library (ES6 module — Pattern #21)
    if (url == "/js/juce/index.js") {
        return juce::WebBrowserComponent::Resource {
            makeVector(BinaryData::index_js, BinaryData::index_jsSize),
            juce::String("application/javascript")  // Pattern #8: correct MIME type
        };
    }

    // Pattern #13: REQUIRED for WebView native integration initialization
    if (url == "/js/juce/check_native_interop.js") {
        return juce::WebBrowserComponent::Resource {
            makeVector(BinaryData::check_native_interop_js,
                       BinaryData::check_native_interop_jsSize),
            juce::String("application/javascript")
        };
    }

    // 404 — resource not found
    return std::nullopt;
}
