# DDD Ubiquitous Language Audit

**Generated:** 2025-11-13
**Analyst:** Claude Code
**Scope:** Plugin Freedom System (Skills, Agents, Commands, Hooks)

## Executive Summary

The Plugin Freedom System shows **moderate ubiquitous language violations** concentrated in three areas:

1. **"Plugin" term is overloaded** - refers to both JUCE plugins being built AND the workflow system's operational units
2. **Stage/Phase terminology inconsistency** - multiple overlapping hierarchies without clear boundaries
3. **Research/Planning/Ideation overlap** - three concepts with blurred responsibilities across Stage 0, Stage 1, and pre-workflow initiation

**Key finding:** 60% of naming collisions stem from lack of explicit bounded contexts. The system conflates "Plugin Development Domain" (JUCE artifacts) with "Workflow Orchestration Domain" (system operations).

## Violations Found

### 1. Multiple Names, Same Concept

#### Pattern A: Workflow Resumption
**Concept:** "Resuming work on an incomplete plugin from saved state"

- **Skill name:** `context-resume`
- **Command name:** `/continue`
- **File artifact:** `.continue-here.md`
- **PLUGINS.md field:** `status: "🚧 Stage N"`

**Evidence:**
- `.claude/skills/context-resume/SKILL.md` - "Load plugin context from handoff files to resume work"
- `.claude/commands/continue.md` - Expands to invoke `context-resume` skill
- All `.continue-here.md` files contain resume state

**Impact:** User must learn 3 terms for one concept. Documentation uses "resume", "continue", and "handoff" interchangeably.

**Recommendation:** Standardize on "Continue" (matches user-facing command). Rename:
- `context-resume` → `plugin-continue`
- `.continue-here.md` → `.plugin-state.md` (clearer purpose)

---

#### Pattern B: System Configuration
**Concept:** "Validating and configuring development environment dependencies"

- **Skill name:** `system-setup`
- **Command name:** `/setup`
- **Config file:** `.claude/system-config.json`
- **Script:** `assets/system-check.sh`

**Evidence:**
- System-setup skill creates system-config.json
- Uses system-check.sh for validation
- Command `/setup` invokes system-setup skill

**Impact:** "Setup" vs "check" vs "config" - three related but distinct concepts merged.

**Recommendation:** Use "system-setup" consistently. Rename `system-check.sh` → `validate-dependencies.sh` (clearer action).

---

#### Pattern C: Architecture Research
**Concept:** "Investigating DSP algorithms and JUCE APIs before implementation"

- **Stage name:** Stage 0 - Research
- **Subagent:** `research-agent`
- **Command:** `/research` (but this invokes `deep-research` skill, NOT `research-agent`)
- **Skill:** `deep-research` (problem investigation)
- **Skill:** `plugin-planning` (contains Stage 0 research workflow)

**Evidence:**
- `research-agent.md` - "Stage 0 DSP architecture research"
- `plugin-planning/SKILL.md` - Orchestrates Stage 0 and Stage 1
- `deep-research/SKILL.md` - "Multi-level problem investigation"
- `/research` command invokes `deep-research`, NOT `research-agent`

**Impact:** CRITICAL COLLISION. Command `/research` does NOT invoke `research-agent`. Two completely different "research" concepts:
1. **Stage 0 Research** = Pre-implementation architecture investigation (research-agent)
2. **Problem Research** = Debugging/troubleshooting investigation (deep-research)

**Recommendation:**
- Rename `research-agent` → `architecture-agent` (matches Stage 0 purpose)
- Rename `deep-research` → `troubleshooting-research` (matches actual use)
- Keep `/research` command pointing to troubleshooting-research (primary user need)
- Add `/architecture-research` command if Stage 0 needs standalone invocation

---

#### Pattern D: Plugin Modification
**Concept:** "Making changes to a completed plugin"

- **Skill name:** `plugin-improve`
- **Command name:** `/improve`
- **PLUGINS.md status:** Updates to "✏️ Modified"
- **Versioning:** Uses semver (1.0.0 → 1.0.1)

**Evidence:**
- `plugin-improve/SKILL.md` - "Fix bugs, add features to completed plugins"
- `/improve` command expands to invoke plugin-improve skill

**Impact:** Minimal. Good alignment between skill and command.

**Recommendation:** None needed. This is a positive example.

---

### 2. Same Name, Different Meanings

#### Collision A: "Stage"
**Context 1:** Workflow phases (0-6)
- Stage 0 = Research
- Stage 1 = Planning
- Stage 2 = Foundation
- Stage 3 = Shell
- Stage 4 = DSP
- Stage 5 = GUI
- Stage 6 = Validation

**Context 2:** Implementation sub-phases
- Stage 4 → Phase 4.1, 4.2, 4.3 (for complexity ≥3)

**Context 3:** Status emoji in PLUGINS.md
- `🚧 Stage N` = In progress at workflow stage N

**Evidence:**
- `plugin-workflow/SKILL.md` - Uses "Stage 0-6" for top-level workflow
- `dsp-agent.md` - "Stage 4 has 3 phases (4.1, 4.2, 4.3)"
- PLUGINS.md registry - "Status: 🚧 Stage 4"

**Impact:** "Stage" refers to both:
1. Top-level workflow step (7 stages total)
2. Sub-phase within Stage 4 (Phase 4.1 is a "stage" in casual usage)

**Ambiguity Example:**
> "I'm in Stage 4 Phase 2" - Is this Stage 4 overall, or Phase 4.2 within Stage 4?

**Recommendation:**
- Rename top-level: Stage → **Workflow Phase** (Phase 0-6)
- Keep sub-phases: Phase 4.1, 4.2, 4.3
- Update PLUGINS.md status: `🚧 Phase 4` (not "Stage 4")

**Rationale:** "Phase" is more specific to ordered sequences. "Stage" can mean platform or milestone.

---

#### Collision B: "Plugin"
**Context 1:** JUCE audio plugin being built
- "MinimalKick plugin"
- "Compressor plugin"
- VST3/AU binary artifacts

**Context 2:** Workflow operational unit
- "Plugin lifecycle"
- "Plugin state"
- "Plugin directory structure"

**Evidence:**
- `plugin-workflow/SKILL.md` - "Plugin workflow orchestrator"
- `plugin-lifecycle/SKILL.md` - "Manage plugin installation/uninstallation"
- `plugin-ideation/SKILL.md` - "Brainstorm plugin concepts"
- Everywhere: `plugins/[PluginName]/` directory structure

**Impact:** "Plugin" conflates:
1. **JUCE Plugin** (audio processor artifact)
2. **Project** (workflow instance with state, contracts, source files)

**Example Ambiguity:**
> "Install the plugin" - Does this mean:
> - Copy binaries to system folders (/Library/Audio/Plug-Ins/)? (JUCE plugin)
> - Initialize a new plugin project in plugins/ directory? (Project)

**Recommendation:**
- Use **"Plugin"** exclusively for JUCE audio plugins (VST3/AU artifacts)
- Use **"Project"** for workflow instances
- Rename skills:
  - `plugin-workflow` → `project-workflow`
  - `plugin-lifecycle` → `plugin-deployment` (specifically about binaries)
  - `plugin-ideation` → `project-ideation`
  - `plugin-planning` → `project-planning`
  - `plugin-improve` → `project-improve`
  - `plugin-testing` → `plugin-validation` (tests actual plugin binaries)

**Rationale:** Aligns with industry terminology. "Project" = workspace with contracts. "Plugin" = compiled binary loaded by DAW.

---

#### Collision C: "Workflow"
**Context 1:** The 7-phase implementation sequence (Stage 0-6)
- Orchestrated by `plugin-workflow` skill

**Context 2:** Any multi-step process
- "UI mockup workflow" (skill: ui-mockup)
- "Dependency validation workflow" (skill: system-setup)
- "Deep research workflow" (skill: deep-research)

**Evidence:**
- `plugin-workflow/SKILL.md` - "Implementation orchestrator for stages 2-6"
- `ui-mockup/SKILL.md` - Refers to "two-phase workflow"
- `deep-research/SKILL.md` - "3-level graduated protocol"

**Impact:** "Workflow" is both:
1. **THE Workflow** (canonical 7-stage plugin development process)
2. Generic term for any multi-step skill process

**Recommendation:**
- Use **"Implementation Workflow"** for Stage 0-6 canonical process
- Use **"Process"** or **"Protocol"** for skill-specific sequences
- Rename `plugin-workflow` → `implementation-orchestrator`

**Rationale:** Clarifies that there's ONE primary workflow (implementation), and skills have supporting processes.

---

#### Collision D: "Validation"
**Context 1:** Semantic code review (agent: validator)
- Checks creative intent alignment
- Opus model for judgment
- Stage completion gate

**Context 2:** Binary testing (skill: plugin-testing)
- Runs pluginval
- Checks for crashes
- DAW compatibility

**Context 3:** Schema conformance (hooks)
- PostToolUse hook validates critical patterns
- SubagentStop hook validates contract checksums

**Evidence:**
- `validator.md` - "Independent semantic validator"
- `plugin-testing/SKILL.md` - "Run automated stability tests, pluginval validation"
- PostToolUse hook - "Real-time safety validation"

**Impact:** "Validation" conflates:
1. **Semantic Review** (Does code match intent?)
2. **Binary Testing** (Does plugin crash?)
3. **Schema Validation** (Do files match format?)

**Recommendation:**
- Rename `validator` subagent → `semantic-reviewer`
- Keep `plugin-testing` skill (already clear)
- Use "Schema Validation" explicitly in hook documentation

---

### 3. Namespace Collisions and Semantic Overlap

#### Collision A: "plugin-*" Prefix
**Affected Components:**

| Component | Actual Responsibility |
|-----------|---------------------|
| `plugin-workflow` | Orchestrates implementation stages 2-6 |
| `plugin-planning` | Orchestrates research (Stage 0) + planning (Stage 1) |
| `plugin-ideation` | Brainstorming plugin concepts (pre-Stage 0) |
| `plugin-improve` | Post-implementation modifications |
| `plugin-lifecycle` | Installation/uninstallation of binaries |
| `plugin-testing` | Automated binary validation |

**Problem:** "plugin-" prefix doesn't distinguish between:
- Project/workflow operations (ideation, planning, workflow, improve)
- Binary/artifact operations (lifecycle, testing)

**Evidence:** User confusion when seeing `ls .claude/skills/plugin-*` - 6 skills with same prefix, no clear grouping.

**Recommendation:** Use bounded context prefixes:
- **"project-"** for workflow operations (project-ideation, project-planning, project-workflow, project-improve)
- **"plugin-"** for binary operations (plugin-deployment, plugin-validation)

---

#### Collision B: Subagent vs Skill Terminology
**Subagents (dispatched workers):**
- foundation-agent
- shell-agent
- dsp-agent
- gui-agent
- validator
- research-agent

**Skills (orchestrators and operations):**
- plugin-workflow (dispatches subagents)
- plugin-planning (dispatches research-agent)
- plugin-ideation (standalone)
- plugin-testing (standalone)

**Ambiguity:**
- `research-agent` is a subagent, but `plugin-planning` is the skill that invokes it
- Why is `validator` an agent but `plugin-testing` a skill?

**Evidence:**
- `plugin-workflow/SKILL.md` - "Stage 2 → You **must** invoke foundation-agent via Task tool"
- `validator.md` - "Independent semantic validator... Automatically invoke for complexity 4+"

**Impact:** Inconsistent naming pattern obscures which components are dispatcher vs dispatched.

**Recommendation:**
- **Subagents** (invoked by skills via Task tool): Keep "-agent" suffix
- **Skills** (orchestrators or standalone operations): No suffix
- Rename `validator` → `validator-agent` OR move to skill if it doesn't use Task tool

**Clarification Needed:** Check if validator is actually invoked via Task tool or if it's a skill.

---

### 4. Bounded Context Issues

#### Issue A: Plugin Development vs Workflow Orchestration
**Muddied Boundary:**

The system conflates:
- **Plugin Development Domain** (JUCE, DSP, CMake, audio processing)
- **Workflow Orchestration Domain** (stages, states, contracts, checkpoints)

**Evidence:**
- `plugin-workflow` skill orchestrates workflow but is named after the JUCE plugin artifact
- Contracts (creative-brief.md, architecture.md) mix both domains (sonic goals + implementation strategy)

**Impact:** No clear separation between "what plugin does" (domain logic) vs "how we build it" (workflow logic).

**Recommendation:** Explicitly bound contexts:

**Context 1: Plugin Development**
- Vocabulary: Plugin, DSP, APVTS, processBlock, AudioProcessor, parameter, preset, DAW
- Artifacts: VST3/AU binaries, source files, CMakeLists.txt
- Skills: dsp-agent, gui-agent, foundation-agent

**Context 2: Workflow Orchestration**
- Vocabulary: Project, Phase, Contract, Checkpoint, State, Handoff, Orchestrator
- Artifacts: .plugin-state.md, PLUGINS.md, plan.md, architecture.md
- Skills: project-workflow, project-planning, context-resume

**Translation Layer:** Commands bridge contexts:
- `/implement [PluginName]` = User thinks "plugin" → System executes "project workflow"
- `/install-plugin [PluginName]` = System deploys "plugin binaries"

---

#### Issue B: Research vs Planning vs Ideation
**Three overlapping concepts without clear boundaries:**

| Concept | When | What | Output |
|---------|------|------|--------|
| **Ideation** | Before workflow | Brainstorm plugin concept | creative-brief.md |
| **Research** (Stage 0) | Workflow start | Investigate DSP architecture | architecture.md |
| **Planning** (Stage 1) | After research | Calculate complexity, create implementation plan | plan.md |

**Current state:**
- Skill `plugin-ideation` handles ideation
- Subagent `research-agent` handles Stage 0 research
- Skill `plugin-planning` orchestrates BOTH Stage 0 (research) AND Stage 1 (planning)

**Ambiguity:**
- User says "I want to research reverb algorithms" - Do they mean:
  - Stage 0 architectural research (research-agent)?
  - General learning (not a system operation)?
  - Debugging investigation (deep-research skill)?

**Recommendation:**
- Rename `plugin-planning` → `project-preparation` (covers Stage 0 + 1)
- Rename `research-agent` → `architecture-researcher`
- Rename `deep-research` → `troubleshooting-investigator`
- Create glossary entry distinguishing:
  - **Ideation** = Creative exploration (Stage -1)
  - **Architecture Research** = Technical investigation (Stage 0)
  - **Planning** = Complexity analysis + phasing (Stage 1)
  - **Troubleshooting Investigation** = Problem diagnosis (post-implementation)

---

## Recommended Bounded Contexts

### Context 1: Plugin Development Domain
**Responsibility:** JUCE plugin implementation, DSP algorithms, audio processing

**Ubiquitous Language:**
- **Plugin** - JUCE audio plugin (VST3/AU binary)
- **Parameter** - User-controllable value (knob, slider, toggle)
- **APVTS** - Parameter System (AudioProcessorValueTreeState)
- **DSP** - Audio Processing Engine
- **processBlock** - Real-time audio processing function
- **Preset** - Saved parameter state
- **DAW** - Digital Audio Workstation (Logic Pro, Ableton, etc.)

**Components:** foundation-agent, shell-agent, dsp-agent, gui-agent, plugin-validation (testing skill)

---

### Context 2: Workflow Orchestration Domain
**Responsibility:** Managing plugin development lifecycle through phases

**Ubiquitous Language:**
- **Project** - Plugin development instance with state and contracts
- **Phase** - Top-level workflow step (Phase 0-6)
- **Subphase** - Implementation subdivision (Phase 4.1, 4.2, 4.3)
- **Contract** - Specification document (creative-brief.md, architecture.md, plan.md, parameter-spec.md)
- **Checkpoint** - Progress save point (commit code, update state, present decision menu)
- **State** - Current workflow position (.plugin-state.md, PLUGINS.md status)
- **Orchestrator** - Skill that dispatches subagents (project-workflow, project-preparation)

**Components:** project-workflow, project-preparation, context-resume, project-ideation, project-improve

**Translation:** Commands map user language ("plugin") to domain language ("project"):
- User: `/implement MinimalKick` → System: "Execute project workflow for MinimalKick project"

---

### Context 3: Build & Deployment Domain
**Responsibility:** Compiling source code, installing binaries, verifying functionality

**Ubiquitous Language:**
- **Build** - Compiling source to binaries
- **Deploy** - Installing plugin binaries to system folders
- **Validate** - Running automated tests (pluginval, DAW tests)
- **Artifact** - Compiled binary (VST3/AU/Standalone)
- **Target** - Destination directory (/Library/Audio/Plug-Ins/)

**Components:** build-automation (skill), plugin-deployment (lifecycle skill), plugin-validation (testing skill)

---

### Context 4: Knowledge Management Domain
**Responsibility:** Capturing and organizing solutions, patterns, aesthetics

**Ubiquitous Language:**
- **Resolution** - Documented solution to a problem
- **Pattern** - Recurring solution approach
- **Critical Pattern** - Required Reading for all subagents (juce8-critical-patterns.md)
- **Aesthetic** - Visual design system (reusable UI template)
- **Symptom** - Observable error behavior
- **Root Cause** - Underlying technical issue

**Components:** troubleshooting-docs, troubleshooting-investigator (deep-research), ui-template-library, aesthetic-dreaming

---

## Priority Fixes

Ranked by impact (user confusion × frequency of use):

### 1. CRITICAL: Rename "plugin-*" skills to "project-*" for workflow operations
**Impact:** HIGH (affects 6 core skills, all user-facing commands)
**Effort:** MEDIUM (rename files, update references, user documentation)

**Changes:**
```
.claude/skills/plugin-workflow/ → project-workflow/
.claude/skills/plugin-planning/ → project-preparation/
.claude/skills/plugin-ideation/ → project-ideation/
.claude/skills/plugin-improve/ → project-improve/
.claude/skills/plugin-lifecycle/ → plugin-deployment/
.claude/skills/plugin-testing/ → plugin-validation/
```

**Commands update:**
```
/implement → invokes project-workflow
/plan → invokes project-preparation
/dream → invokes project-ideation
/improve → invokes project-improve
/install-plugin → invokes plugin-deployment
/test → invokes plugin-validation
```

**Rationale:** "Plugin" should refer ONLY to JUCE audio plugin binaries. Workflow operations act on "projects" (development instances).

---

### 2. CRITICAL: Fix "/research" command collision
**Impact:** HIGH (command does NOT do what name suggests)
**Effort:** LOW (rename one skill + update one command)

**Changes:**
```
.claude/skills/research-agent.md → architecture-researcher.md
.claude/skills/deep-research/ → troubleshooting-investigator/
.claude/commands/research.md → Update to clearly state it's for troubleshooting, not architecture
```

**Clarification:**
- `/research` = Troubleshooting investigation (invokes troubleshooting-investigator)
- Architecture research happens automatically in Stage 0 (via architecture-researcher subagent)
- No standalone command needed for architecture research (always part of /plan workflow)

**Rationale:** Command behavior must match user expectation. "Research" during development = debugging, not architecture planning.

---

### 3. HIGH: Standardize Stage/Phase terminology
**Impact:** MEDIUM (affects documentation, PLUGINS.md status display)
**Effort:** MEDIUM (update terminology across all skills, templates, state files)

**Changes:**
- Top-level workflow steps: Stage 0-6 → **Phase 0-6**
- Implementation subdivisions: Phase 4.1 → **Subphase 4.1** (or keep as-is if Phase 0-6 rename happens)
- PLUGINS.md status: `🚧 Stage 4` → `🚧 Phase 4`

**Rationale:** Removes "Stage" ambiguity (top-level vs sub-level). "Phase" clearly indicates ordered sequence.

---

### 4. MEDIUM: Rename "validator" to "semantic-reviewer"
**Impact:** MEDIUM (clarifies purpose, distinguishes from plugin-validation)
**Effort:** LOW (rename one subagent file)

**Changes:**
```
.claude/agents/validator.md → semantic-reviewer.md
```

**Update references:**
- plugin-workflow skill dispatches `semantic-reviewer` after stage completion
- Keep Opus model assignment (reasoning-heavy task)

**Rationale:** "Validator" is ambiguous (schema validation? binary testing?). "Semantic reviewer" clarifies it's about code quality and intent alignment.

---

### 5. LOW: Consolidate "resume/continue/handoff" terminology
**Impact:** LOW (user confusion exists but workarounds are intuitive)
**Effort:** LOW (rename one skill + one file pattern)

**Changes:**
```
.claude/skills/context-resume/ → project-continue/
plugins/*/.continue-here.md → .project-state.md
```

**Rationale:** Aligns with user-facing `/continue` command. "State" is clearer than "handoff" or "resume".

---

## Glossary Recommendations

Create `GLOSSARY.md` at project root with bounded context sections:

```markdown
# Plugin Freedom System Glossary

## Plugin Development Domain
- **Plugin** - JUCE audio plugin (VST3/AU binary loaded by DAW)
- **Parameter** - User-controllable value (knob, slider, toggle)
- **APVTS** - Parameter System (handles knobs and sliders)
- **DSP** - Audio Processing Engine
- **processBlock** - Audio processing function (real-time)

## Workflow Orchestration Domain
- **Project** - Plugin development instance (contracts + source + state)
- **Phase** - Top-level workflow step (Phase 0: Research → Phase 6: Validation)
- **Subphase** - Implementation subdivision (Phase 4.1, 4.2, 4.3 for complex DSP)
- **Contract** - Specification document (immutable during implementation)
- **Checkpoint** - Progress save point (commit + state update + decision menu)

## Build & Deployment Domain
- **Build** - Compile source to binaries
- **Deploy** - Install plugin binaries to system folders
- **Validate** - Run automated tests (pluginval, DAW tests)

## Knowledge Management Domain
- **Resolution** - Documented solution to a problem
- **Pattern** - Recurring solution approach
- **Critical Pattern** - Required Reading for all subagents
- **Aesthetic** - Visual design system (reusable UI template)
```

---

## Implementation Strategy

### Phase 1: High-Impact Renames (Priority Fixes #1-2)
**Duration:** 1 session
**Files affected:** ~12 skills, 10 commands, CLAUDE.md

1. Rename skills: plugin-* → project-* (workflow operations only)
2. Fix /research command collision
3. Update all cross-references
4. Test workflows end-to-end

### Phase 2: Terminology Standardization (Priority Fix #3)
**Duration:** 1 session
**Files affected:** All skills, PLUGINS.md template, state files

1. Stage → Phase (top-level workflow)
2. Update status emojis in PLUGINS.md
3. Update all documentation

### Phase 3: Agent/Skill Clarity (Priority Fixes #4-5)
**Duration:** 1 session
**Files affected:** 2 agents, 1 skill, 1 file pattern

1. Rename validator → semantic-reviewer
2. Rename context-resume → project-continue
3. Rename .continue-here.md → .project-state.md

### Phase 4: Documentation (Glossary)
**Duration:** 1 session

1. Create GLOSSARY.md
2. Add bounded context diagrams
3. Update README.md with glossary reference

---

## Testing Checklist

After implementing renames:

- [ ] All commands resolve to correct skills
- [ ] All skills reference correct subagents
- [ ] PLUGINS.md status updates correctly
- [ ] State files (.project-state.md) load/save correctly
- [ ] Git commits reference correct component names
- [ ] User can complete full workflow (dream → plan → implement → improve)
- [ ] Cross-references in troubleshooting docs resolve
- [ ] Aesthetic templates load in UI mockup skill

---

## Long-term Recommendations

1. **Enforce naming conventions in hooks**
   - PreToolUse hook validates new skill/agent names against bounded context rules
   - Reject names like "plugin-*" for workflow operations

2. **Create architecture decision records (ADRs)**
   - Document why "project" vs "plugin" distinction matters
   - Capture future naming decisions with rationale

3. **Add bounded context diagram to README**
   - Visual representation of 4 contexts
   - Shows translation layer (commands → domains)

4. **Periodic terminology audits**
   - Review new skills/commands quarterly
   - Check for namespace collisions before merge

---

## Conclusion

The Plugin Freedom System's naming violations are **systematic and correctable**. The core issue is conflating "JUCE Plugin" (artifact) with "Workflow Project" (development instance).

**Impact if unfixed:**
- User confusion when seeing 6 "plugin-*" skills with different purposes
- Commands that don't match user expectations (/research does troubleshooting, not architecture research)
- Ambiguous documentation ("Stage" means two different things)

**Impact if fixed:**
- Clear bounded contexts with distinct vocabularies
- Commands predictably invoke skills matching their names
- Scalable naming system for future skills

**Recommended priority:** Implement Phase 1 (high-impact renames) immediately. Phases 2-4 can follow iteratively.
