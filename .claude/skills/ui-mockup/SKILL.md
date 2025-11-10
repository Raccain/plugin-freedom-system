---
name: ui-mockup
description: Generate production-ready WebView UI mockups in two phases - design iteration (2 files) then implementation scaffolding (5 files after approval)
allowed-tools:
  - Read
  - Write
preconditions:
  - None (can work standalone or with creative brief)
---

# ui-mockup Skill

**Purpose:** Generate production-ready WebView UIs in two phases. The HTML generated IS the plugin UI, not a throwaway prototype.

## Workflow Overview

**TWO-PHASE WORKFLOW:**

### Phase A: Design Iteration (Fast)
Generate 2 design files for rapid iteration:
1. **v[N]-ui.yaml** - Machine-readable design specification
2. **v[N]-ui-test.html** - Browser-testable mockup (no JUCE required)

**STOP HERE** - Present decision menu for user to iterate or finalize.

### Phase B: Implementation Scaffolding (After Finalization)
Generate 5 implementation files only after user approves design:
3. **v[N]-ui.html** - Production HTML (copy-paste to plugin)
4. **v[N]-PluginEditor.h** - C++ header boilerplate
5. **v[N]-PluginEditor.cpp** - C++ implementation boilerplate
6. **v[N]-CMakeLists.txt** - WebView build configuration
7. **v[N]-integration-checklist.md** - Implementation steps

**Why two phases?** HTML mockups are cheap to iterate. C++ boilerplate is pointless if design isn't locked. This saves time by avoiding premature scaffolding generation.

All files saved to: `plugins/[PluginName]/.ideas/mockups/`

## Phase 1: Load Context

**Check for existing documentation:**

```bash
test -f "plugins/$PLUGIN_NAME/.ideas/creative-brief.md"
find "plugins/$PLUGIN_NAME/.ideas/improvements/" -name "*.md"
test -d "plugins/$PLUGIN_NAME/Source/"  # For redesigns
```

**Extract from creative-brief.md:**

- Plugin type (compressor, EQ, reverb, synth, utility)
- Parameters (names, types, ranges, defaults)
- UI vision (layout preferences, visual style)
- Colors (brand colors, dark/light theme)
- Special elements (visualizers, waveforms, custom graphics)

**See:** `references/context-extraction.md` for detailed extraction guidelines

## Phase 1.5: Free-Form UI Vision

**Prompt user:**

```
What should the UI look like?

Describe your vision - layout, colors, style, special elements. I'll ask follow-ups for anything missing.
```

**Why this phase exists:** Users often have clear visions but struggle with structured questions. Free-form capture lets them express ideas naturally.

**Listen for:**

- Layout preferences ("controls on left, visualizer on right")
- Visual references ("like FabFilter Pro-C", "vintage analog gear")
- Mood/feel ("minimal and clean", "skeuomorphic wood panels")
- Special requests ("animated VU meter", "resizable window")

**Capture verbatim notes before moving to targeted questions.**

## Phase 2: Targeted Design Questions

**Ask only about gaps not covered in Phase 1.5.** One question at a time, wait for answers.

**Question categories:**

1. **Window Size** - If not mentioned (default: 600x400 for effects)
2. **Layout Organization** - If not described (grid, vertical/horizontal sections, custom)
3. **Control Style** - If not specified (rotary knobs, sliders, buttons)
4. **Color Scheme** - If not provided (dark, light, custom)
5. **Special Elements** - If mentioned but vague (clarify implementation details)

**See:** `references/design-questions.md` for detailed question templates and defaults

## Phase 3: Generate Hierarchical YAML

**Create:** `plugins/[Name]/.ideas/mockups/v[N]-ui.yaml`

**Purpose:** Machine-readable design spec that guides HTML generation and C++ implementation.

**Structure:**

```yaml
window:
  width: 600
  height: 400
  resizable: false

colors:
  background: "#2b2b2b"
  primary: "#4a9eff"
  text: "#ffffff"

layout:
  type: grid  # or: vertical-sections, horizontal-sections, custom
  sections:
    - id: header
      height: 60
      controls: [title]
    - id: main
      flex: 1
      controls: [threshold, ratio, attack, release, gain]

controls:
  - id: threshold
    type: rotary-knob
    label: "Threshold"
    parameter: "threshold"
    range: [-60.0, 0.0]
    unit: "dB"
    default: -20.0
    position: {x: 50, y: 100}
```

**See:** `assets/ui-yaml-template.yaml` for complete template structure

## Phase 4: Generate Browser Test HTML

**Create:** `plugins/[Name]/.ideas/mockups/v[N]-ui-test.html`

**Purpose:** Test UI in browser for rapid design iteration.

**Features:**

- Standalone HTML file (open directly in browser)
- Mock parameter state (simulates C++ backend)
- Interactive controls (test bindings)
- Console logging (verify parameter messages)
- Same visual as production will be
- No JUCE/WebView required

**See:** `references/browser-testing.md` for testing guidelines

---

## ⚠️ CRITICAL STOP POINT - Phase 4.5: Design Decision Menu

**DO NOT PROCEED TO PHASE 5 WITHOUT USER CONFIRMATION**

After generating YAML + test HTML, present this decision menu:

```
✓ Mockup v[N] design created (2 files)

Files generated:
- v[N]-ui.yaml (design specification)
- v[N]-ui-test.html (browser-testable mockup)

What do you think?
1. Provide refinements (iterate on design) ← Creates v[N+1]
2. Finalize and create implementation files (recommended if satisfied)
3. Test in browser (open v[N]-ui-test.html)
4. Other

Choose (1-4): _
```

**WAIT for user response before continuing.**

**Option handling:**
- **Option 1**: User gives feedback → Return to Phase 3 with new version number (v2, v3, etc.)
- **Option 2**: User approves → Proceed to Phase 5-8 (generate remaining 5 files)
- **Option 3**: Offer to open test HTML in browser for interactive review
- **Option 4**: Handle custom request

**Only execute Phases 5-8 if user chose option 2 (finalize).**

---

## Phase 5: Generate Production HTML (After Finalization Only)

**Prerequisites:** User confirmed design in Phase 4.5 decision menu.

**Create:** `plugins/[Name]/.ideas/mockups/v[N]-ui.html`

**This HTML IS the plugin UI.** Copy-paste into `Source/ui/index.html` during Stage 5 (GUI).

**Key features:**

- **Parameter bindings**: `data-param="threshold"` attributes for C++ communication
- **Value displays**: Synchronized with parameter changes
- **Responsive layout**: Adapts to window size (if resizable)
- **WebView API**: Uses `window.juce.postMessage()` for parameter updates
- **No external dependencies**: Pure HTML/CSS/JS, no npm packages

**See:** `references/html-generation.md` for detailed generation rules

## Phase 6: Generate C++ Boilerplate (After Finalization Only)

**Prerequisites:** User confirmed design in Phase 4.5 decision menu.

**Create:**
- `plugins/[Name]/.ideas/mockups/v[N]-PluginEditor.h`
- `plugins/[Name]/.ideas/mockups/v[N]-PluginEditor.cpp`

**Purpose:** WebView integration boilerplate for Stage 5 (GUI).

**Key components:**

- `juce::WebBrowserComponent` setup
- Parameter binding (C++ ↔ JavaScript)
- Message handlers (`window.juce.postMessage()` receivers)
- Value updates (push changes to UI)
- Window sizing and resizing logic

**See:** `assets/webview-boilerplate.md` for template code

## Phase 7: Generate Build Configuration (After Finalization Only)

**Prerequisites:** User confirmed design in Phase 4.5 decision menu.

**Create:** `plugins/[Name]/.ideas/mockups/v[N]-CMakeLists.txt`

**Purpose:** CMake snippet to enable WebView support in JUCE.

**Key settings:**

```cmake
# Enable WebView
juce_add_plugin(${PROJECT_NAME}
    NEEDS_WEB_BROWSER TRUE
    # ... other settings
)

# Bundle HTML resources
juce_add_binary_data(${PROJECT_NAME}Assets
    SOURCES
        Source/ui/index.html
        Source/ui/style.css
        Source/ui/script.js
)
```

**See:** `references/cmake-configuration.md` for complete examples

## Phase 8: Generate Integration Checklist (After Finalization Only)

**Prerequisites:** User confirmed design in Phase 4.5 decision menu.

**Create:** `plugins/[Name]/.ideas/mockups/v[N]-integration-checklist.md`

**Purpose:** Step-by-step guide to integrate UI into plugin during Stage 5.

**Steps:**

1. Copy HTML to `Source/ui/index.html`
2. Update `PluginEditor.h` with WebView component
3. Update `PluginEditor.cpp` with message handlers
4. Update `CMakeLists.txt` with WebView settings
5. Build and test standalone
6. Test in DAW
7. Troubleshooting common issues

**See:** `assets/integration-checklist-template.md` for full template

## Phase 9: Finalize parameter-spec.md (After Finalization Only)

**Prerequisites:** User confirmed design in Phase 4.5 decision menu AND this is the first mockup version.

**If this is the first UI mockup (v1):**

**Create:** `plugins/[Name]/.ideas/parameter-spec.md`

**Purpose:** Lock parameter specification for implementation. This becomes the **immutable contract** for all subsequent stages.

**Extract from YAML:**

```markdown
## Total Parameter Count

**Total:** 5 parameters

## Parameter Definitions

### threshold
- **Type:** Float
- **Range:** -60.0 to 0.0 dB
- **Default:** -20.0
- **Skew Factor:** linear
- **UI Control:** Rotary knob, center-top position
- **DSP Usage:** Compressor threshold level
```

**See:** `assets/parameter-spec-template.md`

## After Completing All Phases

Once user has finalized a design and all 7 files are generated, present this menu:

```
✓ Mockup v[N] complete (7 files generated)

What's next?
1. Start implementation (invoke plugin-workflow)
2. Create another UI version (explore alternative design)
3. Test in browser (open v[N]-ui-test.html)
4. Other

Choose (1-4): _
```

## Versioning Strategy

**v1, v2, v3...** Each UI version is saved separately.

**Why multiple versions:**

- Explore different layouts without losing previous work
- A/B test designs in browser before committing
- Iterate based on user feedback
- Keep design history

**File naming:**

```
plugins/[Name]/.ideas/mockups/
├── v1-ui.yaml
├── v1-ui.html
├── v1-browser-test.html
├── v1-PluginEditor.h
├── v1-PluginEditor.cpp
├── v1-CMakeLists.txt
├── v1-integration-checklist.md
├── v2-ui.yaml  (if user wants alternative design)
├── v2-ui.html
└── ... (v2 variants)
```

**Latest version is used for Stage 5 implementation** (unless user specifies different version).

## Success Criteria

**Design phase successful when:**
- ✅ YAML spec generated matching user requirements
- ✅ Browser test HTML works (interactive controls, parameter messages)
- ✅ User presented with Phase 4.5 decision menu
- ✅ Design approved OR user iterates with refinements

**Implementation phase successful when (after finalization):**
- ✅ All 7 files generated and saved to `.ideas/mockups/`
- ✅ Production HTML is complete (no placeholder content)
- ✅ C++ boilerplate matches YAML structure (correct parameter bindings)
- ✅ parameter-spec.md generated and locked (for v1 only)

## Integration Points

**Invoked by:**

- `/dream` command → After creative brief, before parameter finalization
- `plugin-workflow` skill → During Stage 0 (UI design phase)
- `plugin-improve` skill → When redesigning existing plugin UI
- Natural language: "Design UI for [PluginName]", "Create mockup for compressor"

**Invokes:**

- (None - terminal skill that generates files only)

**Creates:**

- `plugins/[Name]/.ideas/mockups/v[N]-*.{yaml,html,h,cpp,txt,md}` (7 files)
- `plugins/[Name]/.ideas/parameter-spec.md` (if v1 and doesn't exist)

**Updates:**

- `PLUGINS.md` → Mark UI designed (if part of workflow)
- `.continue-here.md` → Update workflow state (if part of workflow)

**Blocks:**

- Stage 1 (Planning) → Cannot proceed without parameter-spec.md
- Stage 5 (GUI) → Cannot implement without approved UI mockup

## Reference Documentation

- **Context extraction:** `references/context-extraction.md` - Guidelines for loading plugin context
- **Design questions:** `references/design-questions.md` - Targeted question templates and defaults
- **HTML generation:** `references/html-generation.md` - Rules for production HTML generation
- **Browser testing:** `references/browser-testing.md` - Browser test workflow
- **CMake configuration:** `references/cmake-configuration.md` - WebView build settings

## Template Assets

- **UI YAML template:** `assets/ui-yaml-template.yaml` - Complete YAML structure
- **WebView boilerplate:** `assets/webview-boilerplate.md` - C++ integration templates
- **Integration checklist:** `assets/integration-checklist-template.md` - Step-by-step integration guide
- **Parameter spec template:** `assets/parameter-spec-template.md` - Parameter specification format
