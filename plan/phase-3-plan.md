# Phase 3 Implementation Plan

## Prerequisites Status

**Previous Phase(s) Required:** Phase 0, Phase 1, Phase 2

**Deliverables Check:**

**Phase 0 (Foundation & Contracts):**
- ✅ `.claude/` directory structure exists
- ✅ `.claude/commands/` directory exists (11 commands)
- ✅ `.claude/skills/` directory exists (7 skills)
- ✅ `.claude/agents/` directory exists (validator present)
- ✅ `.claude/hooks/` directory exists
- ✅ `CLAUDE.md` exists with navigation index
- ✅ `PLUGINS.md` exists with state machine
- ⚠️ Contract templates: Need verification in skill assets/

**Phase 1 (Discovery System):**
- ✅ All 9 slash commands present in `.claude/commands/`
- ✅ Core skills exist (plugin-ideation, plugin-workflow, plugin-improve, context-resume, ui-mockup, plugin-testing, plugin-lifecycle)
- ⚠️ Skills have SKILL.md with frontmatter: Need verification
- ⚠️ Interactive decision system implementation: Need verification in skills

**Phase 2 (Workflow Engine):**
- ✅ plugin-workflow skill exists
- ⚠️ Stage 0 implementation: Need verification
- ⚠️ Stage 1 implementation: Need verification
- ⚠️ Stage 6 implementation: Need verification
- ⚠️ State management (PLUGINS.md updates): Need verification
- ⚠️ Git commit integration: Need verification
- ⚠️ .continue-here.md handoff system: Need verification

**Status:** **Partial** - Core structure exists, implementation completeness needs verification

---

## ⚠️ PREREQUISITE WARNING ⚠️

**Prerequisites PARTIAL. Phase 2 must be verified/completed first.**

**Unclear/Missing:**
- Contract templates in skill assets/ directories
- Stage 0, 1, 6 implementation completeness in plugin-workflow
- State management and handoff file system operational status

**This plan assumes prerequisites will be completed. Verify Phase 2 before beginning Phase 3 implementation.**

---

## Phase Overview

Phase 3 implements the **4 implementation subagents** (foundation-agent, shell-agent, dsp-agent, gui-agent) and integrates them with the dispatcher pattern in plugin-workflow. This enables autonomous code generation with fresh context per stage, eliminating context accumulation while maintaining contract enforcement and zero-drift validation.

**Why Phase 3 is Critical:**
- Transforms plugin-workflow from planning-only to full code generation
- Enables single-session plugin creation (concept → installed plugin)
- Provides fresh context per stage (prevents accumulation bugs)
- Establishes hybrid validation system (deterministic hooks + semantic validator)

---

## Required Documentation

### Architecture Files (MUST READ IN ENTIRETY)

- **@architecture/00-PHILOSOPHY.md** - Discovery through play, progressive disclosure, risk-free exploration via checkpointing
- **@architecture/01-executive-summary.md** - Core philosophy, subagent architecture, contract-driven development, hybrid validation
- **@architecture/02-core-abstractions.md** - Subagent interfaces, contract specifications, hybrid validation strategy (Layer 1: hooks 2s deterministic, Layer 2: validator 60s semantic)
- **@architecture/03-model-selection-extended-thinking-strategy.md** - Opus for dsp-agent complexity 4-5, Sonnet for foundation/shell/gui, extended thinking usage
- **@architecture/04-component-architecture.md** - Subagent lifecycle, dispatcher flow, hybrid validation implementation, validator prerequisites
- **@architecture/05-routing-architecture.md** - Slash commands, interactive decision system, inline numbered menus, vagueness detection
- **@architecture/06-state-architecture.md** - PLUGINS.md state machine (🚧 protection), .continue-here.md handoff format, version state, logs/backups
- **@architecture/07-communication-architecture.md** - Dispatcher ↔ subagent messaging, JSON report parsing with robust error handling, error propagation (never auto-retry)
- **@architecture/09-file-system-design.md** - Directory structure, contract locations, plugins/[Name]/.ideas/ organization
- **@architecture/10-extension-architecture.md** - Hook system (6 of 9 used), conditional hook execution pattern (exit 0 graceful skip)
- **@architecture/11-build-automation-architecture.md** - build-and-install.sh 7-phase pipeline, flags (--no-install, --dry-run), Ninja usage
- **@architecture/13-error-handling-recovery.md** - 8 failure modes, recovery procedures, PRODUCT_NAME extraction fallback, validation failure override logging

### Procedure Files (MUST READ IN ENTIRETY)

**Agents:**
- **@procedures/agents/troubleshooter.md** - 4-level graduated research protocol, stop rules, integration with deep-research skill

**Skills:**
- **@procedures/skills/plugin-workflow.md** - Complete 7-stage workflow, complexity scoring, stage boundary protocol, handoff system, git workflow, calls to other skills
- **@procedures/skills/juce-foundation.md** - API verification, real-time safety rules (processBlock constraints), parameter management, DSP constraints, WebView rules (JUCE 8.0.9+), platform considerations
- **@procedures/skills/build-automation.md** - 4-option failure protocol (investigate, show code, show output, wait), build logging, error categories
- **@procedures/skills/plugin-testing.md** - 5 automated tests, build+pluginval mode, manual testing checklist

**Core:**
- **@procedures/core/checkpoint-system.md** - Hard/soft/decision checkpoints, .continue-here.md format (YAML + markdown), exploration pattern (Esc+Esc rewind)
- **@procedures/core/interactive-decision-system.md** - Inline numbered menus (no AskUserQuestion), progressive disclosure, context-aware generation, keyword shortcuts

**WebView:**
- **@procedures/webview/README.md** - ⚠️ START HERE for WebView integration
- **@procedures/webview/best-practices.md** - ⚠️ CRITICAL: Member declaration order (Relays → WebView → Attachments prevents 90% crashes)
- **@procedures/webview/04-parameter-binding.md** - Relay pattern (WebSliderRelay, WebToggleButtonRelay, WebComboBoxRelay)
- **@procedures/webview/03-communication-patterns.md** - C++ ↔ JavaScript bidirectional communication

---

## Implementation Sequence

### Phase 3a: Foundation Subagent (Stage 2)

#### Task 1: Create foundation-agent.md File

**Description**: Create the foundation-agent subagent definition with frontmatter and complete implementation instructions.

**Required Reading:**
- @architecture/02-core-abstractions.md (lines 74-109: Subagent abstraction, interfaces, JSON report format)
- @architecture/03-model-selection-extended-thinking-strategy.md (lines 21-35: Sonnet for foundation-agent, no extended thinking)
- @procedures/skills/juce-foundation.md (all: API verification, platform considerations, CMakeLists.txt patterns)
- @architecture/11-build-automation-architecture.md (all: build-and-install.sh usage, --no-install flag)

**Dependencies**: None (first subagent)

**Implementation Steps:**

1. Create file `.claude/agents/foundation-agent.md`
2. Add YAML frontmatter:
   ```yaml
   ---
   name: foundation-agent
   type: agent
   model: sonnet
   description: Create JUCE plugin project structure and verify compilation (Stage 2)
   tools:
     - Read
     - Write
     - Bash
     - mcp__context7__resolve-library-id
     - mcp__context7__get-library-docs
   preconditions:
     - creative-brief.md exists
     - architecture.md exists (from Stage 0)
     - plan.md exists (from Stage 1)
   ---
   ```
3. Write implementation instructions:
   - Task: Create CMakeLists.txt with JUCE 8 configuration
   - Input contracts: architecture.md (plugin type), plan.md (complexity), creative-brief.md (PRODUCT_NAME)
   - Create Source/PluginProcessor.{h,cpp} with empty AudioProcessor subclass
   - Create Source/PluginEditor.{h,cpp} with empty AudioProcessorEditor subclass
   - Configure build system (Ninja generator)
   - Invoke build-automation with --no-install flag
   - Self-validation: Verify compilation succeeded
   - Return JSON report format (exactly as specified in architecture/02-core-abstractions.md lines 93-101)

**Expected Output:**
- `.claude/agents/foundation-agent.md` exists
- Frontmatter specifies Sonnet model, correct tools
- Implementation instructions reference all required contracts
- JSON report format matches specification exactly

**Verification:**
- Automated: File exists at correct path
- Automated: YAML frontmatter validates (name, type, model, tools, preconditions present)
- Manual: STOP AND ASK LEX: "Please review `.claude/agents/foundation-agent.md` and confirm: (1) All required contracts are referenced in implementation instructions, (2) JSON report format matches architecture specification, (3) Self-validation steps are clear"

---

#### Task 2: Create shell-agent.md File

**Description**: Create the shell-agent subagent definition for Stage 3 (parameter implementation).

**Required Reading:**
- @architecture/02-core-abstractions.md (lines 111-183: Contract enforcement, parameter-spec.md format)
- @procedures/skills/juce-foundation.md (lines 82-108: Parameter management rules, APVTS creation)
- @architecture/04-component-architecture.md (lines 206-252: Parameter validation pattern)

**Dependencies**: Task 1 complete (understand subagent pattern)

**Implementation Steps:**

1. Create file `.claude/agents/shell-agent.md`
2. Add YAML frontmatter with Sonnet model, tools: Read, Edit, Write, Bash, Context7
3. Write implementation instructions:
   - Task: Implement ALL parameters from parameter-spec.md
   - Contract enforcement: Block if parameter-spec.md missing
   - Create APVTS with juce::ParameterID { "id", 1 } format (JUCE 8 requirement)
   - Implement processBlock stub (pass-through audio, juce::ScopedNoDenormals)
   - Create empty editor with placeholder text
   - Build with build-automation (full install)
   - Self-validation: Verify all parameter IDs from spec present in code, compilation succeeded
   - Return JSON with parameter list

**Expected Output:**
- `.claude/agents/shell-agent.md` exists with complete implementation instructions
- Contract validation specified (parameter-spec.md must exist)
- JUCE 8 ParameterID format enforced
- JSON report includes parameters array

**Verification:**
- Automated: File exists, frontmatter validates
- Manual: STOP AND ASK LEX: "Please review `.claude/agents/shell-agent.md` and confirm: (1) Parameter contract enforcement is explicit (blocks if parameter-spec.md missing), (2) JUCE 8 ParameterID format is specified, (3) Self-validation checks all parameter IDs from spec are present in code"

---

#### Task 3: Create dsp-agent.md File

**Description**: Create the dsp-agent subagent definition for Stage 4 (audio processing implementation).

**Required Reading:**
- @architecture/03-model-selection-extended-thinking-strategy.md (lines 9-20: Opus for complexity 4-5, Sonnet for simpler)
- @procedures/skills/juce-foundation.md (lines 40-78: Real-time audio safety rules, DSP constraints)
- @architecture/04-component-architecture.md (lines 254-296: DSP component validation pattern)
- @procedures/skills/plugin-workflow.md (lines 82-99: Stage 4 complexity adaptation, phased implementation)

**Dependencies**: Task 2 complete (understand contract validation)

**Implementation Steps:**

1. Create file `.claude/agents/dsp-agent.md`
2. Add YAML frontmatter:
   - Conditional model: Opus if complexity ≥4, else Sonnet
   - Extended thinking: Conditional (complexity ≥4)
   - Tools: Read, Edit, Write, Bash, Context7
3. Write implementation instructions:
   - Task: Implement audio processing from architecture.md
   - Contract inputs: architecture.md (DSP components), parameter-spec.md (parameter usage), plan.md (phase breakdown if complex)
   - Phased implementation if complexity ≥3:
     - Phase 4.1: Core processing components
     - Phase 4.2: Modulation systems
     - Phase 4.3: Advanced features
     - Git commit after each phase
   - Real-time safety enforcement:
     - NO allocations in processBlock()
     - Use juce::ScopedNoDenormals
     - Preallocate buffers in prepareToPlay()
     - Lock-free parameter access
   - Self-validation: Check all components from architecture.md implemented, no allocations in processBlock, compilation + test load succeeded
   - Return JSON with phases_completed array

**Expected Output:**
- `.claude/agents/dsp-agent.md` exists with conditional model selection
- Phased implementation logic specified
- Real-time safety rules explicitly enforced
- Extended thinking enabled for complex DSP

**Verification:**
- Automated: File exists, frontmatter validates, model selection conditional on complexity
- Manual: STOP AND ASK LEX: "Please review `.claude/agents/dsp-agent.md` and confirm: (1) Conditional model selection (Opus for complexity ≥4) is specified, (2) Real-time safety rules are explicit (no allocations, ScopedNoDenormals, preallocate buffers), (3) Phased implementation logic for complexity ≥3 is clear with git commits per phase"

---

#### Task 4: Create gui-agent.md File

**Description**: Create the gui-agent subagent definition for Stage 5 (WebView UI integration).

**Required Reading:**
- @procedures/webview/README.md (all: WebView integration overview)
- @procedures/webview/best-practices.md (all: ⚠️ CRITICAL member declaration order)
- @procedures/webview/04-parameter-binding.md (all: Relay pattern implementation)
- @architecture/04-component-architecture.md (lines 298-319: UI binding validation pattern)

**Dependencies**: Task 3 complete (understand phased implementation)

**Implementation Steps:**

1. Create file `.claude/agents/gui-agent.md`
2. Add YAML frontmatter with Sonnet model (no extended thinking needed)
3. Write implementation instructions:
   - Task: Integrate WebView UI from finalized mockup
   - Contract inputs: v[N]-ui.html (finalized mockup), parameter-spec.md (parameter IDs)
   - Copy v[N]-ui.html to ui/public/index.html
   - ⚠️ CRITICAL: Member declaration order in PluginEditor.h:
     ```cpp
     private:
         juce::WebSliderRelay relay1;              // 1. Relays first
         juce::WebBrowserComponent webView;         // 2. WebView second
         juce::WebSliderParameterAttachment attachment1; // 3. Attachments last
     ```
   - Create one WebSliderRelay per parameter
   - Register relays with `.withOptionsFrom(relay)`
   - Create one WebSliderParameterAttachment per parameter
   - Update CMakeLists.txt: NEEDS_WEB_BROWSER TRUE, juce_add_binary_data for ui/public/
   - Build with build-automation (full install)
   - Self-validation: All parameter IDs from spec have relay + attachment, member order correct, compilation succeeded
   - Return JSON with bindings array

**Expected Output:**
- `.claude/agents/gui-agent.md` exists with WebView integration instructions
- Member declaration order explicitly specified and marked CRITICAL
- Relay pattern implementation detailed
- CMake configuration updates specified

**Verification:**
- Automated: File exists, frontmatter validates
- Manual: STOP AND ASK LEX: "Please review `.claude/agents/gui-agent.md` and confirm: (1) Member declaration order (Relays → WebView → Attachments) is explicitly specified and marked CRITICAL, (2) Relay pattern is correct (one relay + one attachment per parameter), (3) CMake updates for WebView are specified (NEEDS_WEB_BROWSER TRUE, juce_add_binary_data)"

---

### Phase 3b: Dispatcher Integration (plugin-workflow Updates)

#### Task 5: Update plugin-workflow Stage 2 Dispatcher

**Description**: Replace Stage 2 stub with foundation-agent dispatch logic in plugin-workflow skill.

**Required Reading:**
- @architecture/07-communication-architecture.md (lines 9-101: Dispatcher → subagent communication, JSON parsing with error handling)
- @architecture/04-component-architecture.md (lines 409-487: Complete workflow lifecycle example)
- @procedures/skills/plugin-workflow.md (lines 54-68: Stage 2 Foundation specification)

**Dependencies**: Task 1 complete (foundation-agent exists)

**Implementation Steps:**

1. Read `.claude/skills/plugin-workflow/SKILL.md`
2. Locate Stage 2 implementation section
3. Replace stub with dispatcher logic:
   - Prepare inputs: plugin name, contracts (creative-brief.md, architecture.md, plan.md), CMakeLists.txt template
   - Invoke Task tool:
     ```
     Task(
       subagent_type="foundation-agent",
       description="Implement build system for [PluginName]",
       prompt=`[Complete specs + contracts]`
     )
     ```
   - Implement robust JSON parsing (extract from markdown code blocks, fallback to brace matching, validate required fields)
   - Handle parse errors: Present to user with recovery options (retry, manual inspection, report bug)
   - If status="success": Continue to validation
   - If status="failure": Present 4-option menu (investigate, show code, show output, wait)
4. Update stage completion:
   - Git commit: `feat: [Plugin] Stage 2 - foundation`
   - Update PLUGINS.md: 🚧 Stage 2 complete
   - Update .continue-here.md handoff
   - Present decision menu: Continue to Stage 3 | Review Stage 2 code | Test current state | Pause here | Other

**Expected Output:**
- Stage 2 dispatcher logic complete in plugin-workflow
- JSON parsing includes error handling
- 4-option failure menu implemented
- Git workflow integration complete

**Verification:**
- Automated: Grep for "Task(" in Stage 2 section, verify foundation-agent invocation
- Automated: Grep for JSON parsing error handling code
- Manual: STOP AND ASK LEX: "Please review Stage 2 dispatcher implementation in `.claude/skills/plugin-workflow/SKILL.md` and confirm: (1) Task tool invocation with complete contracts, (2) Robust JSON parsing with error handling (markdown extraction, brace matching, validation), (3) 4-option failure menu (investigate, show code, show output, wait), (4) Git commit + state update + handoff update + decision menu"

---

#### Task 6: Update plugin-workflow Stage 3 Dispatcher

**Description**: Replace Stage 3 stub with shell-agent dispatch logic.

**Required Reading:**
- @architecture/02-core-abstractions.md (lines 111-154: Parameter contract validation)
- @procedures/skills/plugin-workflow.md (lines 70-81: Stage 3 Shell specification)

**Dependencies**: Task 5 complete (understand dispatcher pattern), Task 2 complete (shell-agent exists)

**Implementation Steps:**

1. Edit `.claude/skills/plugin-workflow/SKILL.md` Stage 3 section
2. Add precondition check: Verify parameter-spec.md exists (from finalized mockup), BLOCK if missing with explanation
3. Implement dispatcher logic similar to Stage 2:
   - Prepare inputs: plugin name, parameter-spec.md (contract), PluginProcessor files from Stage 2
   - Invoke shell-agent via Task tool
   - Parse JSON report with error handling
   - Handle failures with 4-option menu
4. Update stage completion:
   - Git commit: `feat: [Plugin] Stage 3 - shell`
   - Update PLUGINS.md: 🚧 Stage 3 complete
   - Update .continue-here.md
   - Decision menu

**Expected Output:**
- Stage 3 dispatcher with parameter-spec.md precondition enforcement
- Same error handling and workflow integration as Stage 2

**Verification:**
- Automated: Grep for parameter-spec.md existence check, verify blocks if missing
- Manual: STOP AND ASK LEX: "Please review Stage 3 dispatcher in plugin-workflow and confirm: (1) Precondition check for parameter-spec.md with blocking error if missing, (2) Dispatcher pattern matches Stage 2 (Task invocation, JSON parsing, error handling), (3) Git + state + handoff + decision menu complete"

---

#### Task 7: Update plugin-workflow Stage 4 Dispatcher

**Description**: Replace Stage 4 stub with dsp-agent dispatch logic, including phased implementation support.

**Required Reading:**
- @procedures/skills/plugin-workflow.md (lines 82-99: Stage 4 DSP with complexity adaptation)
- @architecture/03-model-selection-extended-thinking-strategy.md (lines 102-118: Conditional extended thinking for DSP)

**Dependencies**: Task 6 complete, Task 3 complete (dsp-agent exists)

**Implementation Steps:**

1. Edit `.claude/skills/plugin-workflow/SKILL.md` Stage 4 section
2. Implement complexity-aware dispatcher:
   - Read complexity score from plan.md
   - If complexity ≥3: Phased implementation
     - Parse phase breakdown from plan.md
     - For each phase (4.1, 4.2, 4.3...):
       - Invoke dsp-agent with current phase context
       - Parse JSON, handle errors
       - Git commit per phase: `feat: [Plugin] Stage 4.1 - [phase description]`
       - Update plan.md with phase completion timestamp
       - Decision menu: Continue to next phase | Review current phase | Test | Pause
   - If complexity ≤2: Single-pass implementation
     - Invoke dsp-agent once
     - Git commit: `feat: [Plugin] Stage 4 - DSP`
3. After Stage 4 complete (all phases):
   - Auto-invoke plugin-testing skill
   - If tests FAIL: STOP, show results, wait for fixes (do not continue to Stage 5)
   - If tests PASS: Update PLUGINS.md, update .continue-here.md, decision menu

**Expected Output:**
- Stage 4 dispatcher with complexity-aware phasing
- Git commit per phase for complex plugins
- Auto-test invocation after completion
- Test failure blocks continuation to Stage 5

**Verification:**
- Automated: Grep for complexity check in Stage 4, verify phased vs single-pass branching
- Automated: Grep for plugin-testing invocation, verify blocks on test failure
- Manual: STOP AND ASK LEX: "Please review Stage 4 dispatcher and confirm: (1) Complexity-aware phasing (≥3 uses phases, ≤2 single-pass), (2) Git commit per phase for complex plugins with plan.md timestamp updates, (3) Auto-test invocation after completion with blocking on test failure before Stage 5"

---

#### Task 8: Update plugin-workflow Stage 5 Dispatcher

**Description**: Replace Stage 5 stub with gui-agent dispatch logic, including phased implementation support.

**Required Reading:**
- @procedures/skills/plugin-workflow.md (lines 101-121: Stage 5 GUI with complexity adaptation)
- @procedures/webview/best-practices.md (all: Critical member order, validation steps)

**Dependencies**: Task 7 complete, Task 4 complete (gui-agent exists)

**Implementation Steps:**

1. Edit `.claude/skills/plugin-workflow/SKILL.md` Stage 5 section
2. Implement complexity-aware dispatcher (same pattern as Stage 4):
   - If complexity ≥3: Phased implementation (5.1, 5.2, ...)
   - If complexity ≤2: Single-pass
3. After Stage 5 complete:
   - Auto-invoke plugin-testing skill
   - If tests FAIL: STOP, show results, wait for fixes
   - If tests PASS: Update PLUGINS.md, update .continue-here.md, decision menu

**Expected Output:**
- Stage 5 dispatcher with phasing support
- Auto-test invocation with blocking on failure

**Verification:**
- Automated: Grep for complexity check, verify phasing logic matches Stage 4 pattern
- Automated: Grep for plugin-testing invocation
- Manual: STOP AND ASK LEX: "Please review Stage 5 dispatcher and confirm: (1) Phasing pattern matches Stage 4 (complexity-aware, commits per phase), (2) Auto-test invocation after completion with blocking on failure, (3) All dispatcher patterns consistent across Stages 2-5"

---

### Phase 3c: Validation System (Hooks + Validator Subagent)

#### Task 9: Implement SubagentStop Hook

**Description**: Create the SubagentStop hook for deterministic validation after each subagent completes.

**Required Reading:**
- @architecture/10-extension-architecture.md (lines 96-165: Hook system, conditional execution pattern)
- @architecture/13-error-handling-recovery.md (lines 182-461: Hook patterns, graceful skip logic)
- @architecture/02-core-abstractions.md (lines 185-252: Layer 1 validation - deterministic, fast, blocking)

**Dependencies**: Tasks 1-4 complete (all subagents exist)

**Implementation Steps:**

1. Create file `.claude/hooks/validate-subagent-stop.sh`
2. Implement conditional execution pattern:
   ```bash
   #!/bin/bash
   INPUT=$(cat)
   SUBAGENT=$(echo "$INPUT" | jq -r '.subagent_name')

   # Check relevance FIRST
   if [[ ! "$SUBAGENT" =~ ^(foundation-agent|shell-agent|dsp-agent|gui-agent)$ ]]; then
     echo "Hook not relevant for subagent: $SUBAGENT, skipping gracefully"
     exit 0  # Success, not failure
   fi

   # Execute hook logic based on subagent
   case "$SUBAGENT" in
     foundation-agent)
       # Validate Stage 2: CMakeLists.txt exists, builds successfully
       python3 .claude/hooks/validators/validate-foundation.py || exit 2
       ;;
     shell-agent)
       # Validate Stage 3: All parameters from spec present in code
       python3 .claude/hooks/validators/validate-parameters.py || exit 2
       ;;
     dsp-agent)
       # Validate Stage 4: All components from architecture present
       python3 .claude/hooks/validators/validate-dsp-components.py || exit 2
       ;;
     gui-agent)
       # Validate Stage 5: All bindings match parameter IDs
       python3 .claude/hooks/validators/validate-gui-bindings.py || exit 2
       ;;
   esac

   exit 0
   ```
3. Make executable: `chmod +x .claude/hooks/validate-subagent-stop.sh`
4. Register in `.claude/hooks/hooks.json`:
   ```json
   {
     "hooks": {
       "SubagentStop": [
         {
           "matcher": ".*",
           "hooks": [
             {
               "type": "command",
               "command": "${CLAUDE_PROJECT_DIR}/.claude/hooks/validate-subagent-stop.sh",
               "timeout": 10
             }
           ]
         }
       ]
     }
   }
   ```

**Expected Output:**
- `.claude/hooks/validate-subagent-stop.sh` exists and is executable
- Conditional execution pattern: exits 0 if not relevant subagent
- Calls Python validators for each stage
- Blocks workflow (exit 2) if validation fails
- Registered in hooks.json

**Verification:**
- Automated: File exists at correct path
- Automated: File is executable (`test -x`)
- Automated: hooks.json contains SubagentStop registration
- Manual: STOP AND ASK LEX: "Please review `.claude/hooks/validate-subagent-stop.sh` and confirm: (1) Conditional execution pattern checks relevance first and exits 0 if not applicable, (2) Calls appropriate Python validator for each subagent type, (3) Exits 2 (blocking) on validation failure, (4) Registered in hooks.json with correct matcher and timeout"

---

#### Task 10: Create Python Validation Scripts

**Description**: Create the three Python validator scripts called by SubagentStop hook.

**Required Reading:**
- @architecture/04-component-architecture.md (lines 206-252: Validation implementation details with regex patterns)

**Dependencies**: Task 9 complete (SubagentStop hook exists)

**Implementation Steps:**

1. Create `.claude/hooks/validators/validate-parameters.py`:
   - Parse parameter-spec.md for parameter IDs, types, ranges
   - Extract APVTS declarations from PluginProcessor.cpp via regex:
     `AudioParameter(Float|Bool|Choice)\s*\(\s*ParameterID\s*\{\s*"(\w+)"`
   - Check ID sets match (spec ⊆ code)
   - Validate type mapping (Float→AudioParameterFloat, Bool→AudioParameterBool, Choice→AudioParameterChoice)
   - Warn on range mismatches (not errors—may be intentional normalization)
   - Exit 0 if PASS, exit 1 if FAIL with specific error message

2. Create `.claude/hooks/validators/validate-dsp-components.py`:
   - Parse architecture.md for DSP component list (e.g., "juce::dsp::StateVariableTPTFilter")
   - Check `#include <juce_dsp/juce_dsp.h>` present in PluginProcessor.cpp
   - Verify member variable declaration via regex: `juce::dsp::StateVariableTPTFilter<float> \w+;`
   - Warn if prepareToPlay() doesn't call component.prepare()
   - Exit 0 if PASS, exit 1 if FAIL

3. Create `.claude/hooks/validators/validate-gui-bindings.py`:
   - Extract HTML parameter IDs from ui/public/index.html via regex: `getSliderState\(['"](\w+)['"]\)`
   - Extract C++ relays from PluginEditor.cpp: `WebSliderRelay\s+(\w+)Relay\s*\{\s*"(\w+)"\s*\}`
   - Check ID sets match (HTML IDs == relay IDs)
   - Verify attachments exist for each relay
   - Verify `.withOptionsFrom(relay)` registration in browser setup
   - Exit 0 if PASS, exit 1 if FAIL

4. Make all scripts executable

**Expected Output:**
- Three Python validation scripts exist in `.claude/hooks/validators/`
- Each script parses contracts and implementation
- Regex patterns match architecture specification
- Exit codes: 0 (pass), 1 (fail with error message)

**Verification:**
- Automated: All three files exist and are executable
- Automated: Each script contains required regex patterns
- Manual: STOP AND ASK LEX: "Please review the three Python validators and confirm: (1) validate-parameters.py checks ID sets match and type mapping is correct, (2) validate-dsp-components.py verifies all components from architecture.md are declared and initialized, (3) validate-gui-bindings.py checks HTML IDs match relay IDs and attachments exist, (4) All scripts exit 0 on pass and 1 on fail with specific error messages"

---

#### Task 11: Enhance validator Subagent

**Description**: Update the existing validator subagent for semantic validation (Layer 2).

**Required Reading:**
- @architecture/02-core-abstractions.md (lines 185-252: Layer 2 validation - semantic, 60s, advisory)
- @architecture/04-component-architecture.md (lines 344-407: Validator implementation details, false positive overrides)

**Dependencies**: Tasks 9-10 complete (Layer 1 validation exists)

**Implementation Steps:**

1. Read `.claude/agents/validator.md`
2. Verify/update frontmatter:
   - Model: Sonnet (quality over speed for semantic analysis)
   - No extended thinking (semantic review, not research)
   - Tools: Read, Grep, Bash (research only, no code changes)
3. Verify/update instructions:
   - Role: Independent semantic validation AFTER hooks pass
   - Responsibilities:
     - Assume patterns are correct (hooks validated that)
     - Compare implementation to creative intent from brief
     - Evaluate semantic correctness and design decisions
     - Check JUCE best practices beyond compilation
     - Return structured JSON with recommendations (advisory, not blocking)
   - Process:
     1. Read contracts (creative-brief, parameter-spec, architecture)
     2. Read implementation files
     3. Evaluate semantic correctness and quality
     4. Return JSON with issues array (severity: suggestion | warning | error)
   - False positive override support:
     - Load `.validator-overrides.yaml` if exists
     - Suppress matching errors with logged reason
     - Format:
       ```yaml
       overrides:
         - stage: 4
           check_type: "missing_dsp_component"
           pattern: "CustomReverbImpl"
           reason: "Using custom reverb, not juce::dsp::Reverb"
           date: "2025-11-10"
       ```
4. Integration notes:
   - Invoked by plugin-workflow for complexity ≥4 OR when thorough review requested
   - User decides next action via decision menu (accept | make changes | other)
   - Never blocks workflow (unlike hooks)

**Expected Output:**
- validator.md updated with Layer 2 responsibilities
- Sonnet model specified
- JSON report format includes severity levels
- False positive override system documented

**Verification:**
- Automated: Frontmatter specifies Sonnet model
- Automated: Tools list includes Read, Grep, Bash (not Write/Edit)
- Manual: STOP AND ASK LEX: "Please review `.claude/agents/validator.md` and confirm: (1) Role clearly states 'semantic validation AFTER hooks pass', (2) Instructions specify to assume patterns are correct and focus on semantics/quality, (3) JSON report format includes severity levels (suggestion, warning, error), (4) False positive override system is documented with YAML format"

---

### Phase 3d: Integration Testing & Error Handling

#### Task 12: Implement Error Recovery Menus

**Description**: Ensure all failure points present 4-option recovery menus as specified in architecture.

**Required Reading:**
- @architecture/13-error-handling-recovery.md (lines 1-149: Complete error taxonomy and recovery procedures)
- @procedures/skills/build-automation.md (all: 4-option failure protocol specification)

**Dependencies**: Tasks 5-8 complete (all dispatcher stages implemented)

**Implementation Steps:**

1. Review all dispatcher error handling in plugin-workflow Stages 2-5
2. Verify each failure point presents 4 options:
   - Option 1: Investigate (invoke deep-research skill via troubleshooter agent)
   - Option 2: Show me the code (display problematic files)
   - Option 3: Show me the output (full build log or error details)
   - Option 4: Wait (I'll fix manually, then say "resume automation")
3. Implement investigation option:
   - When user selects "Investigate":
     - Extract error from build log or subagent report
     - Invoke troubleshooter agent via Task tool with error context
     - Present troubleshooter findings (root cause, confidence level, solution)
     - Offer: Apply fix | Show code | Try alternative | Manual fix
4. Implement "resume automation" trigger:
   - Listen for phrase "resume automation" or "continue automation"
   - Re-validate from failure point
   - If validation passes: Continue workflow
   - If validation fails: Show new errors, present 4 options again
5. Never auto-retry: Always wait for user decision

**Expected Output:**
- All dispatcher failure points present 4-option menu
- Investigation option invokes troubleshooter
- "Resume automation" trigger implemented
- No auto-retry behavior anywhere in code

**Verification:**
- Automated: Grep for "Investigate" option in all dispatcher stages
- Automated: Grep for "resume automation" phrase detection
- Automated: Verify no auto-retry loops exist (search for "retry" in loops)
- Manual: STOP AND ASK LEX: "Please review error handling across all dispatcher stages and confirm: (1) All failure points present 4-option menu (investigate, show code, show output, wait), (2) Investigation option invokes troubleshooter agent, (3) 'Resume automation' trigger is implemented, (4) No auto-retry behavior exists anywhere"

---

#### Task 13: Implement Handoff File System

**Description**: Ensure .continue-here.md handoff files are created and updated correctly at all checkpoints.

**Required Reading:**
- @architecture/06-state-architecture.md (lines 156-316: Complete handoff file specification)
- @procedures/core/checkpoint-system.md (all: Checkpoint types and handoff format)

**Dependencies**: Tasks 5-8 complete (all stages need handoff updates)

**Implementation Steps:**

1. Verify .continue-here.md creation after Stage 0:
   - Location: `plugins/[PluginName]/.continue-here.md`
   - Format: YAML frontmatter + markdown sections
   - Content: Current stage (0), completed work (architecture.md), next steps (Stage 1 planning)
2. Verify .continue-here.md updates after Stages 1-5:
   - Update YAML: stage number, last_updated timestamp
   - Update markdown: Add completed work, update next steps, add known issues if any
3. Verify .continue-here.md updates after phases (complex plugins):
   - Example Stage 4.2 complete: Update to reflect phase 4.2 done, next is 4.3
4. Verify .continue-here.md deletion after Stage 6:
   - Plugin status changes to ✅ Working
   - Handoff file no longer needed (workflow complete)
5. Implement error state handoffs:
   - Build failure: Document error in "Known Issues", suggest investigation
   - Validation failure: Document findings, suggest fixes
6. Verify context-resume skill can parse handoff files:
   - Test with various stage numbers
   - Test with phased stages (4.2, 5.1, etc.)
   - Test with error states

**Expected Output:**
- Handoff files created after Stage 0
- Handoff files updated after every stage/phase
- Handoff files deleted after Stage 6
- Error states documented in handoff files
- context-resume skill can parse all handoff formats

**Verification:**
- Automated: Grep for ".continue-here.md" write operations in all stages
- Automated: Verify deletion logic after Stage 6
- Manual: STOP AND ASK LEX: "Please verify handoff file system by checking: (1) .continue-here.md is created after Stage 0 with correct format, (2) Handoff file is updated after every stage and phase completion, (3) Handoff file is deleted after Stage 6 when status changes to ✅, (4) Error states are documented in handoff file 'Known Issues' section, (5) context-resume skill can parse handoff files from any stage"

---

#### Task 14: Implement State Machine Updates

**Description**: Ensure PLUGINS.md state machine is updated correctly at all stage boundaries.

**Required Reading:**
- @architecture/06-state-architecture.md (lines 1-155: Plugin lifecycle state machine, state transitions, ownership model)

**Dependencies**: Tasks 5-8 complete (all stages need state updates)

**Implementation Steps:**

1. Verify PLUGINS.md updates in plugin-workflow:
   - Stage 0 complete: 🚧 Stage 0 (if not already 🚧)
   - Stage 1 complete: 🚧 Stage 1
   - Stage 2 complete: 🚧 Stage 2 (+ lifecycle timeline entry)
   - Stage 3 complete: 🚧 Stage 3 (+ lifecycle timeline entry)
   - Stage 4 complete: 🚧 Stage 4 (+ lifecycle timeline entry)
   - Stage 5 complete: 🚧 Stage 5 (+ lifecycle timeline entry)
   - Stage 6 complete: ✅ Working (+ validation results)
2. Verify state machine protection:
   - If status is 🚧: ONLY plugin-workflow can modify
   - plugin-improve must BLOCK if status is 🚧
   - Display error: "Plugin is in development (🚧 Stage N). Complete workflow first with `/continue [Plugin]` or change status manually."
3. Verify lifecycle timeline entries:
   - Each stage completion adds entry: `- **YYYY-MM-DD (Stage N):** [Description]`
   - Include complexity score in Stage 1 entry
   - Include build results in Stage 2+ entries
4. Verify metadata updates:
   - Last Updated timestamp
   - Build log path
   - Validation log path (Stage 6)

**Expected Output:**
- PLUGINS.md updates at every stage boundary
- State machine protection enforced (🚧 blocks plugin-improve)
- Lifecycle timeline entries added
- Metadata kept current

**Verification:**
- Automated: Grep for PLUGINS.md write operations in all stages
- Automated: Verify plugin-improve has 🚧 status check with blocking error
- Manual: STOP AND ASK LEX: "Please verify state machine by checking: (1) PLUGINS.md is updated with correct status emoji at every stage boundary, (2) plugin-improve blocks if status is 🚧 with clear error message, (3) Lifecycle timeline entries are added for each stage completion, (4) Metadata (Last Updated, build logs, validation logs) is kept current"

---

#### Task 15: Implement Git Workflow Integration

**Description**: Ensure git commits happen at all stage boundaries with standardized messages.

**Required Reading:**
- @procedures/skills/plugin-workflow.md (lines 219-243: Git workflow specification)
- @architecture/06-state-architecture.md (lines 456-470: Atomic state transitions)

**Dependencies**: Tasks 5-8 complete (all stages need commits)

**Implementation Steps:**

1. Verify git commit after Stage 0:
   - Message: `feat: [PluginName] Stage 0 - architecture`
   - Files: `plugins/[PluginName]/.ideas/architecture.md`, `.continue-here.md`, `PLUGINS.md`
2. Verify git commit after Stage 1:
   - Message: `feat: [PluginName] Stage 1 - planning complete`
   - Files: `plugins/[PluginName]/plan.md`, `.continue-here.md`, `PLUGINS.md`
3. Verify git commits after Stages 2-6:
   - Message format: `feat: [PluginName] Stage N - [description]`
   - Files: All modified source files, `.continue-here.md`, `PLUGINS.md`
4. Verify git commits for phased stages (complexity ≥3):
   - Stage 4.1: `feat: [PluginName] Stage 4.1 - [phase description]`
   - Stage 4.2: `feat: [PluginName] Stage 4.2 - [phase description]`
   - Files: Modified source files, `plan.md` (with phase timestamp), `.continue-here.md`
5. Verify atomic commits:
   - All related files (source + state + handoff) committed together
   - No partial commits (either all or none)
6. Error handling:
   - If git commit fails: Present error, offer to resolve manually, retry after resolution

**Expected Output:**
- Git commits at every stage/phase boundary
- Standardized commit messages
- Atomic commits (source + state + handoff together)
- Phased stages have separate commits per phase

**Verification:**
- Automated: Grep for git commit commands in all stages
- Automated: Verify commit message format matches specification
- Automated: Verify atomic commits (multiple files in single commit)
- Manual: STOP AND ASK LEX: "Please verify git workflow by checking: (1) Git commits occur after every stage/phase completion, (2) Commit messages follow format 'feat: [PluginName] Stage N - description', (3) Phased stages have separate commits per phase with phase number (4.1, 4.2), (4) Commits are atomic (source + state + handoff files together), (5) Git commit failures are handled with retry option"

---

## Comprehensive Verification Criteria

### Automated Tests

```bash
# Test 1: Verify all subagent files exist
ls -la .claude/agents/foundation-agent.md
ls -la .claude/agents/shell-agent.md
ls -la .claude/agents/dsp-agent.md
ls -la .claude/agents/gui-agent.md

# Test 2: Verify SubagentStop hook exists and is executable
test -x .claude/hooks/validate-subagent-stop.sh && echo "PASS" || echo "FAIL"

# Test 3: Verify Python validators exist and are executable
test -x .claude/hooks/validators/validate-parameters.py && echo "PASS" || echo "FAIL"
test -x .claude/hooks/validators/validate-dsp-components.py && echo "PASS" || echo "FAIL"
test -x .claude/hooks/validators/validate-gui-bindings.py && echo "PASS" || echo "FAIL"

# Test 4: Verify hooks.json contains SubagentStop registration
jq '.hooks.SubagentStop' .claude/hooks/hooks.json | grep validate-subagent-stop.sh && echo "PASS" || echo "FAIL"

# Test 5: Verify plugin-workflow contains dispatcher logic for all stages
grep -q "Task(" .claude/skills/plugin-workflow/SKILL.md && echo "PASS" || echo "FAIL"
grep -q "foundation-agent" .claude/skills/plugin-workflow/SKILL.md && echo "PASS" || echo "FAIL"
grep -q "shell-agent" .claude/skills/plugin-workflow/SKILL.md && echo "PASS" || echo "FAIL"
grep -q "dsp-agent" .claude/skills/plugin-workflow/SKILL.md && echo "PASS" || echo "FAIL"
grep -q "gui-agent" .claude/skills/plugin-workflow/SKILL.md && echo "PASS" || echo "FAIL"

# Test 6: Verify validator subagent uses Sonnet model
grep -q "model: sonnet" .claude/agents/validator.md && echo "PASS" || echo "FAIL"

# Test 7: Verify no auto-retry loops exist
! grep -r "while.*retry" .claude/skills/plugin-workflow/SKILL.md && echo "PASS" || echo "FAIL"

# Test 8: Verify 4-option failure menus exist
grep -c "Investigate" .claude/skills/plugin-workflow/SKILL.md # Should be ≥4 (one per stage)

# Test 9: Verify .continue-here.md write operations in all stages
grep -c ".continue-here.md" .claude/skills/plugin-workflow/SKILL.md # Should be ≥7 (once per stage)

# Test 10: Verify PLUGINS.md write operations in all stages
grep -c "PLUGINS.md" .claude/skills/plugin-workflow/SKILL.md # Should be ≥7

# Test 11: Verify git commit commands in all stages
grep -c "git commit" .claude/skills/plugin-workflow/SKILL.md # Should be ≥7
```

### Manual Verification Checklist

- [ ] When I examine foundation-agent.md, does it reference all required contracts (creative-brief.md, architecture.md, plan.md)?
- [ ] When I examine shell-agent.md, does it enforce parameter-spec.md contract (blocks if missing)?
- [ ] When I examine dsp-agent.md, does it specify conditional model selection (Opus for complexity ≥4)?
- [ ] When I examine gui-agent.md, is the member declaration order (Relays → WebView → Attachments) marked CRITICAL?
- [ ] When I examine plugin-workflow Stage 2, does it include robust JSON parsing with error handling?
- [ ] When I examine plugin-workflow Stage 3, does it check parameter-spec.md existence before dispatch?
- [ ] When I examine plugin-workflow Stage 4, does it implement complexity-aware phasing with git commits per phase?
- [ ] When I examine plugin-workflow Stage 5, does it follow the same phasing pattern as Stage 4?
- [ ] When I examine SubagentStop hook, does it check relevance first and exit 0 gracefully if not applicable?
- [ ] When I examine Python validators, do they match the regex patterns specified in architecture/04-component-architecture.md?
- [ ] When I examine validator subagent, does it clearly state "semantic validation AFTER hooks pass"?
- [ ] When I examine error handling, do all failure points present 4 options (investigate, show code, show output, wait)?
- [ ] When I examine handoff file creation, is .continue-here.md created after Stage 0 and updated after every stage?
- [ ] When I examine state machine, does plugin-improve block if status is 🚧 with a clear error message?
- [ ] When I examine git workflow, are commits atomic (source + state + handoff together)?

### File Existence Verification

```bash
# Verify all expected files exist
ls -la .claude/agents/foundation-agent.md
ls -la .claude/agents/shell-agent.md
ls -la .claude/agents/dsp-agent.md
ls -la .claude/agents/gui-agent.md
ls -la .claude/agents/validator.md
ls -la .claude/hooks/validate-subagent-stop.sh
ls -la .claude/hooks/validators/validate-parameters.py
ls -la .claude/hooks/validators/validate-dsp-components.py
ls -la .claude/hooks/validators/validate-gui-bindings.py
ls -la .claude/hooks/hooks.json
ls -la .claude/skills/plugin-workflow/SKILL.md
```

### Integration Tests

**Test 1: Simple Plugin End-to-End (Complexity ≤2)**

1. Create test plugin: SimpleGain (1 parameter: gain)
2. Run `/dream SimpleGain` → Create creative brief
3. Run `/implement SimpleGain`
4. Verify Stage 0: architecture.md created
5. Verify Stage 1: plan.md created with complexity score ≤2
6. Verify Stage 2: foundation-agent dispatched, CMakeLists.txt created, build succeeds
7. Verify Stage 3: shell-agent dispatched, parameter from spec present, build succeeds
8. Verify Stage 4: dsp-agent dispatched (single-pass, no phases), DSP implementation, tests pass
9. Verify Stage 5: gui-agent dispatched (single-pass), WebView UI, tests pass
10. Verify Stage 6: pluginval passes, status changes to ✅ Working
11. Verify .continue-here.md deleted after Stage 6
12. Verify PLUGINS.md shows ✅ Working status
13. Total duration: ~30-45 minutes

**Test 2: Complex Plugin End-to-End (Complexity ≥3)**

1. Create test plugin: MediumDelay (5 parameters: delay time, feedback, filter, mix, modulation)
2. Run `/dream MediumDelay` → Create creative brief
3. Create UI mockup with `/mockup`, finalize to generate parameter-spec.md
4. Run `/implement MediumDelay`
5. Verify Stage 1: plan.md created with complexity score ≥3, phased breakdown
6. Verify Stage 4: Multiple phases (4.1, 4.2, 4.3), separate git commits per phase
7. Verify Stage 5: Multiple phases (5.1, 5.2), separate git commits per phase
8. Verify plan.md updated with phase timestamps
9. Verify .continue-here.md reflects current phase progress
10. Total duration: ~60-90 minutes

**Test 3: Error Recovery**

1. Create test plugin: ErrorTest
2. During Stage 2: Intentionally introduce CMake error
3. Verify build failure presents 4-option menu
4. Select "Investigate" option
5. Verify troubleshooter invoked and returns findings
6. Fix error manually
7. Say "resume automation"
8. Verify Stage 2 re-validates and continues

**Test 4: Validation System**

1. Create test plugin: ValidationTest
2. After Stage 3: Manually remove a parameter from code (but keep in parameter-spec.md)
3. Verify SubagentStop hook catches missing parameter
4. Verify workflow blocks with error message
5. Fix parameter
6. Verify workflow continues after fix

**Test 5: Pause/Resume**

1. Create test plugin: PauseTest
2. Complete Stage 3
3. At Stage 3 decision menu, select "Pause here"
4. Verify .continue-here.md updated
5. Verify PLUGINS.md shows 🚧 Stage 3
6. Start new session
7. Run `/continue PauseTest`
8. Verify context loaded correctly
9. Verify workflow continues from Stage 4

### Success Criteria

Phase 3 is COMPLETE when:

1. ✅ All 4 subagent files exist with correct frontmatter and complete instructions
2. ✅ SubagentStop hook exists, is executable, and registered in hooks.json
3. ✅ All 3 Python validators exist, are executable, and contain correct regex patterns
4. ✅ validator subagent updated with Layer 2 semantic validation responsibilities
5. ✅ plugin-workflow Stages 2-5 have dispatcher logic with Task tool invocations
6. ✅ JSON parsing includes robust error handling (markdown extraction, brace matching, validation)
7. ✅ All failure points present 4-option recovery menu (investigate, show code, show output, wait)
8. ✅ Complexity-aware phasing implemented in Stages 4 and 5
9. ✅ Auto-test invocation after Stages 4 and 5 with blocking on failure
10. ✅ .continue-here.md handoff files created and updated at all checkpoints
11. ✅ PLUGINS.md state machine updated at all stage boundaries
12. ✅ State machine protection enforced (🚧 blocks plugin-improve)
13. ✅ Git commits at all stage/phase boundaries with standardized messages
14. ✅ Atomic commits (source + state + handoff together)
15. ✅ Simple plugin (complexity ≤2) completes end-to-end successfully
16. ✅ Complex plugin (complexity ≥3) completes with phased implementation
17. ✅ Error recovery workflow operates correctly
18. ✅ Validation system catches contract violations
19. ✅ Pause/resume workflow preserves context across sessions
20. ✅ No auto-retry behavior exists anywhere in the system

## Potential Issues & Mitigations

**Issue 1: JSON parsing failures from subagents**
- **Symptom:** Subagent returns malformed JSON or text outside JSON block
- **Mitigation:** Robust parsing implemented (markdown extraction, brace matching, partial data extraction)
- **Recovery:** Present parse error to user with retry option

**Issue 2: Context limit exceeded during long stages**
- **Symptom:** Task tool fails with "context limit exceeded"
- **Mitigation:** PreCompact hook preserves contracts during compaction
- **Recovery:** Truncate older messages, prioritize contracts, retry

**Issue 3: Build failures with unclear errors**
- **Symptom:** build-and-install.sh fails but error message unclear
- **Mitigation:** 4-option menu with investigation via troubleshooter
- **Recovery:** User selects "Investigate", troubleshooter researches error

**Issue 4: Validation false positives**
- **Symptom:** Validator flags correct code as problematic
- **Mitigation:** False positive override system (.validator-overrides.yaml)
- **Recovery:** User logs override with justification, validator suppresses error

**Issue 5: Handoff file corruption**
- **Symptom:** .continue-here.md malformed, context-resume fails to parse
- **Mitigation:** Fallback to git commit history for state reconstruction
- **Recovery:** context-resume uses commit messages to infer state

**Issue 6: State machine race condition**
- **Symptom:** Multiple workflows try to modify 🚧 plugin simultaneously
- **Mitigation:** State machine protection (only plugin-workflow can modify 🚧)
- **Recovery:** plugin-improve blocks with error, user completes workflow first

**Issue 7: Hook execution timeout**
- **Symptom:** Python validator takes >10s, hook times out
- **Mitigation:** Increase timeout in hooks.json (currently 10s, can extend to 30s)
- **Recovery:** Disable hook temporarily, test validator manually, fix performance, re-enable

**Issue 8: Git commit failures**
- **Symptom:** Git commit fails due to conflicts or uncommitted changes
- **Mitigation:** Check git status before commit, resolve conflicts automatically if possible
- **Recovery:** Present error to user, offer manual resolution, retry after fix

## Notes for Next Phase

**Phase 4 (Build & Troubleshooting System) will require:**
- build-and-install.sh script fully operational (started in Phase 3, completed in Phase 4)
- troubleshooter agent fully configured (exists but needs deep-research skill integration)
- 4-option failure protocol already implemented in Phase 3
- Build logging system (logs/[Plugin]/ directory structure)

**Phase 4 prerequisites from Phase 3:**
- ✅ Dispatcher system operational (Stages 2-5 dispatch subagents)
- ✅ Error recovery menus implemented (4 options at all failure points)
- ✅ troubleshooter agent exists and is invoked via "Investigate" option

**Phase 5 (Validation System) prerequisites from Phase 3:**
- ✅ SubagentStop hook implemented (deterministic Layer 1)
- ✅ Python validators implemented (pattern matching)
- ✅ validator subagent enhanced (semantic Layer 2)
- Need in Phase 5: Additional hooks (PostToolUse, Stop, UserPromptSubmit, PreCompact, SessionStart)

**Phase 6 (WebView UI System) prerequisites from Phase 3:**
- ✅ gui-agent subagent implemented with WebView integration
- ✅ Member declaration order enforcement
- Need in Phase 5: Complete ui-mockup two-phase workflow (design → decision → implementation)
- Need in Phase 6: WebView code templates in skill assets/
