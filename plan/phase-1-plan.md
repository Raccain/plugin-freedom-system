# Phase 1 Implementation Plan

## Prerequisites Status

**Previous Phase(s) Required:** Phase 0

**Deliverables Check:**
- ✅ `.claude/commands/` - EXISTS
- ✅ `.claude/skills/` - EXISTS
- ✅ `.claude/agents/` - EXISTS
- ✅ `.claude/hooks/` - EXISTS
- ✅ `.claude/CLAUDE.md` - EXISTS
- ✅ `PLUGINS.md` - EXISTS
- ✅ `.gitignore` - EXISTS
- ✅ Contract templates in skill assets/ - EXISTS
  - `.claude/skills/plugin-ideation/assets/creative-brief-template.md`
  - `.claude/skills/plugin-workflow/assets/plan-template.md`
  - `.claude/skills/plugin-workflow/assets/architecture-template.md`
  - `.claude/skills/ui-mockup/assets/parameter-spec-template.md`

**Status:** Complete

All Phase 0 deliverables exist and are ready for Phase 1 implementation.

---

## Phase Overview

**Phase 1: Discovery System** implements the complete navigation and discovery layer that enables users to explore system capabilities from session 1. This phase creates all user-facing entry points (slash commands), workflow orchestrators (skills), and the interactive decision system that enables "discovery through play" rather than documentation-heavy learning.

**Why this phase is critical:**
- Establishes all user entry points via slash command autocomplete
- Implements the 7 core workflows that drive plugin development
- Creates the interactive decision menu system that enables progressive feature discovery
- Implements the handoff/checkpoint system that allows pause and resume across sessions
- Without this layer, users cannot interact with the system - all subsequent phases depend on these workflows

**Duration:** ~4-6 hours (split across 3 sub-phases)

---

## Sub-Phase Breakdown

Phase 1 is split into 3 sub-phases because:
- **Distinct components:** Commands (entry layer) vs Skills (workflow layer) vs Systems (decision/handoff infrastructure)
- **Logical grouping:** Commands can be tested via autocomplete before skills exist; Skills can be implemented incrementally; Systems integrate across both
- **Different complexity:** Commands are simple text files; Skills are complex multi-stage workflows; Systems require cross-component integration

### Phase 1a: Slash Commands (All 9)
Complete all slash command files in `.claude/commands/` with proper frontmatter, precondition checks, and routing instructions. This enables feature discovery via autocomplete.

**Duration:** ~30-45 minutes

### Phase 1b: Core Skills (All 7)
Implement all 7 skill files in `.claude/skills/` with complete workflow logic, decision menus, state management, and git integration. This creates the functional workflows.

**Duration:** ~2.5-3.5 hours

### Phase 1c: Interactive & Handoff Systems
Integrate interactive decision menus into all skills and implement the `.continue-here.md` handoff file system for pause/resume functionality.

**Duration:** ~1-2 hours

---

## Required Documentation

### Architecture Files (MUST READ IN ENTIRETY)

- **@architecture/05-routing-architecture.md** - Instructed routing pattern (how commands invoke skills via text prompts, not programmatic dispatch), precondition verification patterns, command-to-skill mapping

- **@architecture/04-component-architecture.md** - Skill structure requirements, frontmatter format, allowed tools per skill, model selection rules, precondition specifications

- **@architecture/06-state-architecture.md** - Lines 1-61: Plugin lifecycle state machine (💡→🚧→✅→📦), state transition rules, PLUGINS.md format. Lines 62-154: Handoff file YAML frontmatter and markdown body structure

- **@architecture/07-communication-architecture.md** - Skill-to-skill communication patterns, context passing, decision menu response handling, error propagation

- **@architecture/00-PHILOSOPHY.md** - Discovery through play principle, progressive disclosure philosophy, risk-free exploration via checkpointing, "choose your own adventure" design

### Procedure Files (MUST READ IN ENTIRETY)

**Commands:**
- **@procedures/commands/dream.md** - Complete specification for `/dream` command: 4-option menu structure, routing logic, plugin-ideation invocation
- **@procedures/commands/implement.md** - Complete specification for `/implement` command: 7-stage workflow routing, precondition enforcement, creative brief requirement
- **@procedures/commands/improve.md** - Complete specification for `/improve` command: versioning logic, vagueness detection, 3-tier investigation protocol
- **@procedures/commands/continue.md** - Complete specification for `/continue` command: handoff file parsing, context restoration, resumption options
- **@procedures/commands/test.md** - Complete specification for `/test` command: 3 test modes (automated/build/manual), pluginval integration
- **@procedures/commands/install-plugin.md** - Complete specification for `/install-plugin` command: system folder installation, cache clearing, verification
- **@procedures/commands/show-standalone.md** - Complete specification for `/show-standalone` command: standalone build and launch logic
- **@procedures/commands/troubleshoot-juce.md** - DEPRECATED command, replaced by deep-research skill
- **@procedures/commands/doc-fix.md** - Complete specification for `/doc-fix` command: troubleshooting documentation capture

**Skills:**
- **@procedures/skills/plugin-ideation.md** - Complete workflow: adaptive brainstorming, creative brief generation, decision menus, output format
- **@procedures/skills/plugin-workflow.md** - Complete 7-stage workflow: research, planning, foundation, shell, DSP, GUI, validation with complexity adaptation
- **@procedures/skills/plugin-improve.md** - Complete improvement workflow: investigation tiers, versioning, backup creation, CHANGELOG updates
- **@procedures/skills/context-resume.md** - Complete resume logic: handoff file parsing, context loading, state reconstruction, continuation options
- **@procedures/skills/ui-mockup.md** - Complete mockup workflow: 7-file generation, versioning, browser testing, JUCE integration
- **@procedures/skills/plugin-testing.md** - Complete testing suite: automated stability tests, pluginval integration, manual checklist
- **@procedures/skills/plugin-lifecycle.md** - Complete installation workflow: system folder management, cache clearing, verification

**Core Systems:**
- **@procedures/core/interactive-decision-system.md** - Decision menu patterns, option ordering, progressive disclosure, context-aware generation, inline format requirements
- **@procedures/core/checkpoint-system.md** - 3 checkpoint types (stage/phase/decision), handoff file system, Claude Code Esc+Esc integration, resume commands

---

## Implementation Sequence

### Phase 1a: Slash Commands

#### Task 1: Complete Missing Slash Commands

**Description**: Create the 4 missing command files (`install-plugin.md`, `show-standalone.md`, `troubleshoot-juce.md`, `doc-fix.md`) in `.claude/commands/` to bring total to 9 commands. These commands enable feature discovery via slash command autocomplete and provide routing instructions for Claude to invoke the appropriate skills.

**Required Reading**:
- @procedures/commands/install-plugin.md (sections: all)
- @procedures/commands/show-standalone.md (sections: all)
- @procedures/commands/troubleshoot-juce.md (sections: all)
- @procedures/commands/doc-fix.md (sections: all)
- @architecture/05-routing-architecture.md (sections: lines 122-127, instructed routing pattern)

**Dependencies**: Task 1 from Phase 0 (`.claude/commands/` directory must exist)

**Implementation Steps**:

1. Create `.claude/commands/install-plugin.md`:
   - Frontmatter with name and description
   - Invocation instructions for `plugin-lifecycle` skill
   - Preconditions: Plugin status must be ✅ Working (Stage 6 complete)
   - Behavior: Build release mode, install to system folders, clear caches

2. Create `.claude/commands/show-standalone.md`:
   - Frontmatter with name and description
   - Direct execution instructions (no skill routing)
   - Preconditions: Plugin must exist
   - Behavior: Build standalone if needed, launch .app

3. Create `.claude/commands/troubleshoot-juce.md`:
   - Frontmatter with name and description
   - DEPRECATED notice
   - Redirect to `deep-research` skill (Phase 7)

4. Create `.claude/commands/doc-fix.md`:
   - Frontmatter with name and description
   - Invocation instructions for `troubleshooting-docs` skill (Phase 7)
   - Preconditions: Problem just solved in conversation
   - Behavior: Extract context, create troubleshooting doc

**Expected Output**:
- `.claude/commands/install-plugin.md`
- `.claude/commands/show-standalone.md`
- `.claude/commands/troubleshoot-juce.md`
- `.claude/commands/doc-fix.md`

Total: 9 command files in `.claude/commands/`

**Verification**:
- Automated:
  ```bash
  # Verify all 9 command files exist
  for cmd in dream implement improve continue test install-plugin show-standalone troubleshoot-juce doc-fix; do
    if [ -f ".claude/commands/$cmd.md" ]; then
      echo "✓ .claude/commands/$cmd.md exists"
    else
      echo "✗ .claude/commands/$cmd.md missing"
    fi
  done

  # Verify each has frontmatter
  for cmd in dream implement improve continue test install-plugin show-standalone troubleshoot-juce doc-fix; do
    if grep -q "^---$" ".claude/commands/$cmd.md"; then
      echo "✓ $cmd.md has frontmatter"
    else
      echo "✗ $cmd.md missing frontmatter"
    fi
  done
  ```
- Manual: STOP AND ASK LEX: "Please type `/` in Claude Code and verify that autocomplete now shows all 9 commands: /dream, /implement, /improve, /continue, /test, /install-plugin, /show-standalone, /troubleshoot-juce, /doc-fix"

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 2: Verify Command Routing Logic

**Description**: Read all 9 command files and verify that routing instructions follow the instructed routing pattern (text-based prompts, not programmatic dispatch) and that preconditions are clearly specified.

**Required Reading**:
- @architecture/05-routing-architecture.md (sections: lines 122-578, instructed routing pattern and precondition verification)

**Dependencies**: Task 1 (all 9 command files must exist)

**Implementation Steps**:

1. Read all 9 command files in `.claude/commands/`

2. For each file, verify:
   - Has YAML frontmatter with `name:` and `description:` fields
   - Contains clear invocation instruction (e.g., "When user runs `/X [args]`, invoke the Y skill")
   - Lists preconditions explicitly (or states "None" if entry point)
   - Describes behavior and outputs
   - Uses instructed routing language (not programmatic function calls)

3. Check routing targets:
   - `/dream` → `plugin-ideation` skill
   - `/implement` → `plugin-workflow` skill
   - `/improve` → `plugin-improve` skill
   - `/continue` → `context-resume` skill
   - `/test` → `plugin-testing` skill or `build-automation` skill
   - `/install-plugin` → `plugin-lifecycle` skill
   - `/show-standalone` → Direct execution (no skill)
   - `/troubleshoot-juce` → DEPRECATED marker
   - `/doc-fix` → `troubleshooting-docs` skill

**Expected Output**: Verification report confirming all commands follow patterns

**Verification**:
- Automated:
  ```bash
  # Verify routing targets mentioned in command files
  echo "=== Command Routing Verification ==="
  grep -l "plugin-ideation" .claude/commands/dream.md && echo "✓ /dream routes to plugin-ideation"
  grep -l "plugin-workflow" .claude/commands/implement.md && echo "✓ /implement routes to plugin-workflow"
  grep -l "plugin-improve" .claude/commands/improve.md && echo "✓ /improve routes to plugin-improve"
  grep -l "context-resume" .claude/commands/continue.md && echo "✓ /continue routes to context-resume"
  grep -l "plugin-testing" .claude/commands/test.md && echo "✓ /test routes to plugin-testing"
  grep -l "plugin-lifecycle" .claude/commands/install-plugin.md && echo "✓ /install-plugin routes to plugin-lifecycle"
  grep -l "DEPRECATED" .claude/commands/troubleshoot-juce.md && echo "✓ /troubleshoot-juce marked deprecated"
  ```
- Manual: STOP AND ASK LEX: "Please review one command file (e.g., `.claude/commands/implement.md`) and confirm it uses instructed routing language like 'When user runs... invoke the X skill' rather than programmatic function calls."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

### Phase 1b: Core Skills

#### Task 3: Create plugin-ideation Skill

**Description**: Implement the adaptive brainstorming skill that generates creative briefs for new plugins and improvement proposals. This skill uses free-form collection followed by targeted refinement to capture plugin vision without rigid structure.

**Required Reading**:
- @procedures/skills/plugin-ideation.md (sections: all)
- @procedures/core/interactive-decision-system.md (sections: all - decision menu patterns)
- @architecture/04-component-architecture.md (sections: lines 119-133, skill frontmatter format)

**Dependencies**: Task 1 (`.claude/skills/` directory must exist from Phase 0)

**Implementation Steps**:

1. Create `.claude/skills/plugin-ideation/` directory

2. Create `.claude/skills/plugin-ideation/SKILL.md` with:
   - Frontmatter:
     ```yaml
     ---
     name: plugin-ideation
     description: Adaptive brainstorming for plugin concepts and improvements
     allowed-tools:
       - Read
       - Write
       - Bash
     preconditions:
       - None (entry point)
     ---
     ```

3. Implement workflow sections:
   - **Mode Detection:** Check if plugin exists → new plugin mode vs improvement mode
   - **Free-Form Collection:** "What would you like to build/improve?" with adaptive follow-up
   - **Targeted Refinement:** Ask only about gaps, one question at a time
   - **Progress Checkpoints:** Show summary, offer to continue or finalize
   - **Document Creation:** Generate `creative-brief.md` or `improvements/[feature-name].md`
   - **Session Handoff:** Create `.continue-here.md` with YAML frontmatter
   - **Decision Menu:** Present next steps (create mockup, start implementation, save for later)

4. Add decision menu after completion:
   ```
   ✓ Creative brief complete

   What's next?
   1. Create UI mockup (recommended)
   2. Start implementation
   3. Save for later
   4. Other
   ```

5. Include vagueness detection for improvement mode:
   - Check for specific feature name, action, and acceptance criteria
   - If vague: offer (1) Brainstorm first or (2) Implement with assumptions

**Expected Output**: `.claude/skills/plugin-ideation/SKILL.md` (complete skill file)

**Verification**:
- Automated:
  ```bash
  # Verify skill file exists with frontmatter
  test -f .claude/skills/plugin-ideation/SKILL.md && echo "✓ plugin-ideation skill exists" || echo "✗ Missing"
  grep -q "^---$" .claude/skills/plugin-ideation/SKILL.md && echo "✓ Has frontmatter" || echo "✗ No frontmatter"
  grep -q "name: plugin-ideation" .claude/skills/plugin-ideation/SKILL.md && echo "✓ Name correct" || echo "✗ Wrong name"
  ```
- Manual: STOP AND ASK LEX: "Please run `/dream TestPlugin` and verify that the skill asks 'What would you like to build?' and presents a decision menu after the creative brief is generated."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 4: Create plugin-workflow Skill

**Description**: Implement the complete 7-stage workflow orchestrator that builds plugins from research through validation. This is the most complex skill, requiring stage dispatch logic, complexity-adaptive phasing, git integration, and automated testing invocation.

**Required Reading**:
- @procedures/skills/plugin-workflow.md (sections: all)
- @architecture/06-state-architecture.md (sections: lines 1-61, state machine; lines 62-154, handoff format)
- @procedures/core/checkpoint-system.md (sections: all)

**Dependencies**: Task 3 (skill directory structure established)

**Implementation Steps**:

1. Create `.claude/skills/plugin-workflow/` directory

2. Create `.claude/skills/plugin-workflow/SKILL.md` with:
   - Frontmatter:
     ```yaml
     ---
     name: plugin-workflow
     description: Complete 7-stage JUCE plugin development workflow
     allowed-tools:
       - Task  # For subagents (Phases 2-5)
       - Bash  # For git commits
       - Read
       - Write
       - Edit
     preconditions:
       - Plugin must not exist OR status must be 💡 (ideated)
       - Plugin must NOT be 🚧 (use /continue instead)
     ---
     ```

3. Implement precondition checking:
   - Read PLUGINS.md to check status
   - Check for creative brief existence
   - Block if status is 🚧 with message to use `/continue`

4. Implement 7 stages:

   **Stage 0: Research (5-10 min)**
   - Define plugin type and approach
   - Use Context7 MCP for JUCE docs lookup
   - Research professional examples
   - Output: `plugins/[Name]/.ideas/architecture.md` (DSP specification)
   - Create `.continue-here.md` after completion
   - Decision menu: Continue to Stage 1 / Review research / Pause

   **Stage 1: Planning (2-5 min)**
   - Calculate complexity score: `min(params/5, 2) + algorithms + features` (cap at 5)
   - Simple (≤2): Single-pass
   - Complex (≥3): Phase-based with test criteria
   - Check contract prerequisites: BLOCKS if `parameter-spec.md` or `architecture.md` missing
   - Output: `plugins/[Name]/.ideas/plan.md`
   - Git commit: `feat: [Plugin] Stage 1 - planning complete`
   - Update `.continue-here.md`
   - Decision menu: Continue to Stage 2 / Review plan / Adjust complexity / Pause

   **Stage 2: Foundation (5 min) - STUB FOR PHASE 3**
   - NOTE: "Stage 2 requires foundation-agent (Phase 3). Creating placeholder."
   - Placeholder: Create minimal CMakeLists.txt and empty source files manually
   - Git commit: `feat: [Plugin] Stage 2 - foundation (manual)`
   - Update `.continue-here.md`
   - Decision menu: Continue to Stage 3 / Review foundation / Pause

   **Stage 3: Shell (5 min) - STUB FOR PHASE 3**
   - NOTE: "Stage 3 requires shell-agent (Phase 3). Creating placeholder."
   - Placeholder: Implement basic APVTS and empty processBlock manually
   - Git commit: `feat: [Plugin] Stage 3 - shell (manual)`
   - Update `.continue-here.md`
   - Decision menu: Continue to Stage 4 / Review shell / Pause

   **Stage 4: DSP (15-45 min) - STUB FOR PHASE 3**
   - NOTE: "Stage 4 requires dsp-agent (Phase 3). Creating placeholder."
   - Placeholder: Implement basic processBlock logic manually
   - For complex: Create phases 4.1, 4.2, etc. with test criteria per phase
   - Git commit: Simple: `feat: [Plugin] Stage 4 - DSP`, Complex: per phase
   - Update `.continue-here.md`
   - Decision menu: Continue to Stage 5 / Review DSP / Test audio / Pause

   **Stage 5: GUI (20-60 min) - STUB FOR PHASE 3**
   - NOTE: "Stage 5 requires gui-agent (Phase 3). Creating placeholder."
   - Placeholder: Implement basic FlexBox layout manually
   - For complex: Create phases 5.1, 5.2, etc.
   - Git commit: Simple: `feat: [Plugin] Stage 5 - GUI`, Complex: per phase
   - Update `.continue-here.md`
   - Decision menu: Continue to Stage 6 / Review GUI / Test UI / Pause

   **Stage 6: Validation (10-20 min)**
   - Create factory presets in `Presets/` directory
   - Invoke `plugin-testing` skill (Phase 1b Task 8)
   - Generate CHANGELOG.md using Keep a Changelog format
   - Update PLUGINS.md: status 🚧 Stage 6 → ✅ Working
   - Git commit: `feat: [Plugin] Stage 6 - validation complete`
   - Delete `.continue-here.md` (workflow complete)
   - Decision menu: Install plugin / Test in DAW / Improve / Create another / Pause

5. Implement stage boundary protocol:
   - Show completion statement
   - Auto-commit with standardized message
   - Update `.continue-here.md` with YAML frontmatter and markdown body
   - Present decision menu
   - Wait for user response

6. Implement resume handling:
   - Support `"resume automation"` command
   - Parse `.continue-here.md` to determine current stage/phase
   - Continue from documented "Next Steps"

7. Update PLUGINS.md at key points:
   - After Stage 0: 💡 Ideated → 🚧 Stage 0
   - After each stage: 🚧 Stage N → 🚧 Stage N+1
   - After Stage 6: 🚧 Stage 6 → ✅ Working

**Expected Output**: `.claude/skills/plugin-workflow/SKILL.md` (complete skill file with stage dispatch logic)

**Verification**:
- Automated:
  ```bash
  # Verify skill file exists and has all stages documented
  test -f .claude/skills/plugin-workflow/SKILL.md && echo "✓ plugin-workflow skill exists" || echo "✗ Missing"
  for stage in "Stage 0" "Stage 1" "Stage 2" "Stage 3" "Stage 4" "Stage 5" "Stage 6"; do
    grep -q "$stage" .claude/skills/plugin-workflow/SKILL.md && echo "✓ $stage documented" || echo "✗ $stage missing"
  done
  ```
- Manual: STOP AND ASK LEX: "Please run `/implement TestPlugin` (without creating a creative brief first) and verify that it blocks with a message asking you to run `/dream` first to create the creative brief."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 5: Create plugin-improve Skill

**Description**: Implement the versioned improvement workflow that fixes bugs and adds features with automatic backup creation, investigation protocols, and CHANGELOG updates.

**Required Reading**:
- @procedures/skills/plugin-improve.md (sections: all)
- @procedures/core/checkpoint-system.md (sections: backup creation, git workflow)

**Dependencies**: Task 3 (skill directory structure established)

**Implementation Steps**:

1. Create `.claude/skills/plugin-improve/` directory

2. Create `.claude/skills/plugin-improve/SKILL.md` with:
   - Frontmatter:
     ```yaml
     ---
     name: plugin-improve
     description: Fix bugs and add features with versioning and backups
     allowed-tools:
       - Read
       - Write
       - Edit
       - Bash
       - Task  # For deep-research (Tier 3)
     preconditions:
       - Plugin status must be ✅ Working OR 📦 Installed
       - Plugin must NOT be 🚧 In Development
     ---
     ```

3. Implement precondition checking:
   - Read PLUGINS.md to verify status
   - Block if status is 🚧 with message to use `/continue`
   - Block if status is 💡 with message to use `/implement`

4. Implement Phase 0: Exploration (optional):
   - Detect vague requests (no specific feature name, action, or acceptance criteria)
   - If vague: Present menu (1) Brainstorm first (→ plugin-ideation) or (2) Implement with assumptions
   - Wait for choice

5. Implement Phase 0.5: Investigation (3-tier):
   - **Tier 1: Basic (5-10 min)** - Quick file read, obvious issues
   - **Tier 2: Root Cause (15-30 min)** - Logic flow trace, integration checks
   - **Tier 3: Deep Research (30-60 min)** - Invoke `deep-research` skill (Phase 7 - stub for now)
   - Present findings with root cause, affected files, recommended approach
   - Wait for approval before implementing

6. Implement Phase 1: Pre-Implementation Checks:
   - Read CHANGELOG.md, PLUGINS.md, recent commits
   - Identify current version
   - Determine version bump:
     - PATCH (v1.2 → v1.2.1): Bug fixes, cosmetic
     - MINOR (v1.2 → v1.3): New features, enhancements
     - MAJOR (v1.2 → v2.0): Breaking changes (parameter IDs changed, parameters removed, state format changed)
   - If breaking change: Warn user and confirm major version bump

7. Implement Phase 2: Backup Creation:
   ```bash
   cp -r plugins/[PluginName] backups/[PluginName]-v[X.Y]-$(date +%Y%m%d-%H%M%S)
   ```

8. Implement Phase 3: Implementation:
   - Modify source files according to approved approach
   - Update build config if needed
   - Adjust UI if required

9. Implement Phase 4: CHANGELOG Update:
   - Use Keep a Changelog format
   - Add entry: `## [X.Y.Z] - YYYY-MM-DD`
   - Sections: Added, Changed, Fixed

10. Implement Phase 5: Build & Test:
    - Invoke `build-automation` skill (Phase 4 - stub for now, use manual build)
    - Invoke `plugin-testing` skill (Phase 1b Task 8)
    - Offer manual testing checklist

11. Implement Phase 6: Git Workflow:
    ```bash
    git add plugins/[PluginName]/
    git commit -m "improve: [PluginName] v[X.Y.Z] - [description]"
    git tag -a v[X.Y.Z] -m "[PluginName] v[X.Y.Z]"
    ```

12. Implement Phase 7: Installation:
    - Decision menu: "Build successful. Install to system folders?"
    - If yes: Invoke `plugin-lifecycle` skill (Phase 1b Task 9)
    - Update PLUGINS.md with new version

13. Add decision menu after completion:
    ```
    ✓ [Plugin] v[X.Y.Z] complete

    What's next?
    1. Test in DAW
    2. Make another improvement
    3. Create new plugin
    4. Other
    ```

**Expected Output**: `.claude/skills/plugin-improve/SKILL.md` (complete skill file)

**Verification**:
- Automated:
  ```bash
  # Verify skill file exists with investigation tiers
  test -f .claude/skills/plugin-improve/SKILL.md && echo "✓ plugin-improve skill exists" || echo "✗ Missing"
  grep -q "Tier 1" .claude/skills/plugin-improve/SKILL.md && echo "✓ Investigation tiers documented" || echo "✗ Missing tiers"
  grep -q "CHANGELOG" .claude/skills/plugin-improve/SKILL.md && echo "✓ CHANGELOG update documented" || echo "✗ Missing CHANGELOG"
  ```
- Manual: STOP AND ASK LEX: "Please verify that `.claude/skills/plugin-improve/SKILL.md` includes vagueness detection logic and presents a 'Brainstorm first or Implement with assumptions' menu for vague requests."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 6: Create context-resume Skill

**Description**: Implement the universal resume system that loads plugin development context from `.continue-here.md` handoff files to enable pause/resume across sessions.

**Required Reading**:
- @procedures/skills/context-resume.md (sections: all)
- @architecture/06-state-architecture.md (sections: lines 62-154, handoff file format)
- @procedures/core/checkpoint-system.md (sections: resume commands)

**Dependencies**: Task 3 (skill directory structure established)

**Implementation Steps**:

1. Create `.claude/skills/context-resume/` directory

2. Create `.claude/skills/context-resume/SKILL.md` with:
   - Frontmatter:
     ```yaml
     ---
     name: context-resume
     description: Load plugin context from handoff files to resume work
     allowed-tools:
       - Read
       - Bash
       - Skill  # To invoke next skill
     preconditions:
       - Handoff file must exist in one of 3 locations
     ---
     ```

3. Implement Step 1: Locate Handoff:
   - Search 3 locations:
     - `plugins/[PluginName]/.continue-here.md` (main workflow)
     - `plugins/[PluginName]/.ideas/.continue-here.md` (ideation/mockup)
     - `plugins/[PluginName]/.ideas/mockups/.continue-here.md` (mockup iterations)
   - If no plugin name provided: List all resumable plugins with context (stage, status, time ago)
   - If multiple handoffs: Ask which context to resume

4. Implement Step 2: Parse Context:
   - Read YAML frontmatter: plugin name, stage, phase, status, last_updated, complexity_score
   - Read markdown body: current state, completed work, next steps, context to preserve
   - Extract: last action, suggested next steps, files modified, build status, research references

5. Implement Step 3: Summarize:
   ```
   Resuming [PluginName]:

   Status: Stage 4 complete, DSP implemented
   Last session: Core audio processing finished, tested stable

   Next steps:
   1. ⭐ Start Stage 5 (GUI implementation)
   2. Review Stage 4 code
   3. Run additional tests

   Ready to proceed with Stage 5?
   ```

6. Implement Step 4: Proceed:
   - Parse "Next Steps" to determine which skill to invoke
   - Load relevant files if mentioned in handoff
   - Continue automation from documented next action
   - Invoke appropriate skill:
     - If resuming workflow: Invoke `plugin-workflow` at current stage
     - If resuming ideation: Offer `ui-mockup` or `plugin-workflow`
     - If resuming mockup: Offer `plugin-workflow` Stage 5 or `ui-mockup` iteration
     - If resuming improvement: Offer `plugin-improve` or `plugin-ideation`

7. Implement error handling:
   - **No handoff found:**
     - Check plugin directory exists
     - Show last commit
     - Best guess at stage from git log
     - Options: (1) Start from next stage, (2) Review code, (3) Start fresh
   - **Ambiguous state (multiple handoffs):**
     - List all handoff files with context
     - Ask which to resume
   - **Corrupted handoff:**
     - Use git log to infer state
     - Offer to reconstruct context from commits

8. Add decision menu based on resume context:
   - Completion statement varies by context
   - Options adapt to current stage/phase
   - Always include "Other" option

**Expected Output**: `.claude/skills/context-resume/SKILL.md` (complete skill file)

**Verification**:
- Automated:
  ```bash
  # Verify skill file exists with handoff parsing logic
  test -f .claude/skills/context-resume/SKILL.md && echo "✓ context-resume skill exists" || echo "✗ Missing"
  grep -q "\.continue-here\.md" .claude/skills/context-resume/SKILL.md && echo "✓ Handoff file parsing documented" || echo "✗ Missing handoff logic"
  grep -q "YAML" .claude/skills/context-resume/SKILL.md && echo "✓ YAML parsing documented" || echo "✗ Missing YAML logic"
  ```
- Manual: STOP AND ASK LEX: "Please verify that `.claude/skills/context-resume/SKILL.md` includes logic to search all 3 handoff locations and present an interactive menu if no plugin name is provided."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 7: Create ui-mockup Skill

**Description**: Implement the two-phase UI mockup system that generates production-ready WebView UIs. The HTML generated IS the plugin UI, not a throwaway prototype.

**Required Reading**:
- @procedures/skills/ui-mockup.md (sections: all)
- @procedures/webview/README.md (sections: ⚠️ START HERE)
- @procedures/webview/best-practices.md (sections: member order, resource provider)

**Dependencies**: Task 3 (skill directory structure established)

**Implementation Steps**:

1. Create `.claude/skills/ui-mockup/` directory

2. Create `.claude/skills/ui-mockup/SKILL.md` with:
   - Frontmatter:
     ```yaml
     ---
     name: ui-mockup
     description: Generate production-ready WebView UI mockups (7 files per version)
     allowed-tools:
       - Read
       - Write
     preconditions:
       - None (can work standalone or with creative brief)
     ---
     ```

3. Implement Phase 1: Load Context:
   - Check for creative brief: `plugins/[Name]/.ideas/creative-brief.md`
   - Check for improvement proposals: `plugins/[Name]/.ideas/improvements/`
   - Check for existing implementation (redesigns)
   - Extract: plugin type, parameters, UI vision, colors, special elements

4. Implement Phase 1.5: Free-Form UI Vision:
   - Ask: "What should the UI look like?"
   - Capture design preferences before specific questions
   - Adaptive listening (don't ask what's already mentioned)

5. Implement Phase 2: Targeted Design Questions:
   - Ask only about gaps:
     - Window size (if not mentioned)
     - Layout organization (if not described)
     - Control style (if not specified)
     - Color scheme (if not provided)

6. Implement Phase 3: Generate Hierarchical YAML:
   ```yaml
   window:
     width: 600
     height: 400

   colors:
     background: "#2b2b2b"
     primary: "#4a9eff"

   sections:
     main_controls:
       layout: grid
       components:
         - type: rotary_slider
           id: gain
           range: [0.0, 1.0]
   ```

7. Implement Phase 4: Generate Browser Test HTML:
   - Self-contained HTML for browser testing (no JUCE)
   - Mock parameter state with console logging
   - Interactive testing without building
   - Same visual as production will be

8. **CRITICAL:** Implement Phase 4.5: Present Design Decision Menu and STOP:
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
   ```
   - WAIT for user response
   - Option 1: Iterate (back to Phase 3, new version)
   - Option 2: Finalize (continue to Phase 5-8)
   - Option 3: Open test HTML
   - **Only proceed to Phase 5-8 if user chose option 2**

9. Implement Phase 5: Generate Production HTML (After Finalization Only):
   - Self-contained HTML with JUCE integration
   - Import from `'./js/juce/index.js'`
   - Use `getSliderState(id)`, `getToggleState(id)`, `getComboBoxState(id)`
   - Parameter binding: `state.setNormalisedValue()` and `state.valueChangedEvent.addListener()`
   - Native feel: Disable text selection, right-click, use default cursor

10. Implement Phase 6: Generate C++ Boilerplate (After Finalization Only):
   - `v[N]-editor.h` - Header with **CORRECT member order:**
     ```cpp
     // 1. RELAYS FIRST (no dependencies)
     juce::WebSliderRelay gainRelay;
     juce::WebToggleButtonRelay bypassRelay;

     // 2. WEBVIEW SECOND (depends on relays)
     juce::WebBrowserComponent webView;

     // 3. ATTACHMENTS LAST (depend on both)
     juce::WebSliderParameterAttachment gainAttachment;
     juce::WebToggleButtonParameterAttachment bypassAttachment;
     ```
   - `v[N]-editor.cpp` - Resource provider and parameter attachments

11. Implement Phase 7: Generate Build Configuration (After Finalization Only):
    - CMake snippets for WebView feature flags
    - `juce_add_binary_data` for ui/ directory
    - Platform-specific WebView setup

12. Implement Phase 8: Generate Implementation Checklist (After Finalization Only):
    - Step-by-step guide for Stage 5 implementation
    - File copy instructions
    - Build verification steps

13. File Generation Summary:
    - **Design phase (Phases 3-4):** 2 files (YAML + test HTML)
    - **Implementation phase (Phases 5-8, after finalization):** 5 additional files (production HTML + C++ boilerplate + build config + checklist)
    - **Total per finalized mockup version:** 7 files
    - **Versions not finalized:** Only 2 files (design artifacts)
    ```
    plugins/[PluginName]/.ideas/mockups/
    ├── v[N]-ui.yaml                      # Specification
    ├── v[N]-ui.html                      # Production HTML (JUCE integrated)
    ├── v[N]-ui-test.html                 # Browser test version (no JUCE)
    ├── v[N]-editor.h                     # C++ header boilerplate
    ├── v[N]-editor.cpp                   # C++ implementation boilerplate
    ├── v[N]-cmake.txt                    # CMakeLists.txt snippets
    └── v[N]-implementation-steps.md      # Implementation checklist
    ```

14. Implement versioning:
    - First mockup: v1
    - Iteration request: v2 (reads v1 as baseline, generates only design files until finalized)
    - Further iterations: v3, v4, etc.
    - All versions preserved (idea lineage)
    - Implementation files only generated for finalized version

15. Implement parameter-spec.md generation:
    - After mockup finalized, extract all parameters
    - Generate `plugins/[Name]/.ideas/parameter-spec.md` with:
      - Parameter IDs (camelCase)
      - Types (Float/Choice/Bool)
      - Ranges, defaults, skew factors
      - UI controls mapped
      - DSP usage descriptions
    - Mark as CRITICAL CONTRACT (Stage 1 blocks if missing)

16. Iteration handling:
    - If user chooses "Provide refinements" (option 1): Return to Phase 3 with new version number
    - If user chooses "Finalize" (option 2): Execute Phases 5-8
    - Design files can iterate rapidly without touching implementation scaffolding

**Expected Output**: `.claude/skills/ui-mockup/SKILL.md` (complete skill file)

**Verification**:
- Automated:
  ```bash
  # Verify skill file exists with 7-file generation logic
  test -f .claude/skills/ui-mockup/SKILL.md && echo "✓ ui-mockup skill exists" || echo "✗ Missing"
  for file in "yaml" "ui.html" "ui-test.html" "editor.h" "editor.cpp" "cmake.txt" "implementation-steps.md"; do
    grep -q "$file" .claude/skills/ui-mockup/SKILL.md && echo "✓ $file generation documented" || echo "✗ $file missing"
  done
  grep -q "parameter-spec.md" .claude/skills/ui-mockup/SKILL.md && echo "✓ parameter-spec generation documented" || echo "✗ Missing parameter-spec"
  ```
- Manual: STOP AND ASK LEX: "Please verify that `.claude/skills/ui-mockup/SKILL.md` includes Phase 4.5 decision menu that STOPS after generating 2 design files and waits for user approval before generating the 5 implementation files."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 8: Create plugin-testing Skill

**Description**: Implement the automated stability testing system that catches crashes, parameter issues, and state bugs in 2 minutes. This skill provides 3 test modes: automated, build+pluginval, and manual checklist.

**Required Reading**:
- @procedures/skills/plugin-testing.md (sections: all)

**Dependencies**: Task 3 (skill directory structure established)

**Implementation Steps**:

1. Create `.claude/skills/plugin-testing/` directory

2. Create `.claude/skills/plugin-testing/SKILL.md` with:
   - Frontmatter:
     ```yaml
     ---
     name: plugin-testing
     description: Automated stability tests and validation suite
     allowed-tools:
       - Read
       - Bash
       - Task  # For deep-research on failures
     preconditions:
       - Plugin must exist
       - Plugin status must NOT be 💡 (must have implementation)
     ---
     ```

3. Implement Mode 1: Automated Testing (~2 min):
   - Check for `plugins/[Plugin]/Tests/` directory
   - If exists: Run 5 tests:
     - **Test 1: Parameter Response** - All parameters control audio
     - **Test 2: State Save/Load** - Parameters restore correctly
     - **Test 3: Silent Input Handling** - Output stays silent/reasonable
     - **Test 4: Feedback Loop Detection** - Output doesn't explode
     - **Test 5: CPU Performance** - Real-time factor < 0.1
   - Generate report:
     ```
     ✅ All tests PASSED (5/5)

     Parameter response: PASS (5/5 parameters)
     State save/load: PASS
     Silent input: PASS (output RMS: 0.0)
     Feedback test: PASS (max output: 0.85)
     CPU performance: PASS (real-time factor: 0.03)
     ```
   - If fails: Present menu (1) Investigate, (2) Show test code, (3) Show output, (4) Manual fix

4. Implement Mode 2: Build + Pluginval (~5-10 min):
   - Clean build (remove old files)
   - Rebuild from scratch (Release mode, parallel VST3+AU)
   - Verify no compiler warnings
   - Run pluginval:
     ```bash
     pluginval --validate [path] --strictness-level 10 --timeout-ms 30000 --verbose
     ```
   - Parse output for pass/fail
   - If fails: Present menu with investigation options

5. Implement Mode 3: Manual DAW Testing:
   - Display comprehensive checklist:
     - Setup (load in DAW)
     - Parameter testing (move each, verify changes)
     - Automation (draw, playback, record)
     - Preset recall (save, load)
     - Project save/load
     - Edge cases (sample rate, buffer size)
     - Sonic quality (test signal, real audio)
   - User confirms completion

6. Create test report logging:
   - Save to `logs/[PluginName]/test_[timestamp].log`
   - Include: test results, failures, timestamps

7. Add decision menu after testing:
   ```
   ✓ All tests passed

   What's next?
   1. Continue to next stage (if in workflow)
   2. Install plugin (if Stage 6 complete)
   3. Review test results
   4. Other
   ```

**Expected Output**: `.claude/skills/plugin-testing/SKILL.md` (complete skill file)

**Verification**:
- Automated:
  ```bash
  # Verify skill file exists with 3 test modes
  test -f .claude/skills/plugin-testing/SKILL.md && echo "✓ plugin-testing skill exists" || echo "✗ Missing"
  for mode in "Automated" "pluginval" "Manual"; do
    grep -q "$mode" .claude/skills/plugin-testing/SKILL.md && echo "✓ $mode mode documented" || echo "✗ $mode missing"
  done
  ```
- Manual: STOP AND ASK LEX: "Please verify that `.claude/skills/plugin-testing/SKILL.md` includes all 5 automated tests (parameter response, state save/load, silent input, feedback loop, CPU performance) with pass criteria."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 9: Create plugin-lifecycle Skill

**Description**: Implement the installation management system that handles plugin deployment to macOS system folders with proper permissions, cache clearing, and verification.

**Required Reading**:
- @procedures/skills/plugin-lifecycle.md (sections: all)

**Dependencies**: Task 3 (skill directory structure established)

**Implementation Steps**:

1. Create `.claude/skills/plugin-lifecycle/` directory

2. Create `.claude/skills/plugin-lifecycle/SKILL.md` with:
   - Frontmatter:
     ```yaml
     ---
     name: plugin-lifecycle
     description: Install/remove plugins from macOS system folders
     allowed-tools:
       - Bash
       - Read
       - Edit  # For PLUGINS.md updates
     preconditions:
       - Plugin must have successful Release build
     ---
     ```

3. Implement Step 1: Build Verification:
   - Check for binaries:
     ```
     build/plugins/[PluginName]/[PluginName]_artefacts/Release/VST3/[ProductName].vst3
     build/plugins/[PluginName]/[PluginName]_artefacts/Release/AU/[ProductName].component
     ```
   - If missing: Offer to invoke `build-automation` (Phase 4 - stub for now, manual build)

4. Implement Step 2: Product Name Extraction:
   - Read CMakeLists.txt
   - Extract PRODUCT_NAME (may contain spaces)
   - Use product name for installation filename

5. Implement Step 3: Old Version Removal:
   ```bash
   find ~/Library/Audio/Plug-Ins/VST3/ -name "[ProductName].vst3"
   find ~/Library/Audio/Plug-Ins/Components/ -name "[ProductName].component"
   ```
   - If found: Remove before installing

6. Implement Step 4: Copy to System Folders:
   ```bash
   cp -R build/.../[ProductName].vst3 ~/Library/Audio/Plug-Ins/VST3/
   cp -R build/.../[ProductName].component ~/Library/Audio/Plug-Ins/Components/
   ```

7. Implement Step 5: Permissions Verification:
   ```bash
   chmod -R 755 ~/Library/Audio/Plug-Ins/VST3/[ProductName].vst3
   chmod -R 755 ~/Library/Audio/Plug-Ins/Components/[ProductName].component
   ```

8. Implement Step 6: Cache Clearing:
   - Ableton Live:
     ```bash
     rm -f ~/Library/Preferences/Ableton/Live\ */PluginCache.db
     ```
   - Logic Pro:
     ```bash
     rm -rf ~/Library/Caches/AudioUnitCache/*
     ```

9. Implement Step 7: Verification:
   ```bash
   ls -lh ~/Library/Audio/Plug-Ins/VST3/[ProductName].vst3
   ls -lh ~/Library/Audio/Plug-Ins/Components/[ProductName].component
   ```
   - Show timestamps (must be <60s for fresh install)
   - Show sizes

10. Implement removal process (uninstall):
    - Locate plugin files
    - Confirm removal
    - Remove files
    - Clear caches
    - Verify removal
    - Does NOT remove source files, only system-installed binaries

11. Update PLUGINS.md after installation:
    - Change status: ✅ Working → 📦 Installed
    - Add installed date
    - Add locations (VST3/AU paths)

12. Add decision menu after installation:
    ```
    ✓ [Plugin] installed successfully

    What's next?
    1. Test in DAW (recommended)
    2. Create another plugin
    3. Document this plugin
    4. Share plugin (export build)
    5. Other
    ```

**Expected Output**: `.claude/skills/plugin-lifecycle/SKILL.md` (complete skill file)

**Verification**:
- Automated:
  ```bash
  # Verify skill file exists with installation logic
  test -f .claude/skills/plugin-lifecycle/SKILL.md && echo "✓ plugin-lifecycle skill exists" || echo "✗ Missing"
  grep -q "~/Library/Audio/Plug-Ins" .claude/skills/plugin-lifecycle/SKILL.md && echo "✓ System folder paths documented" || echo "✗ Missing paths"
  grep -q "chmod" .claude/skills/plugin-lifecycle/SKILL.md && echo "✓ Permissions documented" || echo "✗ Missing permissions"
  grep -q "Cache" .claude/skills/plugin-lifecycle/SKILL.md && echo "✓ Cache clearing documented" || echo "✗ Missing cache logic"
  ```
- Manual: STOP AND ASK LEX: "Please verify that `.claude/skills/plugin-lifecycle/SKILL.md` includes cache clearing for both Ableton Live and Logic Pro, and that it updates PLUGINS.md status from ✅ Working to 📦 Installed."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

### Phase 1c: Interactive & Handoff Systems

#### Task 10: Verify Decision Menu Integration

**Description**: Review all 7 skill files and verify that decision menus are properly integrated at all checkpoint boundaries using the inline numbered list format (not AskUserQuestion).

**Required Reading**:
- @procedures/core/interactive-decision-system.md (sections: all)

**Dependencies**: Tasks 3-9 (all 7 skills must exist)

**Implementation Steps**:

1. Read all 7 skill files in `.claude/skills/`

2. For each skill, verify decision menus exist at:
   - After completion (all skills)
   - Before starting (skills with multiple modes: plugin-ideation, ui-mockup)
   - At stage boundaries (plugin-workflow)
   - On failures (plugin-testing)
   - After investigation (plugin-improve)

3. Verify decision menu format:
   ```
   ✓ [Completion statement]

   What's next?
   1. [Primary action] (recommended)
   2. [Secondary action]
   3. [Discovery feature] ← User discovers new capability
   4. [Alternative path]
   5. Other

   Choose (1-5): _
   ```

4. Check for required elements:
   - Completion marker (✓ or ✗)
   - Numbered options (not AskUserQuestion)
   - "(recommended)" marker on primary action
   - "← User discovers [feature]" markers on hidden features
   - "Other" as last option
   - Clear choice prompt

5. Verify option ordering:
   - Primary action first
   - Secondary common actions
   - Discovery features with markers
   - Alternative paths
   - "Other" last

6. Check context-aware option generation:
   - Options adapt to plugin status (💡/🚧/✅/📦)
   - Options adapt to current stage/phase
   - Options adapt to available resources (templates, mockups)
   - Options adapt to error context

**Expected Output**: Verification report confirming all decision menus follow patterns

**Verification**:
- Automated:
  ```bash
  # Verify decision menus exist in all skills
  echo "=== Decision Menu Verification ==="
  for skill in plugin-ideation plugin-workflow plugin-improve context-resume ui-mockup plugin-testing plugin-lifecycle; do
    if grep -q "What's next?" ".claude/skills/$skill/SKILL.md"; then
      echo "✓ $skill has decision menus"
    else
      echo "✗ $skill missing decision menus"
    fi
  done

  # Verify "(recommended)" markers exist
  for skill in plugin-ideation plugin-workflow plugin-improve context-resume ui-mockup plugin-testing plugin-lifecycle; do
    if grep -q "(recommended)" ".claude/skills/$skill/SKILL.md"; then
      echo "✓ $skill has recommendation markers"
    else
      echo "⚠ $skill missing recommendation markers"
    fi
  done
  ```
- Manual: STOP AND ASK LEX: "Please review `.claude/skills/plugin-workflow/SKILL.md` and verify that decision menus appear after each stage boundary (Stages 0-6) with context-appropriate options (e.g., 'Continue to Stage N' as primary action)."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 11: Verify Handoff File Integration

**Description**: Review plugin-workflow and other relevant skills to verify that `.continue-here.md` handoff files are created and updated at all checkpoint boundaries with proper YAML frontmatter and markdown body structure.

**Required Reading**:
- @architecture/06-state-architecture.md (sections: lines 62-154, handoff format)
- @procedures/core/checkpoint-system.md (sections: handoff file system)

**Dependencies**: Tasks 3-9 (all skills must exist)

**Implementation Steps**:

1. Verify handoff file creation in plugin-workflow:
   - After Stage 0 completion: Create `plugins/[Name]/.continue-here.md`
   - Update after each stage (0-6)
   - Delete after Stage 6 (workflow complete)

2. Verify YAML frontmatter structure:
   ```yaml
   ---
   plugin: PluginName
   stage: N
   phase: M  # Optional, for complex plugins
   status: in_progress
   last_updated: YYYY-MM-DD HH:MM:SS
   complexity_score: X.Y  # Optional
   phased_implementation: true/false  # Optional
   ---
   ```

3. Verify markdown body structure:
   ```markdown
   # Resume Point

   ## Current State: Stage N - [Description]

   [Prose description]

   ## Completed So Far

   **Stage 0-X:** ✓ Complete
   - [List of completed work]

   **Stage N:** 🚧 In Progress
   - [What's done]
   - [What's next]

   ## Next Steps

   1. [Specific next action]
   2. [Following action]

   ## Context to Preserve

   **Key Decisions:**
   - [Design choices]

   **Files Modified:**
   - [File paths]

   **Current Build Status:**
   - Last build: [Success/Failed]

   **Research References:**
   - [Links to docs]
   ```

4. Verify handoff file locations in skills:
   - plugin-workflow: `plugins/[Name]/.continue-here.md`
   - plugin-ideation: `plugins/[Name]/.ideas/.continue-here.md`
   - ui-mockup: `plugins/[Name]/.ideas/mockups/.continue-here.md`

5. Verify context-resume skill can parse all 3 locations

6. Verify update triggers:
   - After every stage completion
   - After every phase completion (complex plugins)
   - Before presenting decision menu
   - After user makes choice at checkpoint
   - On build failures
   - On manual pause

**Expected Output**: Verification report confirming handoff integration

**Verification**:
- Automated:
  ```bash
  # Verify handoff file references in skills
  echo "=== Handoff File Integration Verification ==="
  grep -q "\.continue-here\.md" .claude/skills/plugin-workflow/SKILL.md && echo "✓ plugin-workflow creates handoff files" || echo "✗ Missing handoff creation"
  grep -q "\.continue-here\.md" .claude/skills/context-resume/SKILL.md && echo "✓ context-resume parses handoff files" || echo "✗ Missing handoff parsing"
  grep -q "YAML" .claude/skills/context-resume/SKILL.md && echo "✓ YAML frontmatter parsing documented" || echo "✗ Missing YAML parsing"
  ```
- Manual: STOP AND ASK LEX: "Please verify that `.claude/skills/plugin-workflow/SKILL.md` includes instructions to create `.continue-here.md` after Stage 0 with YAML frontmatter containing plugin name, stage, status, and last_updated fields."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

#### Task 12: Update CLAUDE.md with Phase 1 Completion

**Description**: Update `.claude/CLAUDE.md` to mark Phase 1 complete and indicate Phase 2 as next.

**Required Reading**:
- @architecture/02-core-abstractions.md (sections: lines 5-47, CLAUDE.md specification)

**Dependencies**: Tasks 1-11 (all Phase 1 components must exist)

**Implementation Steps**:

1. Read current `.claude/CLAUDE.md`

2. Update Implementation Status section:
   ```markdown
   ## Implementation Status
   - ✓ Phase 0: Foundation & Contracts (complete)
   - ✓ Phase 1: Discovery System (complete)
   - Phase 2: Workflow Engine (next)
   ```

3. Keep file under 300 words total

4. Do NOT add detailed descriptions of skills or commands (they live in their respective files)

**Expected Output**: Updated `.claude/CLAUDE.md`

**Verification**:
- Automated:
  ```bash
  # Verify Phase 1 marked complete
  grep -q "Phase 1: Discovery System (complete)" .claude/CLAUDE.md && echo "✓ Phase 1 marked complete" || echo "✗ Not marked complete"

  # Verify word count
  word_count=$(wc -w < .claude/CLAUDE.md)
  if [ "$word_count" -lt 300 ]; then
    echo "✓ CLAUDE.md is concise ($word_count words)"
  else
    echo "⚠ CLAUDE.md is verbose ($word_count words, target <300)"
  fi
  ```
- Manual: STOP AND ASK LEX: "Please confirm that `.claude/CLAUDE.md` now shows Phase 1 as complete and Phase 2 as next, and that the file remains under 300 words."

**CRITICAL:** After specifying manual verification, the implementation MUST pause and wait for Lex's confirmation before proceeding to the next task. Manual verification is a blocking checkpoint, not optional documentation.

---

## Comprehensive Verification Criteria

### Automated Tests

```bash
# Test 1: Verify all 9 commands exist with frontmatter
echo "=== Command Files Verification ==="
for cmd in dream implement improve continue test install-plugin show-standalone troubleshoot-juce doc-fix; do
  if [ -f ".claude/commands/$cmd.md" ]; then
    if grep -q "^---$" ".claude/commands/$cmd.md"; then
      echo "✓ .claude/commands/$cmd.md exists with frontmatter"
    else
      echo "⚠ .claude/commands/$cmd.md exists but missing frontmatter"
    fi
  else
    echo "✗ .claude/commands/$cmd.md missing"
  fi
done

# Test 2: Verify all 7 skills exist with frontmatter
echo -e "\n=== Skill Files Verification ==="
for skill in plugin-ideation plugin-workflow plugin-improve context-resume ui-mockup plugin-testing plugin-lifecycle; do
  if [ -f ".claude/skills/$skill/SKILL.md" ]; then
    if grep -q "^---$" ".claude/skills/$skill/SKILL.md"; then
      echo "✓ .claude/skills/$skill/SKILL.md exists with frontmatter"
    else
      echo "⚠ .claude/skills/$skill/SKILL.md exists but missing frontmatter"
    fi
  else
    echo "✗ .claude/skills/$skill/SKILL.md missing"
  fi
done

# Test 3: Verify command routing
echo -e "\n=== Command Routing Verification ==="
grep -l "plugin-ideation" .claude/commands/dream.md && echo "✓ /dream routes to plugin-ideation"
grep -l "plugin-workflow" .claude/commands/implement.md && echo "✓ /implement routes to plugin-workflow"
grep -l "plugin-improve" .claude/commands/improve.md && echo "✓ /improve routes to plugin-improve"
grep -l "context-resume" .claude/commands/continue.md && echo "✓ /continue routes to context-resume"
grep -l "plugin-testing" .claude/commands/test.md && echo "✓ /test routes to plugin-testing"
grep -l "plugin-lifecycle" .claude/commands/install-plugin.md && echo "✓ /install-plugin routes to plugin-lifecycle"

# Test 4: Verify decision menus in skills
echo -e "\n=== Decision Menu Verification ==="
for skill in plugin-ideation plugin-workflow plugin-improve context-resume ui-mockup plugin-testing plugin-lifecycle; do
  if grep -q "What's next?" ".claude/skills/$skill/SKILL.md"; then
    echo "✓ $skill has decision menus"
  else
    echo "✗ $skill missing decision menus"
  fi
done

# Test 5: Verify handoff file integration
echo -e "\n=== Handoff File Integration Verification ==="
grep -q "\.continue-here\.md" .claude/skills/plugin-workflow/SKILL.md && echo "✓ plugin-workflow creates handoff files" || echo "✗ Missing handoff creation"
grep -q "\.continue-here\.md" .claude/skills/context-resume/SKILL.md && echo "✓ context-resume parses handoff files" || echo "✗ Missing handoff parsing"
grep -q "YAML" .claude/skills/context-resume/SKILL.md && echo "✓ YAML frontmatter parsing documented" || echo "✗ Missing YAML parsing"

# Test 6: Verify CLAUDE.md updated
echo -e "\n=== CLAUDE.md Update Verification ==="
grep -q "Phase 1: Discovery System (complete)" .claude/CLAUDE.md && echo "✓ Phase 1 marked complete" || echo "✗ Not marked complete"

echo -e "\n=== Phase 1 Verification Complete ==="
```

### Manual Verification Checklist

- [ ] When I type `/` in Claude Code, does autocomplete show all 9 commands?
- [ ] Can you run `/dream TestIdea` and does it present an interactive decision menu after creating the creative brief?
- [ ] Can you run `/implement TestPlugin` (without a creative brief) and does it block with a message asking to run `/dream` first?
- [ ] Does `.claude/skills/plugin-workflow/SKILL.md` include decision menus after each stage boundary (Stages 0-6)?
- [ ] Does `.claude/skills/plugin-workflow/SKILL.md` include instructions to create `.continue-here.md` after Stage 0?
- [ ] Does `.claude/skills/context-resume/SKILL.md` search all 3 handoff locations (`plugins/[Name]/.continue-here.md`, `plugins/[Name]/.ideas/.continue-here.md`, `plugins/[Name]/.ideas/mockups/.continue-here.md`)?
- [ ] Does `.claude/skills/ui-mockup/SKILL.md` include Phase 4.5 decision menu that stops after 2 design files?
- [ ] Does `.claude/skills/ui-mockup/SKILL.md` generate 2 files in design phase, then 5 files only after user confirmation?
- [ ] Does `.claude/skills/plugin-testing/SKILL.md` include all 5 automated tests with pass criteria?
- [ ] Does `.claude/skills/plugin-lifecycle/SKILL.md` include cache clearing for both Ableton Live and Logic Pro?
- [ ] Does `.claude/CLAUDE.md` show Phase 1 as complete and remain under 300 words?

### File Existence Verification

```bash
# Verify all expected files exist
echo "=== Complete File Structure Verification ==="

# Check command files (9 total)
commands=(dream implement improve continue test install-plugin show-standalone troubleshoot-juce doc-fix)
for cmd in "${commands[@]}"; do
  ls -la ".claude/commands/$cmd.md" 2>/dev/null || echo "✗ .claude/commands/$cmd.md missing"
done

# Check skill directories (7 total)
skills=(plugin-ideation plugin-workflow plugin-improve context-resume ui-mockup plugin-testing plugin-lifecycle)
for skill in "${skills[@]}"; do
  ls -la ".claude/skills/$skill/SKILL.md" 2>/dev/null || echo "✗ .claude/skills/$skill/SKILL.md missing"
done

# Check CLAUDE.md updated
ls -la ".claude/CLAUDE.md" 2>/dev/null || echo "✗ .claude/CLAUDE.md missing"
```

### Integration Tests

**Test 1: End-to-End Command Invocation**
```
User types: /dream TestPlugin
Expected: plugin-ideation skill loads, asks "What would you like to build?"
Result: [PASS/FAIL]
```

**Test 2: Precondition Enforcement**
```
User types: /implement TestPlugin (no creative brief exists)
Expected: Blocks with message "Creative brief missing. Run /dream first."
Result: [PASS/FAIL]
```

**Test 3: Decision Menu Presentation**
```
User completes: /dream TestPlugin workflow
Expected: Decision menu with options:
1. Create UI mockup (recommended)
2. Start implementation
3. Save for later
4. Other
Result: [PASS/FAIL]
```

**Test 4: Context Resume**
```
Create .continue-here.md manually in plugins/TestPlugin/
User types: /continue TestPlugin
Expected: Parses handoff, summarizes context, offers to resume
Result: [PASS/FAIL]
```

### Success Criteria

Phase 1 is COMPLETE when:

1. All 9 slash command files exist in `.claude/commands/` with proper frontmatter
2. Typing `/` in Claude Code shows all 9 commands in autocomplete
3. All 7 skill files exist in `.claude/skills/*/SKILL.md` with proper frontmatter
4. Each command correctly routes to its target skill via instructed routing
5. All skills include decision menus at appropriate checkpoints using inline numbered lists
6. plugin-workflow creates `.continue-here.md` with YAML frontmatter after Stage 0
7. context-resume can parse handoff files from all 3 locations
8. ui-mockup generates all 7 files per mockup version
9. ui-mockup generates parameter-spec.md after finalization
10. plugin-workflow includes stage boundary protocol (completion, commit, handoff update, decision menu)
11. All automated tests pass without errors
12. All manual verification items are confirmed
13. `.claude/CLAUDE.md` marks Phase 1 as complete

---

## Potential Issues & Mitigations

### Issue 1: Skills too verbose, exceeding Claude Code limits

**Symptom**: Skill files are very long, may hit token limits when loaded

**Root Cause**: Comprehensive workflow specifications packed into single files

**Mitigation**:
- Keep skills focused on orchestration logic, not implementation details
- Reference procedure files for detailed specifications
- Use clear section headers for navigation
- If skill exceeds 1000 lines, consider splitting into sub-skills

### Issue 2: Decision menus not presenting at checkpoints

**Symptom**: Skills complete tasks without stopping for user input

**Root Cause**: Decision menu format not recognized as stopping point

**Mitigation**:
- Use exact format: "What's next?" with numbered options
- Always include completion marker (✓ or ✗)
- Ensure "Choose (1-N):" prompt is present
- Test manually after implementation

### Issue 3: Handoff files not created or parsed correctly

**Symptom**: `/continue` command can't find or load context

**Root Cause**: Incorrect file paths or YAML parsing issues

**Mitigation**:
- Verify exact paths: `plugins/[Name]/.continue-here.md`
- Test YAML frontmatter with actual files
- Ensure YAML starts with `---` and ends with `---`
- Use `yq` or similar tool to validate YAML syntax

### Issue 4: Command routing fails, skills not invoked

**Symptom**: Commands expand but skills don't load

**Root Cause**: Skill tool invocation syntax incorrect

**Mitigation**:
- Use explicit instruction: "invoke the X skill"
- Verify skill names match exactly (plugin-ideation, not PluginIdeation)
- Test with simple invocation first before complex logic
- Check CLAUDE.md references correct skill paths

### Issue 5: Complexity score calculation unclear

**Symptom**: plugin-workflow can't determine if phasing is needed

**Root Cause**: Formula not implemented or parameters not counted correctly

**Mitigation**:
- Implement explicit formula: `min(params/5, 2) + algorithms + features`
- Cap at 5
- Default to single-pass if uncertain
- Document score calculation in plan.md

### Issue 6: Stage stubs cause confusion

**Symptom**: Stages 2-5 don't work, users confused about manual vs automated

**Root Cause**: Subagents not implemented yet (Phase 3)

**Mitigation**:
- Clear "STUB" markers in skill: "Stage 2 requires foundation-agent (Phase 3)"
- Provide manual implementation instructions as fallback
- Update Phase 1 skills when Phase 3 complete
- Document limitations in CLAUDE.md or plan

---

## Notes for Next Phase

### Phase 2 Prerequisites

Phase 2 (Workflow Engine) requires:
- ✓ All 9 commands exist (Phase 1a)
- ✓ All 7 skills exist (Phase 1b)
- ✓ Decision menu system integrated (Phase 1c)
- ✓ Handoff file system implemented (Phase 1c)
- ✓ Command routing functional (Phase 1a)

### Phase 2 Expectations

Phase 2 will:
- Implement Stages 0, 1, 6 of plugin-workflow in full detail (currently stubs)
- Add contract enforcement in Stage 1 (blocks if parameter-spec.md or architecture.md missing)
- Add complexity scoring logic
- Add git commit automation
- Update PLUGINS.md state machine integration
- Implement `.continue-here.md` update logic at each stage boundary

**NOTE:** Stages 2-5 remain stubs until Phase 3 (subagent implementations)

### Handoff Context

After Phase 1 completion:
- All user entry points exist and are discoverable via autocomplete
- All workflow orchestration logic exists in skills
- Decision menus enable progressive feature discovery
- Handoff system allows pause/resume across sessions
- System is "explorable" but Stages 2-5 require manual implementation until Phase 3

### Testing Strategy for Phase 2

When Phase 2 begins:
1. Test `/implement TestPlugin` through Stages 0-1 (should work fully)
2. Verify Stage 1 blocks if contracts missing
3. Verify complexity score calculation
4. Verify git commits at stage boundaries
5. Test Stage 6 validation workflow
6. Verify PLUGINS.md updates correctly

---

## Meta

**Phase:** 1 (Discovery System)
**Sub-Phases:** 3 (1a: Commands, 1b: Skills, 1c: Systems)
**Duration Estimate:** 4-6 hours
**Dependencies:** Phase 0 complete
**Deliverables:** 12 tasks producing 9 commands + 7 skills + integrated decision/handoff systems
**Architecture References:** 8 files (routing, component, state, communication, philosophy)
**Procedure References:** 22 files (9 commands + 7 skills + 2 core systems + 4 examples)
**Next Phase:** Phase 2 (Workflow Engine) - Stages 0, 1, 6 fully implemented

**Critical Success Factor**: Decision menus and handoff files form the "discovery through play" user experience. All workflows must present options at checkpoints, and all workflows must support pause/resume via handoff files. Without these patterns, the system becomes documentation-heavy rather than exploration-friendly.
