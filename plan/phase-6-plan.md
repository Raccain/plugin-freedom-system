# Phase 6 Implementation Plan

## Prerequisites Status

**Previous Phase(s) Required:** Phase 0, 1, 2, 3, 4, 5

**Deliverables Check:**

### Phase 0 (Foundation & Contracts)
- ✅ `.claude/` directory structure - EXISTS
- ✅ `PLUGINS.md` - EXISTS
- ✅ Contract templates in skill assets - EXISTS

### Phase 1 (Discovery System)
- ✅ All 9 slash commands - EXISTS
- ✅ All 7 skills (basic implementation) - EXISTS
- ✅ Interactive decision system - EXISTS

### Phase 2 (Workflow Engine)
- ✅ plugin-workflow skill with Stages 0,1,6 - EXISTS
- ✅ PLUGINS.md state machine - EXISTS
- ✅ `.continue-here.md` handoff system - EXISTS

### Phase 3 (Implementation Subagents)
- ✅ foundation-agent - EXISTS
- ✅ shell-agent - EXISTS
- ✅ dsp-agent - EXISTS
- ✅ gui-agent - EXISTS
- ✅ Dispatcher integration in plugin-workflow - EXISTS

### Phase 4 (Build & Troubleshooting)
- ✅ `scripts/build-and-install.sh` - EXISTS
- ✅ build-automation skill - EXISTS
- ✅ troubleshooter agent - EXISTS

### Phase 5 (Validation System)
- ✅ Python validation scripts (`.claude/hooks/validators/`) - EXISTS
  - ✅ `validate-parameters.py`
  - ✅ `validate-dsp-components.py`
  - ✅ `validate-gui-bindings.py`
  - ✅ `validate-foundation.py`
- ✅ Hook scripts (`.claude/hooks/`) - EXISTS
  - ✅ `PostToolUse.sh`
  - ✅ `SubagentStop.sh`
  - ✅ `UserPromptSubmit.sh`
  - ✅ `Stop.sh`
  - ✅ `PreCompact.sh`
  - ✅ `SessionStart.sh`
- ✅ validator subagent (`.claude/agents/validator.md`) - EXISTS

### Phase 6 (WebView UI System) - PARTIAL
- ⚠️ ui-mockup skill - EXISTS but INCOMPLETE (missing WebView templates, ui-design-rules.md)
- ❌ ui-template-library skill - MISSING
- ❌ `.claude/aesthetics/` directory - MISSING
- ⚠️ gui-agent WebView integration - PARTIALLY EXISTS (needs WebView-specific patterns)

**Status:** Complete (Phases 0-5) / Partial (Phase 6 foundations exist)

---

## Phase Overview

Phase 6 completes the WebView UI system, enabling production-ready plugin interfaces with beautiful HTML/CSS/JavaScript UIs that are directly embedded in JUCE plugins. This phase implements:

1. **Two-phase mockup workflow** - Fast design iteration (HTML only) followed by implementation scaffolding (C++ boilerplate) only after design approval
2. **Aesthetic template system** - Save/reuse visual design systems across plugins (colors, typography, control styling) while generating appropriate layouts for different parameter counts
3. **WebView integration patterns** - Critical safety patterns (member declaration order), resource provider, relay pattern, parameter binding
4. **gui-agent WebView support** - Stage 5 subagent generates WebView-based UIs following proven patterns

**Why this phase is critical:** Beautiful, modern UIs differentiate plugins and improve user experience. WebView enables rapid UI iteration without C++ recompilation. The aesthetic template system creates design consistency across plugin collections. Critical safety patterns prevent 90% of WebView crashes.

## Sub-Phase Breakdown

This phase is split into 3 sub-phases for logical organization:

- **Phase 6a:** ui-mockup skill completion (WebView templates + UI design rules)
- **Phase 6b:** ui-template-library skill (aesthetic system management)
- **Phase 6c:** gui-agent WebView integration + CMake patterns

**Rationale:**
- 6a establishes WebView infrastructure and design workflow
- 6b adds aesthetic reusability (independent feature)
- 6c integrates WebView into Stage 5 implementation flow

## Required Documentation

### Architecture Files (MUST READ IN ENTIRETY)

- **`architecture/12-webview-integration-design.md`** - Complete WebView architecture specification including:
  - Member declaration order (critical safety pattern)
  - Resource provider pattern (JUCE 8 requirement)
  - Relay pattern and parameter binding
  - CSS constraints (viewport units, native feel)
  - Two-phase mockup workflow architecture
  - Aesthetic template system architecture

- **`architecture/02-core-abstractions.md`** - Lines 254-368: ui-mockup and ui-template-library skill abstractions

- **`architecture/04-component-architecture.md`** - WebView component patterns if mentioned

- **`architecture/10-extension-architecture.md`** - Lines 1-95: Skill structure and integration patterns

### Procedure Files (MUST READ IN ENTIRETY)

**WebView Procedures (procedures/webview/):**

- **`procedures/webview/README.md`** - ⚠️ START HERE - Navigation guide for WebView documentation

- **`procedures/webview/01-overview.md`** - WebView concepts, architecture patterns, when to use WebView vs native JUCE

- **`procedures/webview/02-project-setup.md`** - CMake configuration patterns for WebView integration

- **`procedures/webview/03-communication-patterns.md`** - C++ ↔ JavaScript communication (4 C++→JS methods, 2 JS→C++ methods)

- **`procedures/webview/04-parameter-binding.md`** - Relay pattern implementation and parameter attachments

- **`procedures/webview/05-audio-visualization.md`** - Real-time data visualization patterns (meters, waveforms, analyzers)

- **`procedures/webview/06-development-workflow.md`** - Hot reloading setup and debugging techniques

- **`procedures/webview/07-distribution.md`** - File embedding and production packaging (juce_add_binary_data)

- **`procedures/webview/best-practices.md`** - ⚠️ CRITICAL: Member declaration order enforcement

- **`procedures/webview/common-problems.md`** - Troubleshooting guide for common WebView issues

**Skill Procedures:**

- **`procedures/skills/ui-mockup.md`** - Complete two-phase mockup workflow specification

- **`procedures/skills/ui-template-library.md`** - Aesthetic system management (save/apply/list/delete)

**Agent Procedures:**

- **`procedures/agents/gui-agent.md`** - Sections on WebView integration (Stage 5 implementation)

## Implementation Sequence

---

## Phase 6a: ui-mockup Skill Completion

**Objective:** Complete ui-mockup skill with WebView code templates and UI design rules.

---

### Task 1: Create UI Design Rules Reference

**Description:** Create comprehensive UI design rules document that enforces WebView best practices and CSS constraints.

**Required Reading:**
- `architecture/12-webview-integration-design.md` (sections: CSS constraints, native feel patterns)
- `procedures/webview/01-overview.md` (all)
- `procedures/webview/best-practices.md` (all)

**Dependencies:** None

**Implementation Steps:**

1. Create file: `.claude/skills/ui-mockup/references/ui-design-rules.md`
2. Document CSS constraints:
   - ❌ FORBIDDEN: `100vh`, `100vw`, `100dvh` (viewport units cause blank first render)
   - ✅ REQUIRED: `100%` for heights, `html, body { height: 100%; }`
   - Explanation of why viewport units fail (JUCE WebView initialization timing)
3. Document native application feel requirements:
   - CSS: `user-select: none`, `-webkit-user-select: none`, `cursor: default`, `overflow: hidden`
   - JavaScript: Disable right-click context menu
   - Rationale: Plugins should feel like native apps, not web pages
4. Document sizing strategies:
   - Fixed size: `setResizable(false, false)` + fixed CSS widths
   - Resizable: `setResizable(true, true)` + aspect ratio lock + `clamp()` CSS
   - Decision criteria: Simple plugins (≤5 params) = fixed, complex = resizable
5. Document resource provider requirements:
   - JUCE 8 standard pattern (replaces JUCE 7 data URLs)
   - All files must be embedded via `juce_add_binary_data`
   - No external file loading in production
6. Add examples for each rule with ✅ CORRECT and ❌ WRONG patterns

**Expected Output:**
- File: `.claude/skills/ui-mockup/references/ui-design-rules.md` (~300 lines)

**Verification:**
- Automated: `test -f .claude/skills/ui-mockup/references/ui-design-rules.md && echo "EXISTS"`
- Manual: STOP AND ASK LEX: "Please review `.claude/skills/ui-mockup/references/ui-design-rules.md` to verify it covers CSS constraints (no viewport units), native feel patterns, sizing strategies, and resource provider requirements. Does this match the WebView best practices?"

---

### Task 2: Create WebView Code Templates (PluginEditor Header)

**Description:** Create template for PluginEditor.h with CORRECT member declaration order (prevents 90% of release build crashes).

**Required Reading:**
- `architecture/12-webview-integration-design.md` (sections: member declaration order, relay pattern)
- `procedures/webview/04-parameter-binding.md` (all)
- `procedures/webview/best-practices.md` (all)

**Dependencies:** Task 1 complete

**Implementation Steps:**

1. Create directory: `.claude/skills/ui-mockup/assets/webview-templates/`
2. Create file: `.claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h`
3. Implement header template with correct member order:
   ```cpp
   #pragma once
   #include <JuceHeader.h>
   #include "PluginProcessor.h"

   class PluginEditor : public juce::AudioProcessorEditor, private juce::Timer
   {
   public:
       PluginEditor(PluginProcessor& p);
       ~PluginEditor() override;

       void paint(juce::Graphics&) override;
       void resized() override;

   private:
       void timerCallback() override;

       std::optional<juce::WebBrowserComponent::Resource> getResource(
           const juce::String& url
       );

       PluginProcessor& audioProcessor;

       // ⚠️ CRITICAL ORDER: Relays → WebView → Attachments
       // Destroyed in reverse order (attachments first, relays last)

       // 1. Relays (created first, destroyed last)
       std::unique_ptr<juce::WebSliderRelay> gainRelay;
       std::unique_ptr<juce::WebSliderRelay> toneRelay;
       std::unique_ptr<juce::WebToggleButtonRelay> bypassRelay;
       std::unique_ptr<juce::WebComboBoxRelay> modeRelay;

       // 2. WebView (created after relays, destroyed before relays)
       std::unique_ptr<juce::WebBrowserComponent> webView;

       // 3. Parameter Attachments (created last, destroyed first)
       std::unique_ptr<juce::WebSliderParameterAttachment> gainAttachment;
       std::unique_ptr<juce::WebSliderParameterAttachment> toneAttachment;
       std::unique_ptr<juce::WebToggleButtonParameterAttachment> bypassAttachment;
       std::unique_ptr<juce::WebComboBoxParameterAttachment> modeAttachment;

       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
   };
   ```
4. Add extensive comments explaining:
   - Why order matters (destructor sequence)
   - What happens if wrong (UB in release builds, crashes)
   - Why debug builds hide the bug
5. Add placeholder comments: `// {{RELAY_DECLARATIONS}}`, `// {{ATTACHMENT_DECLARATIONS}}`
6. Document relay type mapping:
   - Slider/Knob → `juce::WebSliderRelay` + `WebSliderParameterAttachment`
   - Toggle → `juce::WebToggleButtonRelay` + `WebToggleButtonParameterAttachment`
   - Dropdown → `juce::WebComboBoxRelay` + `WebComboBoxParameterAttachment`

**Expected Output:**
- File: `.claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h` (~100 lines)
- Correct member order enforced via template structure
- Clear comments explaining safety requirements

**Verification:**
- Automated: `test -f .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h && grep -q "CRITICAL ORDER" .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h`
- Manual: STOP AND ASK LEX: "Please review `.claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h` to verify member declaration order is correct (relays → webView → attachments) and comments explain why this prevents crashes. Does the template structure enforce safe construction/destruction order?"

---

### Task 3: Create WebView Code Templates (PluginEditor Implementation)

**Description:** Create template for PluginEditor.cpp with resource provider pattern and correct relay/attachment initialization order.

**Required Reading:**
- `architecture/12-webview-integration-design.md` (sections: resource provider, relay pattern)
- `procedures/webview/04-parameter-binding.md` (all)
- `procedures/webview/07-distribution.md` (all)

**Dependencies:** Task 2 complete

**Implementation Steps:**

1. Create file: `.claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.cpp`
2. Implement constructor with correct initialization order:
   ```cpp
   PluginEditor::PluginEditor(PluginProcessor& p)
       : AudioProcessorEditor(p), audioProcessor(p)
   {
       // 1. CREATE RELAYS FIRST (before WebView construction)
       gainRelay = std::make_unique<juce::WebSliderRelay>("GAIN");
       toneRelay = std::make_unique<juce::WebSliderRelay>("TONE");
       bypassRelay = std::make_unique<juce::WebToggleButtonRelay>("BYPASS");
       modeRelay = std::make_unique<juce::WebComboBoxRelay>("MODE");

       // 2. CREATE WEBVIEW with relay options
       webView = std::make_unique<juce::WebBrowserComponent>(
           juce::WebBrowserComponent::Options{}
               .withNativeIntegrationEnabled()
               .withOptionsFrom(*gainRelay)
               .withOptionsFrom(*toneRelay)
               .withOptionsFrom(*bypassRelay)
               .withOptionsFrom(*modeRelay)
               .withResourceProvider([this](const auto& url) {
                   return getResource(url);
               })
       );

       // 3. CREATE PARAMETER ATTACHMENTS (after WebView construction)
       gainAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
           *audioProcessor.apvts.getParameter("GAIN"),
           *gainRelay
       );
       toneAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
           *audioProcessor.apvts.getParameter("TONE"),
           *toneRelay
       );
       bypassAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
           *audioProcessor.apvts.getParameter("BYPASS"),
           *bypassRelay
       );
       modeAttachment = std::make_unique<juce::WebComboBoxParameterAttachment>(
           *audioProcessor.apvts.getParameter("MODE"),
           *modeRelay
       );

       // Navigate to root (loads index.html)
       webView->goToURL("https://dummy.invalid/");

       addAndMakeVisible(*webView);

       // Start timer for real-time updates (60ms ≈ 16 FPS)
       startTimerHz(16);

       setSize(600, 400);
   }
   ```
3. Implement resource provider pattern:
   ```cpp
   std::optional<juce::WebBrowserComponent::Resource>
   PluginEditor::getResource(const juce::String& url)
   {
       // Map URLs to embedded binary data
       if (url == "/" || url == "/index.html") {
           return juce::WebBrowserComponent::Resource {
               BinaryData::index_html,
               BinaryData::index_htmlSize,
               "text/html"
           };
       }

       if (url == "/js/juce/index.js") {
           return juce::WebBrowserComponent::Resource {
               BinaryData::juce_index_js,
               BinaryData::juce_index_jsSize,
               "text/javascript"
           };
       }

       // ... more resources

       return std::nullopt;
   }
   ```
4. Implement timer callback for real-time updates:
   ```cpp
   void PluginEditor::timerCallback()
   {
       // Example: Update meters from audio thread data
       float level = audioProcessor.getCurrentLevel();

       webView->emitEventIfBrowserIsVisible("meterUpdate", {
           { "level", level },
           { "timestamp", juce::Time::currentTimeMillis() }
       });
   }
   ```
5. Add placeholder comments: `// {{RELAY_CREATION}}`, `// {{WEBVIEW_OPTIONS}}`, `// {{ATTACHMENT_CREATION}}`, `// {{RESOURCE_MAPPING}}`
6. Add comments explaining each step and why order matters

**Expected Output:**
- File: `.claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.cpp` (~200 lines)
- Correct initialization order enforced
- Resource provider pattern implemented
- Timer-based updates pattern shown

**Verification:**
- Automated: `test -f .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.cpp && grep -q "CREATE RELAYS FIRST" .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.cpp`
- Manual: STOP AND ASK LEX: "Please review `.claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.cpp` to verify initialization order (relays → webView → attachments) and resource provider pattern. Does the template show clear examples of relay creation, WebView options registration, and parameter attachment creation?"

---

### Task 4: Create WebView HTML/JavaScript Template

**Description:** Create template HTML with JUCE parameter binding examples and CSS constraints enforced.

**Required Reading:**
- `architecture/12-webview-integration-design.md` (sections: CSS constraints, parameter binding)
- `procedures/webview/03-communication-patterns.md` (all)
- `procedures/webview/04-parameter-binding.md` (all)

**Dependencies:** Task 1, 2, 3 complete

**Implementation Steps:**

1. Create file: `.claude/skills/ui-mockup/assets/webview-templates/index-template.html`
2. Implement HTML structure with CSS constraints:
   ```html
   <!DOCTYPE html>
   <html lang="en">
   <head>
       <meta charset="UTF-8">
       <meta name="viewport" content="width=device-width, initial-scale=1.0">
       <title>{{PLUGIN_NAME}} UI</title>
       <style>
           /* ⚠️ CRITICAL: Use 100%, NOT 100vh */
           html, body {
               margin: 0;
               padding: 0;
               height: 100%;  /* NOT 100vh - viewport units break first render */
               overflow: hidden;
           }

           body {
               /* Native application feel */
               user-select: none;
               -webkit-user-select: none;
               cursor: default;

               /* Plugin styling */
               background: #2a2a2a;
               color: #f0f0f0;
               font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;

               /* Layout */
               display: flex;
               flex-direction: column;
               justify-content: center;
               align-items: center;
           }

           .control-group {
               display: flex;
               gap: 20px;
               padding: 20px;
           }

           .control {
               display: flex;
               flex-direction: column;
               align-items: center;
               gap: 10px;
           }

           input[type="range"] {
               width: 100px;
           }
       </style>
   </head>
   <body>
       <h1>{{PLUGIN_NAME}}</h1>

       <div class="control-group">
           <div class="control">
               <label for="gain-slider">Gain</label>
               <input type="range" id="gain-slider" min="0" max="1" step="0.01" value="0.5">
               <span id="gain-value">50%</span>
           </div>

           <div class="control">
               <label for="tone-slider">Tone</label>
               <input type="range" id="tone-slider" min="0" max="1" step="0.01" value="0.5">
               <span id="tone-value">50%</span>
           </div>
       </div>

       <script type="module">
           import * as Juce from "./js/juce/index.js";

           // Disable right-click context menu (native feel)
           document.addEventListener("contextmenu", (e) => e.preventDefault());

           // Get state objects from C++
           const gainState = Juce.getSliderState("GAIN");
           const toneState = Juce.getSliderState("TONE");

           // Get HTML elements
           const gainSlider = document.getElementById("gain-slider");
           const toneSlider = document.getElementById("tone-slider");
           const gainValue = document.getElementById("gain-value");
           const toneValue = document.getElementById("tone-value");

           // HTML → C++ (user interaction)
           gainSlider.addEventListener("input", (e) => {
               const value = parseFloat(e.target.value);
               gainState.setNormalisedValue(value);
               gainValue.textContent = Math.round(value * 100) + "%";
           });

           toneSlider.addEventListener("input", (e) => {
               const value = parseFloat(e.target.value);
               toneState.setNormalisedValue(value);
               toneValue.textContent = Math.round(value * 100) + "%";
           });

           // C++ → HTML (automation, preset recall)
           gainState.valueChangedEvent.addListener((newValue) => {
               gainSlider.value = newValue;
               gainValue.textContent = Math.round(newValue * 100) + "%";
           });

           toneState.valueChangedEvent.addListener((newValue) => {
               toneSlider.value = newValue;
               toneValue.textContent = Math.round(newValue * 100) + "%";
           });

           // Initialize from current parameter values
           gainSlider.value = gainState.getNormalisedValue();
           toneSlider.value = toneState.getNormalisedValue();
           gainValue.textContent = Math.round(gainState.getNormalisedValue() * 100) + "%";
           toneValue.textContent = Math.round(toneState.getNormalisedValue() * 100) + "%";
       </script>
   </body>
   </html>
   ```
3. Add extensive comments explaining:
   - Why `100%` not `100vh` (viewport unit issue)
   - Native feel CSS requirements
   - Bidirectional parameter binding pattern
   - Initialization from current parameter values
4. Add placeholder comments: `// {{PARAMETER_BINDINGS}}`, `// {{CONTROL_HTML}}`

**Expected Output:**
- File: `.claude/skills/ui-mockup/assets/webview-templates/index-template.html` (~150 lines)
- CSS constraints enforced (no viewport units)
- Native feel CSS/JS included
- Bidirectional parameter binding examples

**Verification:**
- Automated: `test -f .claude/skills/ui-mockup/assets/webview-templates/index-template.html && ! grep -q "100vh" .claude/skills/ui-mockup/assets/webview-templates/index-template.html`
- Manual: STOP AND ASK LEX: "Please review `.claude/skills/ui-mockup/assets/webview-templates/index-template.html` to verify it uses `100%` (NOT `100vh`), includes native feel CSS/JS (user-select: none, contextmenu disabled), and demonstrates bidirectional parameter binding (HTML→C++ and C++→HTML). Does this template enforce all critical WebView constraints?"

---

### Task 5: Create CMake Configuration Template

**Description:** Create CMake template for WebView resource embedding via `juce_add_binary_data`.

**Required Reading:**
- `procedures/webview/02-project-setup.md` (all)
- `procedures/webview/07-distribution.md` (all)

**Dependencies:** Task 4 complete

**Implementation Steps:**

1. Create file: `.claude/skills/ui-mockup/assets/webview-templates/CMakeLists-webview-snippet.cmake`
2. Implement binary data configuration:
   ```cmake
   # WebView UI Resources
   # Embed all UI files into plugin binary
   juce_add_binary_data(${PLUGIN_NAME}_UIResources
       SOURCES
           ui/public/index.html
           ui/public/js/juce/index.js
           ui/public/css/style.css
           # Add more resources as needed
   )

   # Link UI resources to plugin
   target_link_libraries(${PLUGIN_NAME}
       PRIVATE
           ${PLUGIN_NAME}_UIResources
           juce::juce_gui_extra  # Required for WebBrowserComponent
   )

   # Enable WebView on supported platforms
   target_compile_definitions(${PLUGIN_NAME}
       PUBLIC
           JUCE_WEB_BROWSER=1
           JUCE_USE_CURL=0
   )

   # Platform-specific WebView configuration
   if(APPLE)
       # macOS/iOS use WebKit (built-in, no additional setup)
   elseif(WIN32)
       # Windows requires WebView2 runtime
       # WebView2 SDK will be automatically downloaded by JUCE
   elseif(UNIX)
       # Linux uses WebKit2GTK (must be installed on system)
       find_package(PkgConfig REQUIRED)
       pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.0)
       target_link_libraries(${PLUGIN_NAME} PRIVATE ${WEBKIT2_LIBRARIES})
       target_include_directories(${PLUGIN_NAME} PRIVATE ${WEBKIT2_INCLUDE_DIRS})
   endif()
   ```
3. Add comments explaining:
   - Binary data embedding pattern
   - Required JUCE modules (juce_gui_extra)
   - Platform-specific requirements
   - How to add new resources
4. Document resource naming convention:
   - `ui/public/index.html` → `BinaryData::index_html`
   - `ui/public/js/juce/index.js` → `BinaryData::juce_index_js`
   - Slashes become underscores, dots become underscores

**Expected Output:**
- File: `.claude/skills/ui-mockup/assets/webview-templates/CMakeLists-webview-snippet.cmake` (~50 lines)
- Binary data embedding pattern
- Platform-specific configuration
- Resource naming documentation

**Verification:**
- Automated: `test -f .claude/skills/ui-mockup/assets/webview-templates/CMakeLists-webview-snippet.cmake && grep -q "juce_add_binary_data" .claude/skills/ui-mockup/assets/webview-templates/CMakeLists-webview-snippet.cmake`
- Manual: STOP AND ASK LEX: "Please review `.claude/skills/ui-mockup/assets/webview-templates/CMakeLists-webview-snippet.cmake` to verify it demonstrates `juce_add_binary_data`, links UI resources, enables WebView, and documents platform-specific requirements. Is the resource naming convention clearly explained?"

---

### Task 6: Update ui-mockup Skill to Use WebView Templates

**Description:** Update ui-mockup skill to reference WebView templates when generating implementation scaffolding (Phase B).

**Required Reading:**
- `procedures/skills/ui-mockup.md` (all)
- `.claude/skills/ui-mockup/SKILL.md` (current implementation)

**Dependencies:** Tasks 1, 2, 3, 4, 5 complete

**Implementation Steps:**

1. Read current ui-mockup skill: `.claude/skills/ui-mockup/SKILL.md`
2. Update Phase B (Implementation Scaffolding) section to reference templates:
   ```markdown
   ### Phase B: Implementation Scaffolding (After Finalization)

   Generate 5 implementation files using WebView templates:

   #### File 3: v[N]-ui.html (Production HTML)
   - Base: `assets/webview-templates/index-template.html`
   - Replace placeholders: {{PLUGIN_NAME}}, {{PARAMETER_BINDINGS}}, {{CONTROL_HTML}}
   - Enforce CSS constraints from `references/ui-design-rules.md`
   - Extract parameter IDs from finalized mockup
   - Generate bidirectional binding code for each parameter

   #### File 4: v[N]-PluginEditor.h (C++ Header)
   - Base: `assets/webview-templates/PluginEditor-webview.h`
   - Replace placeholders: {{RELAY_DECLARATIONS}}, {{ATTACHMENT_DECLARATIONS}}
   - Enforce correct member order (relays → webView → attachments)
   - Generate relay declarations for each parameter:
     - Slider/Knob → `std::unique_ptr<juce::WebSliderRelay> [name]Relay;`
     - Toggle → `std::unique_ptr<juce::WebToggleButtonRelay> [name]Relay;`
     - Dropdown → `std::unique_ptr<juce::WebComboBoxRelay> [name]Relay;`
   - Generate attachment declarations matching relay types

   #### File 5: v[N]-PluginEditor.cpp (C++ Implementation)
   - Base: `assets/webview-templates/PluginEditor-webview.cpp`
   - Replace placeholders: {{RELAY_CREATION}}, {{WEBVIEW_OPTIONS}}, {{ATTACHMENT_CREATION}}, {{RESOURCE_MAPPING}}
   - Generate relay creation code (before WebView)
   - Generate WebView options registration (.withOptionsFrom(*relay))
   - Generate attachment creation code (after WebView)
   - Generate resource provider mappings for all UI files

   #### File 6: v[N]-CMakeLists.txt (WebView Build Config)
   - Base: `assets/webview-templates/CMakeLists-webview-snippet.cmake`
   - List all resources from ui/public/ directory
   - Platform-specific configuration

   #### File 7: v[N]-integration-checklist.md
   - Base: `assets/integration-checklist-template.md`
   - Add WebView-specific steps:
     - Verify member order in PluginEditor.h
     - Test resource provider returns all files
     - Verify parameter binding (automation/preset recall)
     - Test in Debug and Release builds
     - Check for crashes on plugin close
   ```
3. Add section on CSS constraint validation:
   ```markdown
   ### CSS Constraint Validation

   Before finalizing any mockup, validate against `references/ui-design-rules.md`:

   - ❌ REJECT if CSS contains: `100vh`, `100vw`, `100dvh`, `100svh`
   - ✅ REQUIRE: `html, body { height: 100%; }`
   - ✅ REQUIRE: `user-select: none` (native feel)
   - ✅ REQUIRE: Context menu disabled in JavaScript

   If validation fails, regenerate mockup with corrections before presenting to user.
   ```
4. Add section on parameter ID extraction:
   ```markdown
   ### Parameter ID Extraction

   Parse finalized HTML for JUCE parameter bindings:

   ```javascript
   // Extract all parameter IDs from JavaScript code
   const parameterIds = [];

   // Pattern 1: Juce.getSliderState("PARAM_ID")
   const sliderMatches = html.matchAll(/Juce\.getSliderState\("([^"]+)"\)/g);
   for (const match of sliderMatches) {
       parameterIds.push({ id: match[1], type: "slider" });
   }

   // Pattern 2: Juce.getToggleButtonState("PARAM_ID")
   const toggleMatches = html.matchAll(/Juce\.getToggleButtonState\("([^"]+)"\)/g);
   for (const match of toggleMatches) {
       parameterIds.push({ id: match[1], type: "toggle" });
   }

   // Pattern 3: Juce.getComboBoxState("PARAM_ID")
   const comboMatches = html.matchAll(/Juce\.getComboBoxState\("([^"]+)"\)/g);
   for (const match of comboMatches) {
       parameterIds.push({ id: match[1], type: "combo" });
   }
   ```

   Use extracted IDs to generate matching relay/attachment code in C++.
   ```
5. Update decision menu to include WebView validation checkpoint:
   ```markdown
   ## Phase 4.5: Decision Menu (MANDATORY STOP)

   Present decision menu with options:

   1. **Finalize this design** - Generate implementation scaffolding (5 files)
   2. **Iterate further** - Return to Phase A (adjust design)
   3. **Validate WebView constraints** - Check CSS rules compliance
   4. **Save as aesthetic template** - Extract visual system to `.claude/aesthetics/`
   5. **Pause** - Save progress and exit
   ```

**Expected Output:**
- Updated: `.claude/skills/ui-mockup/SKILL.md` with WebView template integration
- CSS constraint validation enforced
- Parameter ID extraction documented
- WebView-specific checklist items added

**Verification:**
- Automated: `grep -q "webview-templates" .claude/skills/ui-mockup/SKILL.md && grep -q "CSS Constraint Validation" .claude/skills/ui-mockup/SKILL.md`
- Manual: STOP AND ASK LEX: "Please review the updated `.claude/skills/ui-mockup/SKILL.md` to verify it references WebView templates, enforces CSS constraints (no viewport units), extracts parameter IDs, and generates correct member order. Does the Phase B section clearly show how to use templates to generate C++ boilerplate?"

---

### Task 7: Test ui-mockup Skill with Simple Plugin

**Description:** Generate a simple test mockup to verify WebView templates work end-to-end.

**Required Reading:**
- Updated `.claude/skills/ui-mockup/SKILL.md`

**Dependencies:** Task 6 complete

**Implementation Steps:**

1. Create test plugin directory: `plugins/TestWebView/`
2. Create minimal creative brief: `plugins/TestWebView/.ideas/creative-brief.md`
   ```markdown
   # TestWebView Creative Brief

   **Plugin Type:** Audio Effect
   **Parameters:** 2 (Gain, Mix)
   **GUI Aesthetic:** Simple modern dark theme
   ```
3. Invoke ui-mockup skill: Use Skill tool with `skill: "ui-mockup"` and prompt:
   ```
   Generate a simple WebView mockup for TestWebView plugin:
   - 2 parameters: Gain (slider), Mix (slider)
   - Simple modern dark theme
   - Fixed size (400x300)

   Complete Phase A (design iteration) then STOP for decision menu.
   ```
4. Verify Phase A outputs:
   - Check: `plugins/TestWebView/.ideas/mockups/v1-ui.yaml` exists
   - Check: `plugins/TestWebView/.ideas/mockups/v1-ui-test.html` exists
   - Check: Decision menu presented (Finalize / Iterate / Validate / Pause)
5. Choose "Finalize this design" from decision menu
6. Verify Phase B outputs:
   - Check: `plugins/TestWebView/.ideas/mockups/v1-ui.html` exists
   - Check: `plugins/TestWebView/.ideas/mockups/v1-PluginEditor.h` exists
   - Check: `plugins/TestWebView/.ideas/mockups/v1-PluginEditor.cpp` exists
   - Check: `plugins/TestWebView/.ideas/mockups/v1-CMakeLists.txt` exists
   - Check: `plugins/TestWebView/.ideas/mockups/v1-integration-checklist.md` exists
7. Validate generated C++ code:
   - Check member order in `v1-PluginEditor.h`: relays → webView → attachments
   - Check relay types match parameter types (both sliders)
   - Check `v1-PluginEditor.cpp` creates relays before WebView
   - Check `v1-PluginEditor.cpp` registers relays in WebView options
   - Check `v1-PluginEditor.cpp` creates attachments after WebView
8. Validate generated HTML:
   - Check CSS does NOT use `100vh` (should use `100%`)
   - Check `user-select: none` present
   - Check context menu disabled in JavaScript
   - Check parameter IDs match: "GAIN", "MIX"
9. Validate CMake configuration:
   - Check `juce_add_binary_data` present
   - Check UI resources listed
   - Check platform-specific WebView config

**Expected Output:**
- Test plugin: `plugins/TestWebView/`
- 7 total files generated (2 in Phase A, 5 in Phase B)
- All files follow WebView templates
- CSS constraints enforced
- Member order correct

**Verification:**
- Automated:
  ```bash
  test -f plugins/TestWebView/.ideas/mockups/v1-ui.html && \
  test -f plugins/TestWebView/.ideas/mockups/v1-PluginEditor.h && \
  test -f plugins/TestWebView/.ideas/mockups/v1-PluginEditor.cpp && \
  ! grep -q "100vh" plugins/TestWebView/.ideas/mockups/v1-ui.html && \
  grep -q "std::unique_ptr<juce::WebSliderRelay> gainRelay" plugins/TestWebView/.ideas/mockups/v1-PluginEditor.h
  ```
- Manual: STOP AND ASK LEX: "Please test the generated WebView mockup in `plugins/TestWebView/.ideas/mockups/`. Open `v1-ui-test.html` in a browser - does it render correctly? Review `v1-PluginEditor.h` - is the member order correct (relays → webView → attachments)? Review `v1-ui.html` - does it use `100%` (NOT `100vh`) and include native feel CSS? Does the generated code look ready to integrate into a real plugin?"

---

## Phase 6b: ui-template-library Skill

**Objective:** Implement aesthetic template system for saving/reusing visual design systems across plugins.

---

### Task 8: Create ui-template-library Skill Structure

**Description:** Create ui-template-library skill directory structure with frontmatter and basic workflow outline.

**Required Reading:**
- `procedures/skills/ui-template-library.md` (all)
- `architecture/12-webview-integration-design.md` (sections: aesthetic template system)

**Dependencies:** Phase 6a complete (Tasks 1-7)

**Implementation Steps:**

1. Create directory: `.claude/skills/ui-template-library/`
2. Create subdirectories:
   - `.claude/skills/ui-template-library/references/`
   - `.claude/skills/ui-template-library/assets/`
3. Create skill file: `.claude/skills/ui-template-library/SKILL.md`
4. Add frontmatter:
   ```yaml
   ---
   name: ui-template-library
   description: Manage aesthetic templates - save visual systems from completed mockups, apply to new plugins with adaptive layouts
   allowed-tools:
     - Read
     - Write
     - Bash
   preconditions:
     - .claude/aesthetics/ directory exists
   ---
   ```
5. Add workflow sections:
   ```markdown
   # ui-template-library Skill

   **Purpose:** Manage aesthetic templates for consistent visual design across plugins.

   ## Core Concept: Aesthetics vs Layouts

   **Aesthetic template** = Visual system (colors, typography, control styling, spacing philosophy, effects)
   **NOT** = Layout grid or parameter count

   **Example:**
   - Aesthetic: "Vintage Hardware" (warm colors, brass knobs, textured background)
   - Applied to 3-parameter plugin: Horizontal layout, 3 brass knobs
   - Applied to 8-parameter plugin: Grid layout 2x4, 8 brass knobs, same colors/styling

   ## Operations

   1. **Save aesthetic** - Extract visual system from completed mockup
   2. **Apply aesthetic** - Generate mockup with aesthetic + appropriate layout for parameter count
   3. **List aesthetics** - Show all saved aesthetics with previews
   4. **Delete aesthetic** - Remove aesthetic from library
   5. **Update aesthetic** - Refine aesthetic from improved mockup

   ## Directory Structure

   ```
   .claude/aesthetics/
   ├── manifest.json                    # Registry of all aesthetics
   └── [aesthetic-id]/
       ├── aesthetic.yaml               # Visual system specification
       ├── preview.html                 # Visual reference (original mockup)
       └── metadata.json                # Name, description, source plugin
   ```

   ## Workflow: Save Aesthetic

   [TO BE IMPLEMENTED IN TASK 9]

   ## Workflow: Apply Aesthetic

   [TO BE IMPLEMENTED IN TASK 10]

   ## Workflow: List Aesthetics

   [TO BE IMPLEMENTED IN TASK 11]

   ## Workflow: Delete Aesthetic

   [TO BE IMPLEMENTED IN TASK 12]
   ```

**Expected Output:**
- Directory: `.claude/skills/ui-template-library/`
- File: `.claude/skills/ui-template-library/SKILL.md` with structure outline
- Subdirectories: `references/`, `assets/`

**Verification:**
- Automated: `test -d .claude/skills/ui-template-library && test -f .claude/skills/ui-template-library/SKILL.md`
- Manual: STOP AND ASK LEX: "Please review `.claude/skills/ui-template-library/SKILL.md` to verify it explains the aesthetic vs layout concept clearly. Does it make sense that aesthetics save visual systems (colors, styling) but NOT layouts (parameter count, grid structure)?"

---

### Task 9: Implement Save Aesthetic Workflow

**Description:** Implement workflow to extract visual system from completed mockup and save to `.claude/aesthetics/`.

**Required Reading:**
- `procedures/skills/ui-template-library.md` (sections: save aesthetic workflow)

**Dependencies:** Task 8 complete

**Implementation Steps:**

1. Update `.claude/skills/ui-template-library/SKILL.md` with save workflow:
   ```markdown
   ## Workflow: Save Aesthetic

   **Input:** Path to finalized mockup HTML
   **Output:** New aesthetic in `.claude/aesthetics/[aesthetic-id]/`

   ### Step 1: Parse Mockup HTML

   Extract visual system components:

   ```javascript
   // Extract CSS custom properties (if present)
   const cssVars = {
       background: getComputedStyle(document.body).getPropertyValue('--bg-color'),
       accent: getComputedStyle(document.body).getPropertyValue('--accent-color'),
       text: getComputedStyle(document.body).getPropertyValue('--text-color'),
       // ...
   };

   // Extract control styling
   const controlStyles = {
       knobType: detectKnobType(),  // rotary, linear, vintage, modern
       sliderStyle: detectSliderStyle(),  // horizontal, vertical, minimal, skeuomorphic
       buttonStyle: detectButtonStyle(),  // toggle, momentary, flat, 3d
       // ...
   };

   // Extract typography
   const typography = {
       heading: getComputedStyle(document.querySelector('h1')).fontFamily,
       body: getComputedStyle(document.body).fontFamily,
       mono: getComputedStyle(document.querySelector('code')).fontFamily,
       // ...
   };

   // Extract spacing philosophy
   const spacing = {
       unit: detectSpacingUnit(),  // 8px, 10px, 12px grid
       density: detectDensity(),  // compact, comfortable, spacious
   };

   // Extract visual effects
   const effects = {
       shadows: detectShadows(),  // none, subtle, dramatic
       borders: detectBorders(),  // none, subtle, prominent
       textures: detectTextures(),  // none, subtle, prominent
   };
   ```

   ### Step 2: Generate Aesthetic YAML

   Create `aesthetic.yaml`:

   ```yaml
   # aesthetic.yaml
   version: 1.0

   colors:
     background: "#2a2a2a"
     backgroundAlt: "#1e1e1e"
     accent: "#ff6b35"
     accentHover: "#ff8555"
     text: "#f0e7d8"
     textSecondary: "#a0a0a0"
     border: "#404040"

   typography:
     heading:
       family: "Bebas Neue, sans-serif"
       weight: 700
       size: "24px"
       letterSpacing: "0.05em"
     body:
       family: "Roboto, sans-serif"
       weight: 400
       size: "14px"
       letterSpacing: "0"
     mono:
       family: "Roboto Mono, monospace"
       weight: 400
       size: "12px"

   controls:
     knob:
       type: rotary-vintage
       size: 60px
       color: "#c8a882"  # brass
       pointerColor: "#ff6b35"
     slider:
       type: vertical-wood
       height: 100px
       width: 30px
       trackColor: "#4a3929"
       thumbColor: "#c8a882"
     button:
       type: toggle-mechanical
       height: 30px
       onColor: "#ff6b35"
       offColor: "#404040"

   spacing:
     unit: 10px
     density: comfortable
     controlGap: 20px
     sectionGap: 40px

   effects:
     shadows:
       level: subtle
       color: "rgba(0, 0, 0, 0.3)"
       blur: 10px
     borders:
       width: 1px
       color: "#404040"
       style: solid
     textures:
       background: "noise-subtle.png"
       opacity: 0.05
   ```

   ### Step 3: Generate Metadata

   Create `metadata.json`:

   ```json
   {
     "id": "vintage-hardware-001",
     "name": "Vintage Hardware",
     "description": "Warm, retro, skeuomorphic design with brass controls and textured background",
     "sourcePlugin": "TapeAge",
     "sourceVersion": "v3",
     "created": "2025-11-10T10:30:00Z",
     "usedInPlugins": ["TapeAge"],
     "tags": ["vintage", "warm", "skeuomorphic", "brass", "retro"]
   }
   ```

   ### Step 4: Copy Preview HTML

   Copy finalized mockup HTML as visual reference:

   ```bash
   cp plugins/$PLUGIN_NAME/.ideas/mockups/v$N-ui.html \
      .claude/aesthetics/$AESTHETIC_ID/preview.html
   ```

   ### Step 5: Update Manifest

   Update `.claude/aesthetics/manifest.json`:

   ```json
   {
     "version": "1.0",
     "aesthetics": [
       {
         "id": "vintage-hardware-001",
         "name": "Vintage Hardware",
         "description": "Warm, retro, skeuomorphic design",
         "sourcePlugin": "TapeAge",
         "created": "2025-11-10T10:30:00Z",
         "tags": ["vintage", "warm", "skeuomorphic"]
       }
     ]
   }
   ```

   ### Step 6: Confirmation

   Display success message with preview path:

   ```
   ✅ Aesthetic saved: "Vintage Hardware" (vintage-hardware-001)

   Preview: .claude/aesthetics/vintage-hardware-001/preview.html

   You can now apply this aesthetic to new plugins using:
   /ui-mockup [PluginName] --aesthetic vintage-hardware-001
   ```
   ```
2. Create aesthetic YAML template: `.claude/skills/ui-template-library/assets/aesthetic-template.yaml`
3. Create metadata template: `.claude/skills/ui-template-library/assets/metadata-template.json`
4. Create manifest initialization: `.claude/skills/ui-template-library/assets/manifest-init.json`
5. Document CSS parsing logic in: `.claude/skills/ui-template-library/references/css-extraction.md`

**Expected Output:**
- Updated: `.claude/skills/ui-template-library/SKILL.md` with save workflow
- Template: `.claude/skills/ui-template-library/assets/aesthetic-template.yaml`
- Template: `.claude/skills/ui-template-library/assets/metadata-template.json`
- Template: `.claude/skills/ui-template-library/assets/manifest-init.json`
- Reference: `.claude/skills/ui-template-library/references/css-extraction.md`

**Verification:**
- Automated: `test -f .claude/skills/ui-template-library/assets/aesthetic-template.yaml && grep -q "colors:" .claude/skills/ui-template-library/assets/aesthetic-template.yaml`
- Manual: STOP AND ASK LEX: "Please review the save aesthetic workflow in `.claude/skills/ui-template-library/SKILL.md`. Does it clearly explain how to extract visual system components (colors, typography, controls, spacing, effects) from HTML? Does the aesthetic.yaml structure make sense for capturing reusable visual design?"

---

### Task 10: Implement Apply Aesthetic Workflow

**Description:** Implement workflow to apply saved aesthetic to new plugin with appropriate layout for parameter count.

**Required Reading:**
- `procedures/skills/ui-template-library.md` (sections: apply aesthetic workflow)

**Dependencies:** Task 9 complete

**Implementation Steps:**

1. Update `.claude/skills/ui-template-library/SKILL.md` with apply workflow:
   ```markdown
   ## Workflow: Apply Aesthetic

   **Input:**
   - Aesthetic ID
   - Target plugin name
   - Parameter count and types

   **Output:** Generated mockup HTML with aesthetic applied + appropriate layout

   ### Step 1: Load Aesthetic

   Read aesthetic.yaml and metadata.json:

   ```javascript
   const aesthetic = YAML.parse(
       fs.readFileSync(`.claude/aesthetics/${aestheticId}/aesthetic.yaml`)
   );
   const metadata = JSON.parse(
       fs.readFileSync(`.claude/aesthetics/${aestheticId}/metadata.json`)
   );
   ```

   ### Step 2: Analyze Target Plugin

   Read parameter-spec.md or creative-brief.md:

   ```javascript
   const parameters = parseParameterSpec(
       `plugins/${pluginName}/.ideas/parameter-spec.md`
   );

   const paramCount = parameters.length;
   const paramTypes = parameters.map(p => p.type);  // slider, toggle, combo
   ```

   ### Step 3: Generate Appropriate Layout

   Choose layout based on parameter count:

   ```javascript
   function chooseLayout(paramCount) {
       if (paramCount <= 3) {
           return 'horizontal';  // Single row
       } else if (paramCount <= 6) {
           return 'grid-2x3';  // 2 rows, up to 3 columns
       } else if (paramCount <= 9) {
           return 'grid-3x3';  // 3x3 grid
       } else {
           return 'scrollable-grid';  // Larger grid with scrolling
       }
   }

   const layout = chooseLayout(paramCount);
   ```

   ### Step 4: Generate HTML with Aesthetic

   Combine aesthetic + layout + parameters:

   ```html
   <!DOCTYPE html>
   <html lang="en">
   <head>
       <meta charset="UTF-8">
       <style>
           /* Apply aesthetic colors */
           :root {
               --bg-color: {{aesthetic.colors.background}};
               --accent-color: {{aesthetic.colors.accent}};
               --text-color: {{aesthetic.colors.text}};
               /* ... more CSS vars from aesthetic */
           }

           html, body {
               margin: 0;
               padding: 0;
               height: 100%;  /* NOT 100vh */
               background: var(--bg-color);
               color: var(--text-color);
               font-family: {{aesthetic.typography.body.family}};
               user-select: none;
           }

           /* Apply layout based on parameter count */
           .controls {
               {{generateLayoutCSS(layout)}}
               gap: {{aesthetic.spacing.controlGap}};
           }

           /* Apply control styling from aesthetic */
           .knob {
               {{generateKnobCSS(aesthetic.controls.knob)}}
           }

           .slider {
               {{generateSliderCSS(aesthetic.controls.slider)}}
           }
       </style>
   </head>
   <body>
       <h1 style="font-family: {{aesthetic.typography.heading.family}}">
           {{pluginName}}
       </h1>

       <div class="controls">
           {{generateControlsHTML(parameters, aesthetic)}}
       </div>
   </body>
   </html>
   ```

   ### Step 5: Save Generated Mockup

   Save to target plugin mockups directory:

   ```bash
   mkdir -p plugins/$PLUGIN_NAME/.ideas/mockups/
   echo "<!-- Generated from aesthetic: $AESTHETIC_ID -->" > \
       plugins/$PLUGIN_NAME/.ideas/mockups/v1-ui-test.html
   cat generated.html >> \
       plugins/$PLUGIN_NAME/.ideas/mockups/v1-ui-test.html
   ```

   ### Step 6: Update Aesthetic Metadata

   Track usage in metadata.json:

   ```json
   {
     "usedInPlugins": ["TapeAge", "NewPlugin"]
   }
   ```

   ### Step 7: Present Decision Menu

   Offer next steps:

   1. **Preview in browser** - Open v1-ui-test.html
   2. **Iterate design** - Adjust colors/styling
   3. **Finalize** - Generate implementation scaffolding
   4. **Choose different aesthetic** - Try another template
   ```
2. Document layout generation logic: `.claude/skills/ui-template-library/references/layout-generation.md`
   - Horizontal layout (1-3 params)
   - Grid 2x3 (4-6 params)
   - Grid 3x3 (7-9 params)
   - Scrollable grid (10+ params)
3. Document control generation: `.claude/skills/ui-template-library/references/control-generation.md`
   - How to apply knob styling
   - How to apply slider styling
   - How to apply button styling
   - Adaptive sizing based on layout density

**Expected Output:**
- Updated: `.claude/skills/ui-template-library/SKILL.md` with apply workflow
- Reference: `.claude/skills/ui-template-library/references/layout-generation.md`
- Reference: `.claude/skills/ui-template-library/references/control-generation.md`

**Verification:**
- Automated: `grep -q "Generate Appropriate Layout" .claude/skills/ui-template-library/SKILL.md`
- Manual: STOP AND ASK LEX: "Please review the apply aesthetic workflow in `.claude/skills/ui-template-library/SKILL.md`. Does it clearly explain how to generate appropriate layouts for different parameter counts (3 params = horizontal, 8 params = grid)? Does the workflow maintain visual consistency (colors, styling) while adapting layout structure?"

---

### Task 11: Implement List and Delete Workflows

**Description:** Implement workflows to list all aesthetics and delete aesthetics from library.

**Required Reading:**
- `procedures/skills/ui-template-library.md` (sections: list/delete workflows)

**Dependencies:** Task 10 complete

**Implementation Steps:**

1. Update `.claude/skills/ui-template-library/SKILL.md` with list workflow:
   ```markdown
   ## Workflow: List Aesthetics

   **Input:** None (or optional filter by tag)
   **Output:** Table of all aesthetics with metadata

   ### Step 1: Read Manifest

   ```javascript
   const manifest = JSON.parse(
       fs.readFileSync('.claude/aesthetics/manifest.json')
   );
   ```

   ### Step 2: Display Table

   ```markdown
   # Aesthetic Library

   | ID | Name | Description | Source Plugin | Created | Used In |
   |----|------|-------------|---------------|---------|---------|
   | vintage-hardware-001 | Vintage Hardware | Warm, retro design | TapeAge | 2025-11-10 | TapeAge, CompCo |
   | modern-minimal-002 | Modern Minimal | Clean, flat design | EQPro | 2025-11-11 | EQPro |
   ```

   ### Step 3: Show Preview Paths

   ```
   Preview files:
   - vintage-hardware-001: .claude/aesthetics/vintage-hardware-001/preview.html
   - modern-minimal-002: .claude/aesthetics/modern-minimal-002/preview.html
   ```

   ### Step 4: Decision Menu

   1. **Apply aesthetic to plugin** - Choose aesthetic and target plugin
   2. **View aesthetic details** - Show full aesthetic.yaml
   3. **Delete aesthetic** - Remove from library
   4. **Exit**
   ```
2. Add delete workflow:
   ```markdown
   ## Workflow: Delete Aesthetic

   **Input:** Aesthetic ID
   **Output:** Aesthetic removed from library

   ### Step 1: Confirm Deletion

   Show usage information:

   ```
   ⚠️  Delete aesthetic "Vintage Hardware" (vintage-hardware-001)?

   Currently used in 2 plugins:
   - TapeAge
   - CompCo

   Deleting will NOT affect existing plugins (they have copies of the UI).
   This only removes the template from the library.

   Are you sure? (yes/no)
   ```

   ### Step 2: Remove Files

   ```bash
   rm -rf .claude/aesthetics/$AESTHETIC_ID
   ```

   ### Step 3: Update Manifest

   Remove entry from manifest.json:

   ```javascript
   manifest.aesthetics = manifest.aesthetics.filter(
       a => a.id !== aestheticId
   );
   fs.writeFileSync(
       '.claude/aesthetics/manifest.json',
       JSON.stringify(manifest, null, 2)
   );
   ```

   ### Step 4: Confirmation

   ```
   ✅ Aesthetic "Vintage Hardware" deleted from library.

   Plugins using this aesthetic are unaffected (they have independent copies).
   ```
   ```

**Expected Output:**
- Updated: `.claude/skills/ui-template-library/SKILL.md` with list/delete workflows

**Verification:**
- Automated: `grep -q "Workflow: List Aesthetics" .claude/skills/ui-template-library/SKILL.md && grep -q "Workflow: Delete Aesthetic" .claude/skills/ui-template-library/SKILL.md`
- Manual: STOP AND ASK LEX: "Please review the list and delete workflows in `.claude/skills/ui-template-library/SKILL.md`. Do they provide clear ways to browse the aesthetic library and safely remove unwanted aesthetics? Does the delete confirmation properly warn about existing plugin usage?"

---

### Task 12: Integrate ui-template-library into ui-mockup

**Description:** Add decision menu options in ui-mockup to save/apply aesthetics at appropriate points.

**Required Reading:**
- Updated `.claude/skills/ui-mockup/SKILL.md`
- Updated `.claude/skills/ui-template-library/SKILL.md`

**Dependencies:** Tasks 6, 11 complete

**Implementation Steps:**

1. Update ui-mockup Phase 0 (plugin context loading) to check for existing aesthetics:
   ```markdown
   ### Phase 0: Check for Aesthetic Library

   Before starting design, check if aesthetics exist:

   ```bash
   if [ -f .claude/aesthetics/manifest.json ]; then
       AESTHETIC_COUNT=$(jq '.aesthetics | length' .claude/aesthetics/manifest.json)
       if [ $AESTHETIC_COUNT -gt 0 ]; then
           echo "Found $AESTHETIC_COUNT saved aesthetics"
       fi
   fi
   ```

   If aesthetics exist, present decision menu:

   1. **Start from aesthetic template** - Choose from library
   2. **Start from scratch** - Custom design
   3. **List all aesthetics** - Browse library first
   ```
2. Update ui-mockup Phase 1 (design iteration) to offer aesthetic application:
   ```markdown
   ### Phase 1: Initial Design

   If user chose "Start from aesthetic template":
   - Invoke ui-template-library skill with "apply" operation
   - Pass plugin name and parameter count
   - Use generated HTML as starting point

   If user chose "Start from scratch":
   - Generate custom design from creative brief
   ```
3. Update ui-mockup Phase 4.5 (decision menu after finalization) to offer aesthetic saving:
   ```markdown
   ## Phase 4.5: Decision Menu (MANDATORY STOP)

   Present decision menu with options:

   1. **Finalize this design** - Generate implementation scaffolding (5 files)
   2. **Save as aesthetic template** - Add to library (invoke ui-template-library "save")
   3. **Finalize AND save aesthetic** - Do both operations
   4. **Iterate further** - Return to Phase A (adjust design)
   5. **Validate WebView constraints** - Check CSS rules compliance
   6. **Pause** - Save progress and exit
   ```
4. Document integration points: `.claude/skills/ui-mockup/references/aesthetic-integration.md`

**Expected Output:**
- Updated: `.claude/skills/ui-mockup/SKILL.md` with aesthetic integration
- Reference: `.claude/skills/ui-mockup/references/aesthetic-integration.md`

**Verification:**
- Automated: `grep -q "Start from aesthetic template" .claude/skills/ui-mockup/SKILL.md && grep -q "Save as aesthetic template" .claude/skills/ui-mockup/SKILL.md`
- Manual: STOP AND ASK LEX: "Please review the aesthetic integration in `.claude/skills/ui-mockup/SKILL.md`. Does it offer aesthetic templates at the right time (before starting design)? Does it offer to save aesthetics at the right time (after finalizing design)? Do the decision menus feel natural and discoverable?"

---

### Task 13: Create .claude/aesthetics/ Directory and Initialize

**Description:** Create aesthetic library directory structure and initialize manifest.

**Required Reading:**
- `procedures/skills/ui-template-library.md` (sections: directory structure)

**Dependencies:** Task 8 complete (skill structure created)

**Implementation Steps:**

1. Create directory: `.claude/aesthetics/`
2. Create initial manifest: `.claude/aesthetics/manifest.json`
   ```json
   {
     "version": "1.0",
     "created": "2025-11-10",
     "aesthetics": []
   }
   ```
3. Create README: `.claude/aesthetics/README.md`
   ```markdown
   # Aesthetic Library

   This directory stores reusable visual design systems (aesthetics) for plugin UIs.

   ## Structure

   Each aesthetic is stored in its own directory:

   ```
   [aesthetic-id]/
   ├── aesthetic.yaml      # Visual system specification
   ├── preview.html        # Visual reference (original mockup)
   └── metadata.json       # Name, description, usage tracking
   ```

   ## Usage

   - **Save aesthetic:** `/ui-template-library save [path-to-mockup]`
   - **Apply aesthetic:** `/ui-mockup [PluginName] --aesthetic [aesthetic-id]`
   - **List aesthetics:** `/ui-template-library list`
   - **Delete aesthetic:** `/ui-template-library delete [aesthetic-id]`

   ## What is an Aesthetic?

   An aesthetic captures the **visual system** (colors, typography, control styling, spacing, effects)
   but NOT the **layout** (parameter count, grid structure).

   This allows applying the same visual design to plugins with different parameter counts.

   **Example:**
   - Aesthetic: "Vintage Hardware" (brass knobs, warm colors, textured background)
   - 3-param plugin: Horizontal layout, 3 brass knobs
   - 8-param plugin: Grid layout 2x4, 8 brass knobs, same visual styling
   ```
4. Add .gitignore entry for aesthetic previews (if desired):
   ```gitignore
   # Optionally ignore preview HTML files (keep YAML and metadata)
   .claude/aesthetics/*/preview.html
   ```

**Expected Output:**
- Directory: `.claude/aesthetics/`
- File: `.claude/aesthetics/manifest.json` (empty array)
- File: `.claude/aesthetics/README.md`

**Verification:**
- Automated: `test -d .claude/aesthetics && test -f .claude/aesthetics/manifest.json && jq '.aesthetics | length' .claude/aesthetics/manifest.json | grep -q "^0$"`
- Manual: STOP AND ASK LEX: "Please verify `.claude/aesthetics/` directory exists with `manifest.json` and `README.md`. Does the README clearly explain what aesthetics are (visual systems, not layouts) and how to use them?"

---

### Task 14: Test ui-template-library with TestWebView Plugin

**Description:** Test aesthetic system by saving TestWebView aesthetic and applying to new test plugin.

**Required Reading:**
- Updated `.claude/skills/ui-template-library/SKILL.md`

**Dependencies:** Tasks 7, 13 complete (TestWebView exists, aesthetics directory initialized)

**Implementation Steps:**

1. Invoke ui-template-library skill to save TestWebView aesthetic:
   - Use Skill tool with `skill: "ui-template-library"`
   - Prompt: "Save aesthetic from plugins/TestWebView/.ideas/mockups/v1-ui.html with name 'Simple Modern Dark'"
2. Verify aesthetic saved:
   - Check: `.claude/aesthetics/simple-modern-dark-001/` directory exists
   - Check: `aesthetic.yaml` contains colors, typography, controls, spacing, effects
   - Check: `metadata.json` contains name, description, sourcePlugin: "TestWebView"
   - Check: `preview.html` is copy of v1-ui.html
   - Check: `manifest.json` has 1 entry
3. Create second test plugin: `plugins/TestWebView2/`
4. Create creative brief with different parameter count:
   ```markdown
   # TestWebView2 Creative Brief

   **Plugin Type:** Audio Effect
   **Parameters:** 4 (Gain, Tone, Mix, Bypass)
   **GUI Aesthetic:** Use "Simple Modern Dark" aesthetic
   ```
5. Invoke ui-mockup skill with aesthetic:
   - Prompt: "Generate mockup for TestWebView2 using aesthetic 'simple-modern-dark-001'"
6. Verify applied aesthetic:
   - Check: Generated mockup uses same colors as TestWebView
   - Check: Layout is different (4 params instead of 2)
   - Check: Control styling matches (same slider appearance)
   - Check: Typography matches (same fonts)
7. List aesthetics:
   - Invoke ui-template-library skill with "list" operation
   - Verify: Shows "Simple Modern Dark" with sourcePlugin "TestWebView", usedInPlugins: ["TestWebView2"]

**Expected Output:**
- Aesthetic saved: `.claude/aesthetics/simple-modern-dark-001/`
- Aesthetic applied to TestWebView2 with appropriate layout for 4 parameters
- Manifest updated with usage tracking

**Verification:**
- Automated:
  ```bash
  test -d .claude/aesthetics/simple-modern-dark-001 && \
  test -f plugins/TestWebView2/.ideas/mockups/v1-ui-test.html && \
  jq '.aesthetics | length' .claude/aesthetics/manifest.json | grep -q "^1$"
  ```
- Manual: STOP AND ASK LEX: "Please compare the two generated mockups:
  - Open `plugins/TestWebView/.ideas/mockups/v1-ui-test.html` (original 2-param design)
  - Open `plugins/TestWebView2/.ideas/mockups/v1-ui-test.html` (4-param with applied aesthetic)

  Do they have the same visual styling (colors, fonts, control appearance) but different layouts (2-param horizontal vs 4-param grid)? Does the aesthetic system successfully separate visual design from layout structure?"

---

## Phase 6c: gui-agent WebView Integration

**Objective:** Update gui-agent to generate WebView-based UIs using finalized mockups and enforcing critical safety patterns.

---

### Task 15: Update gui-agent with WebView Capability Check

**Description:** Add Phase 0 check in gui-agent to detect if finalized WebView mockup exists.

**Required Reading:**
- `procedures/agents/gui-agent.md` (all)
- `.claude/agents/gui-agent.md` (current implementation)

**Dependencies:** Phase 6a, 6b complete

**Implementation Steps:**

1. Read current gui-agent: `.claude/agents/gui-agent.md`
2. Add WebView detection logic before main implementation:
   ```markdown
   ## Phase 0: Determine UI Type (WebView or Native JUCE)

   **Check for finalized WebView mockup:**

   ```bash
   # Look for finalized mockup files
   WEBVIEW_MOCKUP_DIR="plugins/$PLUGIN_NAME/.ideas/mockups/"

   # Find highest version number with implementation files
   LATEST_VERSION=$(find "$WEBVIEW_MOCKUP_DIR" -name "v*-ui.html" | \
       sed 's/.*v\([0-9]*\)-.*/\1/' | sort -n | tail -1)

   if [ -n "$LATEST_VERSION" ]; then
       # Check if all implementation files exist
       if [ -f "${WEBVIEW_MOCKUP_DIR}v${LATEST_VERSION}-ui.html" ] && \
          [ -f "${WEBVIEW_MOCKUP_DIR}v${LATEST_VERSION}-PluginEditor.h" ] && \
          [ -f "${WEBVIEW_MOCKUP_DIR}v${LATEST_VERSION}-PluginEditor.cpp" ]; then
           UI_TYPE="webview"
           WEBVIEW_VERSION="$LATEST_VERSION"
       fi
   fi

   # Default to native JUCE if no WebView mockup found
   if [ -z "$UI_TYPE" ]; then
       UI_TYPE="native-juce"
   fi
   ```

   **Decision:**
   - If `UI_TYPE="webview"`: Use finalized WebView mockup files (Workflow A)
   - If `UI_TYPE="native-juce"`: Generate native JUCE components (Workflow B)
   ```
3. Add workflow routing:
   ```markdown
   ## Workflow Routing

   Based on Phase 0 detection:

   ### Workflow A: WebView Implementation (if mockup exists)

   [TO BE IMPLEMENTED IN TASK 16]

   ### Workflow B: Native JUCE Implementation (fallback)

   [Existing native JUCE workflow remains unchanged]
   ```

**Expected Output:**
- Updated: `.claude/agents/gui-agent.md` with WebView detection logic
- Workflow routing structure in place

**Verification:**
- Automated: `grep -q "Determine UI Type" .claude/agents/gui-agent.md && grep -q "Workflow A: WebView Implementation" .claude/agents/gui-agent.md`
- Manual: STOP AND ASK LEX: "Please review the WebView detection logic in `.claude/agents/gui-agent.md`. Does it correctly check for finalized mockup files (v[N]-ui.html, v[N]-PluginEditor.h, v[N]-PluginEditor.cpp)? Does the workflow routing make sense (WebView if mockup exists, native JUCE otherwise)?"

---

### Task 16: Implement gui-agent WebView Workflow

**Description:** Implement complete WebView integration workflow in gui-agent (Workflow A).

**Required Reading:**
- `procedures/agents/gui-agent.md` (sections: WebView integration)
- `procedures/webview/04-parameter-binding.md` (all)
- `procedures/webview/best-practices.md` (all)

**Dependencies:** Task 15 complete

**Implementation Steps:**

1. Add Workflow A to gui-agent:
   ```markdown
   ### Workflow A: WebView Implementation

   **Preconditions:**
   - Finalized WebView mockup exists (v[N]-ui.html, v[N]-PluginEditor.h, v[N]-PluginEditor.cpp)
   - parameter-spec.md exists (from Stage 3)

   #### Step 1: Validate Mockup Consistency

   **Critical check:** Verify parameter IDs in mockup match parameter-spec.md

   ```python
   # Extract parameter IDs from HTML
   html_param_ids = extract_parameter_ids_from_html("v{N}-ui.html")

   # Extract parameter IDs from parameter-spec.md
   spec_param_ids = parse_parameter_spec("parameter-spec.md")

   # Compare
   missing_in_html = spec_param_ids - html_param_ids
   extra_in_html = html_param_ids - spec_param_ids

   if missing_in_html or extra_in_html:
       report_error("Parameter ID mismatch between mockup and spec")
   ```

   #### Step 2: Copy UI Files to Plugin Directory

   ```bash
   # Create ui/public/ directory structure
   mkdir -p plugins/$PLUGIN_NAME/Source/ui/public/js/juce/

   # Copy finalized HTML
   cp plugins/$PLUGIN_NAME/.ideas/mockups/v${VERSION}-ui.html \
      plugins/$PLUGIN_NAME/Source/ui/public/index.html

   # Copy JUCE JavaScript bridge (from JUCE modules)
   # This is provided by JUCE, extract from JUCE installation
   cp $JUCE_PATH/modules/juce_gui_extra/native/juce.js \
      plugins/$PLUGIN_NAME/Source/ui/public/js/juce/index.js
   ```

   #### Step 3: Update PluginEditor Files

   **DO NOT copy mockup C++ files directly** - they are templates with placeholders.
   Instead, generate from templates with parameter-specific code:

   ```cpp
   // PluginEditor.h
   #pragma once
   #include <JuceHeader.h>
   #include "PluginProcessor.h"

   class PluginEditor : public juce::AudioProcessorEditor, private juce::Timer
   {
   public:
       PluginEditor(PluginProcessor& p);
       ~PluginEditor() override;

       void paint(juce::Graphics&) override;
       void resized() override;

   private:
       void timerCallback() override;

       std::optional<juce::WebBrowserComponent::Resource> getResource(
           const juce::String& url
       );

       PluginProcessor& audioProcessor;

       // ⚠️ CRITICAL ORDER: Relays → WebView → Attachments

       // 1. Relays (from parameter-spec.md)
       {{GENERATE_RELAY_DECLARATIONS}}

       // 2. WebView
       std::unique_ptr<juce::WebBrowserComponent> webView;

       // 3. Parameter Attachments (from parameter-spec.md)
       {{GENERATE_ATTACHMENT_DECLARATIONS}}

       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
   };
   ```

   **Generate relay declarations from parameter-spec.md:**

   ```python
   for param in parameters:
       if param.type in ["float", "int"]:  # Slider/Knob
           print(f"std::unique_ptr<juce::WebSliderRelay> {param.id.lower()}Relay;")
       elif param.type == "bool":  # Toggle
           print(f"std::unique_ptr<juce::WebToggleButtonRelay> {param.id.lower()}Relay;")
       elif param.type == "choice":  # Dropdown
           print(f"std::unique_ptr<juce::WebComboBoxRelay> {param.id.lower()}Relay;")
   ```

   **Generate attachment declarations:**

   ```python
   for param in parameters:
       if param.type in ["float", "int"]:
           print(f"std::unique_ptr<juce::WebSliderParameterAttachment> {param.id.lower()}Attachment;")
       elif param.type == "bool":
           print(f"std::unique_ptr<juce::WebToggleButtonParameterAttachment> {param.id.lower()}Attachment;")
       elif param.type == "choice":
           print(f"std::unique_ptr<juce::WebComboBoxParameterAttachment> {param.id.lower()}Attachment;")
   ```

   #### Step 4: Implement PluginEditor.cpp

   ```cpp
   PluginEditor::PluginEditor(PluginProcessor& p)
       : AudioProcessorEditor(p), audioProcessor(p)
   {
       // 1. CREATE RELAYS FIRST
       {{GENERATE_RELAY_CREATION}}

       // 2. CREATE WEBVIEW with relay options
       webView = std::make_unique<juce::WebBrowserComponent>(
           juce::WebBrowserComponent::Options{}
               .withNativeIntegrationEnabled()
               {{GENERATE_RELAY_OPTIONS}}
               .withResourceProvider([this](const auto& url) {
                   return getResource(url);
               })
       );

       // 3. CREATE PARAMETER ATTACHMENTS
       {{GENERATE_ATTACHMENT_CREATION}}

       // Navigate to root (loads index.html)
       webView->goToURL("https://dummy.invalid/");

       addAndMakeVisible(*webView);

       // Start timer for real-time updates (60ms ≈ 16 FPS)
       startTimerHz(16);

       {{DETERMINE_SIZE_FROM_MOCKUP}}
       setSize(width, height);
   }
   ```

   **Generate relay creation:**

   ```python
   for param in parameters:
       if param.type in ["float", "int"]:
           print(f'{param.id.lower()}Relay = std::make_unique<juce::WebSliderRelay>("{param.id}");')
       elif param.type == "bool":
           print(f'{param.id.lower()}Relay = std::make_unique<juce::WebToggleButtonRelay>("{param.id}");')
       elif param.type == "choice":
           print(f'{param.id.lower()}Relay = std::make_unique<juce::WebComboBoxRelay>("{param.id}");')
   ```

   **Generate relay options registration:**

   ```python
   for param in parameters:
       print(f'.withOptionsFrom(*{param.id.lower()}Relay)')
   ```

   **Generate attachment creation:**

   ```python
   for param in parameters:
       attachment_type = get_attachment_type(param.type)
       print(f'{param.id.lower()}Attachment = std::make_unique<{attachment_type}>(')
       print(f'    *audioProcessor.apvts.getParameter("{param.id}"),')
       print(f'    *{param.id.lower()}Relay')
       print(f');')
   ```

   #### Step 5: Implement Resource Provider

   ```cpp
   std::optional<juce::WebBrowserComponent::Resource>
   PluginEditor::getResource(const juce::String& url)
   {
       // Map URLs to embedded binary data
       if (url == "/" || url == "/index.html") {
           return juce::WebBrowserComponent::Resource {
               BinaryData::index_html,
               BinaryData::index_htmlSize,
               "text/html"
           };
       }

       if (url == "/js/juce/index.js") {
           return juce::WebBrowserComponent::Resource {
               BinaryData::juce_index_js,
               BinaryData::juce_index_jsSize,
               "text/javascript"
           };
       }

       // Add more resources as needed from ui/public/ directory

       return std::nullopt;
   }
   ```

   #### Step 6: Update CMakeLists.txt

   Add WebView configuration to existing CMakeLists.txt:

   ```cmake
   # WebView UI Resources
   juce_add_binary_data(${PRODUCT_NAME}_UIResources
       SOURCES
           Source/ui/public/index.html
           Source/ui/public/js/juce/index.js
   )

   target_link_libraries(${PRODUCT_NAME}
       PRIVATE
           ${PRODUCT_NAME}_UIResources
           juce::juce_gui_extra  # Required for WebBrowserComponent
   )

   # Enable WebView
   target_compile_definitions(${PRODUCT_NAME}
       PUBLIC
           JUCE_WEB_BROWSER=1
           JUCE_USE_CURL=0
   )
   ```

   #### Step 7: Verify Implementation

   **Automated checks:**

   - [ ] Member order correct in PluginEditor.h (relays → webView → attachments)
   - [ ] All parameter IDs from spec have matching relays
   - [ ] All relays registered in WebView options
   - [ ] All parameters have matching attachments
   - [ ] Resource provider maps all UI files
   - [ ] CMakeLists.txt includes juce_add_binary_data
   - [ ] No CSS contains viewport units (100vh, 100vw)
   - [ ] HTML includes native feel CSS (user-select: none)

   **Report success:**

   ```json
   {
     "agent": "gui-agent",
     "status": "success",
     "ui_type": "webview",
     "mockup_version": 3,
     "outputs": {
       "editor_header": "plugins/PluginName/Source/PluginEditor.h",
       "editor_impl": "plugins/PluginName/Source/PluginEditor.cpp",
       "ui_files": "plugins/PluginName/Source/ui/public/",
       "cmake_updated": true
     },
     "parameters_bound": ["GAIN", "TONE", "MIX"],
     "relay_count": 3,
     "attachment_count": 3,
     "issues": [],
     "ready_for_next_stage": true
   }
   ```
   ```
2. Add error handling for common WebView issues:
   ```markdown
   ### Common Issues and Resolutions

   **Issue 1: Parameter ID mismatch**
   - Symptom: HTML references parameter IDs not in spec
   - Resolution: Regenerate mockup with correct IDs, or update spec to match mockup

   **Issue 2: Wrong member order**
   - Symptom: Generated PluginEditor.h has attachments before relays
   - Resolution: Enforce order: relays → webView → attachments

   **Issue 3: CSS viewport units**
   - Symptom: HTML uses 100vh
   - Resolution: Replace with 100%, add html/body height: 100%

   **Issue 4: Missing resource mappings**
   - Symptom: WebView blank on load
   - Resolution: Verify all files in ui/public/ are mapped in getResource()

   **Issue 5: Relay not registered in WebView options**
   - Symptom: Parameter doesn't respond to UI interaction
   - Resolution: Add .withOptionsFrom(*relay) for missing relay
   ```

**Expected Output:**
- Updated: `.claude/agents/gui-agent.md` with complete WebView workflow
- Parameter ID extraction logic
- Relay/attachment generation from spec
- Resource provider implementation
- CMake integration
- Verification checklist

**Verification:**
- Automated: `grep -q "Validate Mockup Consistency" .claude/agents/gui-agent.md && grep -q "CRITICAL ORDER" .claude/agents/gui-agent.md`
- Manual: STOP AND ASK LEX: "Please review the WebView workflow in `.claude/agents/gui-agent.md`. Does it correctly:
  1. Validate parameter ID consistency between mockup and spec?
  2. Generate relays/attachments from parameter-spec.md?
  3. Enforce correct member declaration order?
  4. Implement resource provider pattern?
  5. Update CMakeLists.txt with binary data?

  Does the workflow feel complete enough to generate a working WebView UI from a finalized mockup?"

---

### Task 17: Update foundation-agent CMakeLists.txt for WebView Support

**Description:** Update foundation-agent to include WebView-ready CMakeLists.txt configuration when needed.

**Required Reading:**
- `procedures/agents/foundation-agent.md` (sections: CMakeLists.txt generation)
- `procedures/webview/02-project-setup.md` (all)

**Dependencies:** Task 16 complete

**Implementation Steps:**

1. Read current foundation-agent: `.claude/agents/foundation-agent.md`
2. Update CMakeLists.txt generation to include juce_gui_extra module:
   ```cmake
   # Update JUCE module linking
   target_link_libraries(${PRODUCT_NAME}
       PRIVATE
           juce::juce_audio_utils
           juce::juce_audio_processors
           juce::juce_dsp
           juce::juce_gui_extra  # Required for WebBrowserComponent
       PUBLIC
           juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
   )
   ```
3. Add WebView-ready compile definitions (can be enabled later):
   ```cmake
   # WebView support (disabled by default, enabled by gui-agent if WebView UI used)
   # target_compile_definitions(${PRODUCT_NAME}
   #     PUBLIC
   #         JUCE_WEB_BROWSER=1
   #         JUCE_USE_CURL=0
   # )
   ```
4. Add commented placeholder for binary data (will be uncommented by gui-agent):
   ```cmake
   # WebView UI Resources (uncommented by gui-agent if WebView UI used)
   # juce_add_binary_data(${PRODUCT_NAME}_UIResources
   #     SOURCES
   #         Source/ui/public/index.html
   #         Source/ui/public/js/juce/index.js
   # )
   #
   # target_link_libraries(${PRODUCT_NAME}
   #     PRIVATE
   #         ${PRODUCT_NAME}_UIResources
   # )
   ```
5. Document that gui-agent will uncomment these sections if WebView UI is used

**Expected Output:**
- Updated: `.claude/agents/foundation-agent.md` with WebView-ready CMakeLists.txt

**Verification:**
- Automated: `grep -q "juce::juce_gui_extra" .claude/agents/foundation-agent.md`
- Manual: STOP AND ASK LEX: "Please review the updated CMakeLists.txt in `.claude/agents/foundation-agent.md`. Does it include juce_gui_extra module? Are the WebView sections properly commented out (to be enabled later by gui-agent)? Will this allow seamless WebView integration in Stage 5?"

---

### Task 18: Test Complete WebView Workflow End-to-End

**Description:** Test full workflow from mockup generation through plugin implementation with WebView UI.

**Required Reading:**
- All Phase 6 documentation

**Dependencies:** All Phase 6 tasks complete (Tasks 1-17)

**Implementation Steps:**

1. Create new test plugin: `plugins/WebViewComplete/`
2. Generate creative brief:
   ```markdown
   # WebViewComplete Creative Brief

   **Plugin Type:** Audio Effect
   **Parameters:** 3 (Drive, Tone, Mix)
   **GUI Aesthetic:** Modern dark theme with gradient accents
   **Purpose:** Test complete WebView workflow from mockup to working plugin
   ```
3. Run ui-mockup skill:
   - Generate design (Phase A) with modern dark aesthetic
   - Finalize design
   - Generate implementation files (Phase B)
   - Verify all 7 files created
4. Run plugin-workflow skill:
   - Stage 0: Research (create architecture.md)
   - Stage 1: Planning (create plan.md)
   - Stage 2: Foundation (foundation-agent creates project structure)
   - Stage 3: Shell (shell-agent implements APVTS with DRIVE, TONE, MIX parameters)
   - Stage 4: DSP (dsp-agent implements simple drive/tone/mix processing)
   - Stage 5: GUI (gui-agent detects WebView mockup and integrates it)
5. Verify Stage 5 (gui-agent) WebView integration:
   - Check: `plugins/WebViewComplete/Source/ui/public/index.html` exists
   - Check: `PluginEditor.h` has correct member order (relays → webView → attachments)
   - Check: `PluginEditor.cpp` creates relays before WebView
   - Check: `PluginEditor.cpp` registers all relays in WebView options
   - Check: `PluginEditor.cpp` creates attachments after WebView
   - Check: `PluginEditor.cpp` implements resource provider
   - Check: `CMakeLists.txt` includes `juce_add_binary_data`
   - Check: All parameter IDs match between spec and implementation
6. Build plugin:
   - Run: `scripts/build-and-install.sh plugins/WebViewComplete/`
   - Verify: Build succeeds without errors
   - Verify: VST3 and AU installed
7. Test in DAW:
   - Load plugin in DAW
   - Verify: WebView UI renders correctly (not blank)
   - Verify: All 3 parameters visible and interactive
   - Verify: Parameter changes affect audio (drive/tone/mix work)
   - Verify: Automation works (parameter binding C++→HTML)
   - Verify: Preset recall works (parameter binding C++→HTML)
   - Test in Debug build: No crashes
   - Test in Release build: No crashes (member order test)
8. Verify CSS constraints enforced:
   - Check: HTML does NOT use `100vh` (uses `100%`)
   - Check: HTML includes `user-select: none`
   - Check: Context menu disabled
   - Verify: UI renders correctly on first load (no blank screen)

**Expected Output:**
- Working plugin: `plugins/WebViewComplete/`
- WebView UI integrated correctly
- All parameters bound and functional
- No crashes in Debug or Release builds
- CSS constraints enforced

**Verification:**
- Automated:
  ```bash
  # Verify WebView files exist
  test -f plugins/WebViewComplete/Source/ui/public/index.html && \
  test -f plugins/WebViewComplete/Source/PluginEditor.h && \
  test -f plugins/WebViewComplete/Source/PluginEditor.cpp && \

  # Verify member order correct
  grep -A 10 "CRITICAL ORDER" plugins/WebViewComplete/Source/PluginEditor.h | \
      grep -q "std::unique_ptr<juce::WebSliderRelay>" && \

  # Verify CSS constraints
  ! grep -q "100vh" plugins/WebViewComplete/Source/ui/public/index.html && \

  # Verify CMake integration
  grep -q "juce_add_binary_data" plugins/WebViewComplete/CMakeLists.txt && \

  # Verify build succeeded
  test -f ~/Library/Audio/Plug-Ins/VST3/WebViewComplete.vst3
  ```
- Manual: STOP AND ASK LEX: "Please test the WebViewComplete plugin thoroughly:

  1. **Build Test:**
     - Did the build complete without errors?
     - Are VST3 and AU installed?

  2. **UI Render Test:**
     - Load plugin in DAW
     - Does the WebView UI appear immediately (not blank)?
     - Are all 3 parameters (Drive, Tone, Mix) visible?

  3. **Parameter Binding Test:**
     - Move UI controls - does audio change?
     - Automate a parameter in DAW - does UI update?
     - Save/recall preset - does UI update?

  4. **Stability Test:**
     - Close plugin - does DAW crash?
     - Test both Debug and Release builds
     - Any crashes on plugin close indicates member order problem

  5. **CSS Constraints Test:**
     - Does UI render correctly on first load (no blank screen)?
     - Can you select text in the UI (should NOT be able to)?
     - Does right-click show context menu (should NOT)?

  If ALL tests pass, Phase 6 WebView system is complete and working correctly."

---

## Comprehensive Verification Criteria

### Automated Tests

```bash
# Test 1: Verify all Phase 6 files exist
test -f .claude/skills/ui-mockup/references/ui-design-rules.md && \
test -d .claude/skills/ui-mockup/assets/webview-templates/ && \
test -f .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h && \
test -f .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.cpp && \
test -f .claude/skills/ui-mockup/assets/webview-templates/index-template.html && \
test -f .claude/skills/ui-mockup/assets/webview-templates/CMakeLists-webview-snippet.cmake && \
test -d .claude/skills/ui-template-library/ && \
test -f .claude/skills/ui-template-library/SKILL.md && \
test -d .claude/aesthetics/ && \
test -f .claude/aesthetics/manifest.json && \
echo "✅ All Phase 6 files exist"

# Test 2: Verify CSS constraints enforced in templates
! grep -q "100vh" .claude/skills/ui-mockup/assets/webview-templates/index-template.html && \
grep -q "user-select: none" .claude/skills/ui-mockup/assets/webview-templates/index-template.html && \
echo "✅ CSS constraints enforced in templates"

# Test 3: Verify member order in templates
grep -A 20 "CRITICAL ORDER" .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h | \
    grep -q "std::unique_ptr<juce::WebSliderRelay>" && \
grep -A 20 "CRITICAL ORDER" .claude/skills/ui-mockup/assets/webview-templates/PluginEditor-webview.h | \
    tail -5 | grep -q "std::unique_ptr<juce::WebBrowserComponent>" && \
echo "✅ Correct member order in templates"

# Test 4: Verify ui-mockup references templates
grep -q "webview-templates" .claude/skills/ui-mockup/SKILL.md && \
grep -q "CSS Constraint Validation" .claude/skills/ui-mockup/SKILL.md && \
echo "✅ ui-mockup references templates"

# Test 5: Verify ui-template-library structure
grep -q "Aesthetics vs Layouts" .claude/skills/ui-template-library/SKILL.md && \
test -f .claude/skills/ui-template-library/assets/aesthetic-template.yaml && \
echo "✅ ui-template-library structure complete"

# Test 6: Verify gui-agent WebView integration
grep -q "Determine UI Type" .claude/agents/gui-agent.md && \
grep -q "Workflow A: WebView Implementation" .claude/agents/gui-agent.md && \
grep -q "Validate Mockup Consistency" .claude/agents/gui-agent.md && \
echo "✅ gui-agent WebView integration implemented"

# Test 7: Test TestWebView mockup generation
test -f plugins/TestWebView/.ideas/mockups/v1-ui.html && \
test -f plugins/TestWebView/.ideas/mockups/v1-PluginEditor.h && \
test -f plugins/TestWebView/.ideas/mockups/v1-PluginEditor.cpp && \
! grep -q "100vh" plugins/TestWebView/.ideas/mockups/v1-ui.html && \
echo "✅ TestWebView mockup generated correctly"

# Test 8: Test aesthetic system
test -d .claude/aesthetics/simple-modern-dark-001/ && \
test -f .claude/aesthetics/simple-modern-dark-001/aesthetic.yaml && \
test -f plugins/TestWebView2/.ideas/mockups/v1-ui-test.html && \
echo "✅ Aesthetic system working"

# Test 9: Test complete WebView workflow (WebViewComplete plugin)
test -f plugins/WebViewComplete/Source/ui/public/index.html && \
test -f ~/Library/Audio/Plug-Ins/VST3/WebViewComplete.vst3 && \
echo "✅ Complete WebView workflow successful"
```

### Manual Verification Checklist

**Phase 6a: ui-mockup skill completion**
- [ ] ui-design-rules.md exists and documents CSS constraints (no viewport units)
- [ ] ui-design-rules.md documents native feel requirements
- [ ] PluginEditor-webview.h template has correct member order (relays → webView → attachments)
- [ ] PluginEditor-webview.h template includes extensive comments explaining safety
- [ ] PluginEditor-webview.cpp template creates relays before WebView
- [ ] PluginEditor-webview.cpp template implements resource provider pattern
- [ ] index-template.html uses `100%` (NOT `100vh`) for heights
- [ ] index-template.html includes native feel CSS (user-select: none, no context menu)
- [ ] index-template.html demonstrates bidirectional parameter binding
- [ ] CMakeLists-webview-snippet.cmake demonstrates `juce_add_binary_data`
- [ ] ui-mockup skill references WebView templates in Phase B
- [ ] ui-mockup skill enforces CSS constraint validation
- [ ] TestWebView mockup generated with 7 files (2 in Phase A, 5 in Phase B)
- [ ] TestWebView mockup HTML uses correct CSS constraints
- [ ] TestWebView mockup C++ has correct member order

**Phase 6b: ui-template-library skill**
- [ ] ui-template-library skill structure created
- [ ] Aesthetic vs layout concept clearly explained
- [ ] Save aesthetic workflow implemented
- [ ] Apply aesthetic workflow implemented with layout generation
- [ ] List aesthetics workflow implemented
- [ ] Delete aesthetics workflow implemented
- [ ] ui-mockup integrates aesthetic save/apply in decision menus
- [ ] .claude/aesthetics/ directory created with manifest.json
- [ ] Aesthetic saved from TestWebView (simple-modern-dark-001)
- [ ] Aesthetic applied to TestWebView2 with different parameter count
- [ ] Applied aesthetic maintains visual consistency with adapted layout

**Phase 6c: gui-agent WebView integration**
- [ ] gui-agent Phase 0 detects finalized WebView mockup
- [ ] gui-agent Workflow A validates parameter ID consistency
- [ ] gui-agent Workflow A generates relays/attachments from spec
- [ ] gui-agent Workflow A enforces correct member order
- [ ] gui-agent Workflow A implements resource provider
- [ ] gui-agent Workflow A updates CMakeLists.txt with binary data
- [ ] foundation-agent CMakeLists.txt includes juce_gui_extra
- [ ] WebViewComplete plugin generated end-to-end
- [ ] WebViewComplete builds without errors
- [ ] WebViewComplete UI renders correctly in DAW (not blank)
- [ ] WebViewComplete parameters bind correctly (C++↔HTML)
- [ ] WebViewComplete automation works
- [ ] WebViewComplete preset recall works
- [ ] WebViewComplete stable in Debug build
- [ ] WebViewComplete stable in Release build (no crashes on close)

**Integration Tests**
- [ ] Can generate mockup from scratch (custom design)
- [ ] Can generate mockup from aesthetic template
- [ ] Can save aesthetic from finalized mockup
- [ ] Can apply aesthetic to different parameter count (layout adapts)
- [ ] gui-agent correctly uses WebView mockup when present
- [ ] gui-agent falls back to native JUCE when no mockup
- [ ] Complete workflow: /dream → /implement with WebView UI works
- [ ] WebView plugins follow all critical safety patterns
- [ ] No viewport units in any generated HTML
- [ ] All generated C++ has correct member order

### Success Criteria

Phase 6 is COMPLETE when:

1. ✅ ui-mockup skill generates WebView mockups with two-phase workflow (design iteration → finalization)
2. ✅ ui-design-rules.md enforces CSS constraints (no viewport units, native feel)
3. ✅ WebView code templates exist with CORRECT member order
4. ✅ ui-template-library skill can save/apply aesthetics
5. ✅ Aesthetic system separates visual design from layout structure
6. ✅ gui-agent detects WebView mockups and integrates them correctly
7. ✅ gui-agent enforces parameter ID consistency between mockup and spec
8. ✅ gui-agent generates correct member order (relays → webView → attachments)
9. ✅ Complete workflow works: mockup generation → plugin implementation → working WebView UI
10. ✅ Test plugins (TestWebView, TestWebView2, WebViewComplete) all work correctly
11. ✅ WebView plugins stable in Debug and Release builds
12. ✅ All CSS constraints enforced (no 100vh, native feel CSS present)
13. ✅ Parameter binding works bidirectionally (C++↔HTML)
14. ✅ Automation and preset recall work correctly
15. ✅ Decision menus guide users through mockup workflow naturally

## Potential Issues & Mitigations

### Issue 1: Member Declaration Order Violations

**Symptom:** Plugin crashes on close in Release builds (but works in Debug)

**Root Cause:** Incorrect member order causes undefined behavior during destruction

**Mitigation:**
- Templates enforce correct order via structure
- gui-agent validates member order after generation
- Comments in templates explain why order matters
- validator subagent can check member order as additional layer

**Prevention:** ALWAYS enforce relays → webView → attachments order

---

### Issue 2: CSS Viewport Units Causing Blank UI

**Symptom:** WebView blank on first load, works after resizing window

**Root Cause:** JUCE WebView doesn't initialize viewport units until first resize event

**Mitigation:**
- ui-design-rules.md documents this constraint
- ui-mockup validates CSS before finalization (rejects viewport units)
- Templates use `100%` not `100vh`

**Prevention:** Automated CSS validation catches viewport units before finalization

---

### Issue 3: Parameter ID Mismatch

**Symptom:** Parameters don't respond to UI interaction

**Root Cause:** HTML uses parameter IDs that don't match APVTS parameter IDs

**Mitigation:**
- gui-agent validates parameter ID consistency (Step 1 of Workflow A)
- Error reported if mismatch detected
- User must regenerate mockup or update spec to fix

**Prevention:** Validation step blocks implementation until IDs match

---

### Issue 4: Missing Resource Mappings

**Symptom:** WebView blank or missing resources (JS, CSS, images)

**Root Cause:** Resource provider doesn't map all files in ui/public/

**Mitigation:**
- gui-agent generates complete resource provider mapping
- All files in ui/public/ automatically mapped
- build-and-install.sh verifies binary data embedded

**Prevention:** Automated resource discovery ensures all files mapped

---

### Issue 5: Native Feel Not Enforced

**Symptom:** UI feels like a web page (text selection, context menu)

**Root Cause:** CSS/JS doesn't disable web page behaviors

**Mitigation:**
- ui-design-rules.md documents native feel requirements
- Templates include native feel CSS/JS
- ui-mockup validates native feel patterns

**Prevention:** Templates enforce native feel by default

---

## Notes for Next Phase (Phase 7)

**Phase 7 will build on Phase 6 WebView system:**

1. **design-sync skill** - Validate mockup ↔ creative brief consistency
   - Catch design drift early
   - Ensure UI matches original vision
   - Uses Phase 6 aesthetic system for comparison

2. **Advanced aesthetic features** - Export/import aesthetics
   - Share aesthetics between projects
   - Version control for aesthetics
   - Aesthetic marketplace potential

3. **WebView debugging tools** - Enhanced development workflow
   - Live reload during mockup iteration
   - Browser devtools integration
   - Parameter binding debugger

4. **Accessibility features** - Make WebView UIs accessible
   - Keyboard navigation
   - Screen reader support
   - High contrast modes

**Prerequisites for Phase 7:**
- Phase 6 complete and verified
- WebViewComplete test plugin working
- Aesthetic system tested and stable
- gui-agent WebView integration battle-tested

**Handoff to Phase 7:**
- All WebView patterns documented and working
- Critical safety patterns enforced (member order, CSS constraints)
- Aesthetic template system operational
- Complete mockup → implementation workflow proven

---

## Meta

**Document Version:** 1.0
**Phase:** 6 (WebView UI System)
**Sub-Phases:** 3 (6a, 6b, 6c)
**Total Tasks:** 18
**Estimated Completion:** 6-8 hours

**Architecture Documents Referenced:**
- architecture/12-webview-integration-design.md (primary)
- architecture/02-core-abstractions.md (ui-mockup, ui-template-library)
- architecture/04-component-architecture.md (WebView components)
- architecture/10-extension-architecture.md (skill integration)

**Procedure Files Referenced:**
- procedures/webview/README.md (START HERE)
- procedures/webview/01-overview.md through 07-distribution.md
- procedures/webview/best-practices.md (CRITICAL)
- procedures/webview/common-problems.md
- procedures/skills/ui-mockup.md
- procedures/skills/ui-template-library.md
- procedures/agents/gui-agent.md

**Based On:** ROADMAP.md Phase 6 specification

**Last Updated:** 2025-11-10
**Next Steps:** Begin Phase 6a implementation (Task 1)
