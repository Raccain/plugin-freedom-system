# Stage 3 (GUI) Integration Checklist — GrooveScout v2

**Plugin:** GrooveScout
**Mockup Version:** v2
**Generated:** 2026-02-21
**Window Size:** 750 x 560 px (fixed, non-resizable)
**Parameter Count:** 15 (9 Float + 1 Choice + 5 Bool)
**Special:** DragAndDropContainer, Timer polling, native function bridge

---

## 0. Pre-Integration Verification

- [ ] `v2-ui.html` exists at `plugins/GrooveScout/.ideas/mockups/v2-ui.html`
- [ ] `v2-PluginEditor-TEMPLATE.h` exists (reference for member order)
- [ ] `v2-PluginEditor-TEMPLATE.cpp` exists (reference for init sequence)
- [ ] `v2-CMakeLists-SNIPPET.txt` exists (CMake additions)
- [ ] `parameter-spec.md` exists and matches 15-parameter list below
- [ ] PluginProcessor.h/cpp exist with 15 APVTS parameters registered

---

## 1. Copy UI Files

- [ ] Create directory: `Source/ui/public/js/juce/`
- [ ] Copy `v2-ui.html` to `Source/ui/public/index.html`
- [ ] Obtain JUCE frontend library (`index.js`) from JUCE SDK or existing plugin (e.g., `plugins/GainKnob/Source/ui/public/js/juce/index.js`)
- [ ] Copy `index.js` to `Source/ui/public/js/juce/index.js`
- [ ] Obtain `check_native_interop.js` from same source (Pattern #13: REQUIRED)
- [ ] Copy `check_native_interop.js` to `Source/ui/public/js/juce/check_native_interop.js`
- [ ] Verify no placeholder text remains in `Source/ui/public/index.html`

---

## 2. Update CMakeLists.txt

- [ ] Open `GrooveScout/CMakeLists.txt`
- [ ] Find existing `juce_add_plugin(GrooveScout ...)` call
- [ ] Add `NEEDS_WEB_BROWSER TRUE` to the juce_add_plugin arguments (Pattern #9)
- [ ] Append `v2-CMakeLists-SNIPPET.txt` contents after existing `target_link_libraries()` block
- [ ] Verify `juce::juce_gui_extra` is listed (Pattern #3: required for WebBrowserComponent)
- [ ] Verify `juce::juce_dsp` is listed (required for FFT, IIR filters)
- [ ] Verify `JUCE_WEB_BROWSER=1` definition is present
- [ ] Verify `juce_generate_juce_header(GrooveScout)` is present (Pattern #1)
- [ ] Verify `juce_add_binary_data` includes all 3 files:
  - `Source/ui/public/index.html`
  - `Source/ui/public/js/juce/index.js`
  - `Source/ui/public/js/juce/check_native_interop.js`
- [ ] Regenerate build: `cmake --preset default` (or project-specific preset)

---

## 3. Update PluginEditor.h

- [ ] Open `Source/PluginEditor.h`
- [ ] Add inheritance: `, public juce::DragAndDropContainer`
- [ ] Add inheritance: `, private juce::Timer`
- [ ] Add `timerCallback() override;` to private section
- [ ] Add `getResource()` private method declaration
- [ ] Replace member declarations using `v2-PluginEditor-TEMPLATE.h` as reference
- [ ] Verify CRITICAL MEMBER ORDER:
  - Section 1: 9 `WebSliderRelay` + 1 `WebComboBoxRelay` + 5 `WebToggleButtonRelay` (relays first)
  - Section 2: `std::unique_ptr<juce::WebBrowserComponent> webView` (second)
  - Section 3: 9 `WebSliderParameterAttachment` + 1 `WebComboBoxParameterAttachment` + 5 `WebToggleButtonParameterAttachment` (attachments last)
- [ ] Verify relay count = attachment count = 15
- [ ] `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` present

---

## 4. Update PluginEditor.cpp

- [ ] Open `Source/PluginEditor.cpp`
- [ ] Add `#include <juce_gui_extra/juce_gui_extra.h>` if not present
- [ ] Use `v2-PluginEditor-TEMPLATE.cpp` as reference for constructor
- [ ] Verify constructor initialization sequence:
  - [ ] Step 1: Create all 15 relays (in same order as declarations)
  - [ ] Step 2: Create WebView with all `.withOptionsFrom()` calls (15 total)
  - [ ] Step 3: Register 4 native functions (analyzeButtonPressed, cancelAnalysis, setBpmToDaw, startMidiDrag)
  - [ ] Step 4: Create all 15 parameter attachments (Pattern #12: 3 args including nullptr)
  - [ ] Step 5: `webView->goToURL(...)` and `addAndMakeVisible(*webView)`
  - [ ] Step 6: `setSize(750, 560)`
  - [ ] Step 7: `startTimer(60)`
- [ ] Implement `timerCallback()`:
  - [ ] Calls `stopTimer()` should be in destructor (NOT in timerCallback)
  - [ ] Polls `audioProcessor.isAnalyzing()`, `isAnalysisComplete()`, `getAnalysisProgress()`, `getDetectedBpm()`, `getDetectedKey()`
  - [ ] Builds JSON payload string
  - [ ] Calls `webView->evaluateJavascript(js, ...)`
- [ ] Implement `getResource()` with explicit URL mapping (Pattern #8):
  - [ ] `"/" or "/index.html"` → `BinaryData::index_html` (MIME: `text/html`)
  - [ ] `"/js/juce/index.js"` → `BinaryData::index_js` (MIME: `application/javascript`)
  - [ ] `"/js/juce/check_native_interop.js"` → `BinaryData::check_native_interop_js` (MIME: `application/javascript`)
  - [ ] Return `std::nullopt` for all other URLs
- [ ] Destructor calls `stopTimer()` before member destruction
- [ ] `resized()` calls `webView->setBounds(getLocalBounds())`
- [ ] `paint()` is empty (WebView fills everything)

---

## 5. Update PluginProcessor (if not already done in Stage 2)

- [ ] Verify 15 APVTS parameters registered in `createParameterLayout()`:

  **Float parameters (9):**
  - `kickSensitivity` — range [0.0, 1.0], default 0.5, linear
  - `kickFreqLow` — range [20.0, 1000.0], default 40.0, log skew (~0.4)
  - `kickFreqHigh` — range [20.0, 1000.0], default 120.0, log skew (~0.4)
  - `snareSensitivity` — range [0.0, 1.0], default 0.5, linear
  - `snareFreqLow` — range [100.0, 20000.0], default 200.0, log skew (~0.25)
  - `snareFreqHigh` — range [100.0, 20000.0], default 8000.0, log skew (~0.25)
  - `hihatSensitivity` — range [0.0, 1.0], default 0.5, linear
  - `hihatFreqLow` — range [1000.0, 20000.0], default 5000.0, log skew (~0.4)
  - `hihatFreqHigh` — range [1000.0, 20000.0], default 16000.0, log skew (~0.4)

  **Choice parameter (1):**
  - `bpmMultiplier` — choices: {"1/2x", "1x", "2x"}, default index 1

  **Bool parameters (5):**
  - `analyzeBPM` — default true
  - `analyzeKey` — default true
  - `analyzeKick` — default true
  - `analyzeSnare` — default true
  - `analyzeHihat` — default true

- [ ] Processor exposes these methods (called by PluginEditor):
  - `startAnalysis()` — starts background analysis thread
  - `cancelAnalysis()` — stops background thread
  - `setDawTempo()` — sets host playback tempo to detected BPM
  - `prepareMidiDragForClip(juce::String clipType)` — prepares MIDI file and triggers drag
  - `bool isAnalyzing()` — atomic flag
  - `bool isAnalysisComplete()` — atomic flag
  - `float getAnalysisProgress()` — 0.0 to 1.0
  - `float getDetectedBpm()` — detected BPM, 0 if not detected yet
  - `juce::String getDetectedKey()` — key string, empty if not detected

---

## 6. Build and Test (Debug)

- [ ] Build succeeds without warnings: `cmake --build build --config Debug`
- [ ] Standalone app launches
- [ ] WebView loads (not blank screen)
- [ ] Open DevTools: right-click in plugin area (if enabled) or use external debugger
- [ ] Console shows no JavaScript errors
- [ ] `window.__JUCE__` object exists in console
- [ ] All parameter names visible in JUCE backend state

---

## 7. Test Parameter Binding (15 parameters)

- [ ] `kickSensitivity` slider moves, value updates in console
- [ ] `kickFreqLow` dual-handle low position syncs to APVTS
- [ ] `kickFreqHigh` dual-handle high position syncs to APVTS
- [ ] `snareSensitivity` slider moves
- [ ] `snareFreqLow` / `snareFreqHigh` dual-handle sliders
- [ ] `hihatSensitivity` slider moves
- [ ] `hihatFreqLow` / `hihatFreqHigh` dual-handle sliders
- [ ] `bpmMultiplier` chip selection updates APVTS Choice parameter
- [ ] `analyzeBPM` toggle updates APVTS Bool
- [ ] `analyzeKey` toggle updates APVTS Bool
- [ ] `analyzeKick` toggle updates APVTS Bool and disables kick card controls
- [ ] `analyzeSnare` toggle updates APVTS Bool and disables snare card controls
- [ ] `analyzeHihat` toggle updates APVTS Bool and disables hihat card controls
- [ ] Preset save/load restores all 15 parameter values correctly
- [ ] Automation from DAW updates slider/toggle visuals

---

## 8. Test Native Functions

- [ ] Analyze button click calls `analyzeButtonPressed` → processor starts analysis
- [ ] Button switches to cancel icon during analysis
- [ ] Cancel button click calls `cancelAnalysis` → analysis stops
- [ ] `groovescout_updateAnalysisState()` called from timer, updates UI:
  - [ ] Progress bar fills during analysis
  - [ ] BPM display updates when detected
  - [ ] Key display updates when detected
  - [ ] MIDI clip tiles transition from placeholder to active state
- [ ] BPM display click calls `setBpmToDaw` → DAW tempo changes
- [ ] MIDI clip drag (chord, kick, snare, hihat) calls `startMidiDrag` with correct type

---

## 9. Test MIDI Drag-and-Drop

- [ ] Drag from ROOT CHORD clip to DAW track — MIDI file drops correctly
- [ ] Drag from KICK clip to DAW track
- [ ] Drag from SNARE clip to DAW track
- [ ] Drag from HIHAT clip to DAW track
- [ ] Drag only works when clip is in "active" state (analysis complete)
- [ ] Drag cursor changes to grab cursor on hover over active clip
- [ ] Test in at least: Ableton Live (primary), Logic Pro, Reaper

---

## 10. Build and Test (Release)

- [ ] Release build succeeds without warnings
- [ ] No crashes on plugin reload (test 10 rapid reloads in DAW)
- [ ] Member destruction order validated: no "evaluateJavascript on destroyed webView" crashes
- [ ] Install and test as AU and VST3 in DAW
- [ ] All drag-and-drop operations work in release build

---

## 11. WebView-Specific Validation

- [ ] No viewport units in `Source/ui/public/index.html` (`100vh`, `100vw`, `100dvh`, `100svh` are FORBIDDEN)
- [ ] `html, body { height: 100%; }` is present
- [ ] `user-select: none` is present on body
- [ ] `contextmenu` event is suppressed in JavaScript
- [ ] Resource provider returns 200 for all 3 files (no 404s in console)
- [ ] Correct MIME types: `text/html` for HTML, `application/javascript` for JS files
- [ ] All 15 parameters accessible via `getSliderState` / `getComboBoxState` / `getToggleState`
- [ ] No `100vh` / `100vw` in any CSS

---

## Parameter Summary

| ID | Type | Relay | Attachment |
|----|------|-------|-----------|
| `kickSensitivity` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `kickFreqLow` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `kickFreqHigh` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `snareSensitivity` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `snareFreqLow` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `snareFreqHigh` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `hihatSensitivity` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `hihatFreqLow` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `hihatFreqHigh` | Float | `WebSliderRelay` | `WebSliderParameterAttachment` |
| `bpmMultiplier` | Choice | `WebComboBoxRelay` | `WebComboBoxParameterAttachment` |
| `analyzeBPM` | Bool | `WebToggleButtonRelay` | `WebToggleButtonParameterAttachment` |
| `analyzeKey` | Bool | `WebToggleButtonRelay` | `WebToggleButtonParameterAttachment` |
| `analyzeKick` | Bool | `WebToggleButtonRelay` | `WebToggleButtonParameterAttachment` |
| `analyzeSnare` | Bool | `WebToggleButtonRelay` | `WebToggleButtonParameterAttachment` |
| `analyzeHihat` | Bool | `WebToggleButtonRelay` | `WebToggleButtonParameterAttachment` |

**Total: 15 relays, 15 attachments**

---

## Native Functions (Non-APVTS)

| JS call | C++ handler | Purpose |
|---------|------------|---------|
| `getNativeFunction("analyzeButtonPressed")()` | `audioProcessor.startAnalysis()` | Start background analysis |
| `getNativeFunction("cancelAnalysis")()` | `audioProcessor.cancelAnalysis()` | Stop analysis thread |
| `getNativeFunction("setBpmToDaw")()` | `audioProcessor.setDawTempo()` | Set DAW project tempo |
| `getNativeFunction("startMidiDrag")(clipType)` | `audioProcessor.prepareMidiDragForClip(...)` | Trigger MIDI file drag |

---

## Key Reference Patterns

| Issue | Pattern | Location |
|-------|---------|---------|
| Member order crashes | #11 | juce8-critical-patterns.md |
| 3-arg attachment | #12 | juce8-critical-patterns.md |
| check_native_interop.js | #13 | juce8-critical-patterns.md |
| Bool uses getToggleState | #19 | juce8-critical-patterns.md |
| ES6 type="module" | #21 | juce8-critical-patterns.md |
| NEEDS_WEB_BROWSER | #9 | juce8-critical-patterns.md |
| Explicit URL mapping | #8 | juce8-critical-patterns.md |
| valueChangedEvent no args | #15 | juce8-critical-patterns.md |
