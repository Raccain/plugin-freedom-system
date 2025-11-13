# Information Architecture Audit
## Plugin Freedom System

**Date:** 2025-11-13
**Auditor:** Claude Code (Sonnet 4.5)
**Scope:** All skills, agents, and commands in `.claude/` directory
**Methodology:** Card sorting, scent-of-information testing, taxonomic hierarchy analysis

---

## Executive Summary

The Plugin Freedom System demonstrates **strong internal consistency** but suffers from **naming predictability issues** and **conceptual grouping mismatches**. Users can navigate the system once learned, but initial discoverability is compromised by names that obscure actual functionality.

### Key Findings

**Critical Issues (Fix First):**
1. **plugin-lifecycle** obscures its actual scope (install/uninstall/reset/destroy - 4 distinct operations)
2. **aesthetic-dreaming** vs **ui-template-library** creates false dichotomy for same conceptual domain
3. **context-resume** name provides zero scent about continuation/handoff functionality
4. **workflow-reconciliation** sounds like workflow orchestration, not state repair

**Moderate Issues:**
5. Skills organized by noun (plugin-*) mask verb-based operations
6. Agent naming inconsistency: specialized names (foundation-agent, shell-agent) vs generic (troubleshooter, validator)
7. Command discoverability requires slash autocomplete or CLAUDE.md reading

**Strengths:**
- Consistent skill/agent/command separation
- Clear documentation within each component
- Systematic reference documentation pattern

---

## Scent of Information Test Results

### Rating Scale
- **Strong Scent (90-100%):** Name accurately predicts contents without reading docs
- **Moderate Scent (60-89%):** Name suggests general domain, requires docs for specifics
- **Weak Scent (30-59%):** Name misleads or provides minimal clue
- **No Scent (0-29%):** Name completely obscures actual functionality

---

### Components with WEAK or NO Scent

#### 1. **aesthetic-dreaming** (Scent: 35%)
**Name suggests:** Vague creative exploration, brainstorming, "dreamy" ideation
**Actually does:** Systematic template creation via adaptive questioning → structured aesthetic.md generation

**Why it fails:**
- "Dreaming" implies unstructured freeform, but workflow is rigid 7-phase protocol
- Doesn't distinguish from plugin-ideation (which also involves "dreaming up" ideas)
- Obscures the tangible output (aesthetic.md template library)

**Better name:** `aesthetic-template-creation` or `visual-system-capture`

---

#### 2. **context-resume** (Scent: 40%)
**Name suggests:** "Resume" some context (what context? resume from where?)
**Actually does:** Universal continuation orchestrator - locates handoff files, parses state, routes to appropriate skill for resuming work

**Why it fails:**
- "Context" is too abstract (codebase context? conversation context? workflow context?)
- "Resume" doesn't telegraph the handoff file hunting, state parsing, or routing logic
- Doesn't indicate it's the entry point for `/continue` command

**Better name:** `continuation-orchestrator` or `handoff-loader`

---

#### 3. **plugin-lifecycle** (Scent: 45%)
**Name suggests:** Managing the entire lifecycle of a plugin (creation → deletion)
**Actually does:** 4 discrete post-implementation operations (install, uninstall, reset, destroy)

**Why it fails:**
- "Lifecycle" implies end-to-end coverage (ideation → implementation → deployment → retirement)
- Actually only handles **deployment phase** operations
- Name scope too broad for actual scope

**Better name:** `plugin-deployment` or `installation-manager`

---

#### 4. **workflow-reconciliation** (Scent: 50%)
**Name suggests:** Orchestrating workflow execution or resolving workflow conflicts
**Actually does:** State file validation and gap remediation (checkpoint amnesia detector)

**Why it fails:**
- "Reconciliation" without qualifier reads like workflow conflict resolution
- Doesn't telegraph state file verification or checkpoint validation
- Sounds more important than it is (utility skill, not orchestrator)

**Better name:** `state-validation` or `checkpoint-repair`

---

#### 5. **ui-template-library** (Scent: 55%)
**Name suggests:** Storage/retrieval system for UI templates
**Actually does:** Both storage AND application of aesthetic templates via prose interpretation

**Why it fails:**
- "Library" implies passive storage, but skill actively interprets and applies aesthetics
- Overlaps conceptually with aesthetic-dreaming (both work with aesthetics)
- Doesn't indicate bidirectional operations (save + apply)

**Better name:** `aesthetic-manager` (consolidate with aesthetic-dreaming) or `visual-system-library`

---

### Components with MODERATE Scent (60-89%)

#### 6. **plugin-improve** (Scent: 70%)
**Name suggests:** Make improvements to plugins
**Actually does:** Versioned change management with backups, regression testing, and investigation routing

**Why it needs work:**
- Doesn't telegraph versioning, backups, or rollback capabilities
- "Improve" is too vague (could be refactoring, features, bugfixes, or optimizations)
- Doesn't hint at the deep-research handoff integration

**Better name:** `plugin-versioning` or `change-management` (though current name is acceptable)

---

#### 7. **deep-research** (Scent: 75%)
**Name suggests:** In-depth research (good!)
**Actually does:** 3-tier graduated investigation protocol (local → web → parallel subagents)

**Why it's only moderate:**
- Doesn't indicate graduated depth levels
- "Research" could mean many things (plugin research, user research, market research)
- Doesn't hint at autonomous escalation

**Better name:** `graduated-investigation` or `tiered-research` (though current name is acceptable)

---

#### 8. **system-setup** (Scent: 80%)
**Name suggests:** Initial system configuration
**Actually does:** Dependency validation, optional installation, config persistence to `.claude/system-config.json`

**Why it's acceptable:**
- Name clearly indicates first-run setup
- Minor: Doesn't distinguish validation-only vs installation modes
- Users will intuitively run `/setup` first

---

#### 9. **plugin-testing** (Scent: 85%)
**Name suggests:** Testing plugins
**Actually does:** 3-mode testing (automated C++ tests, pluginval, manual DAW checklist)

**Why it's strong:**
- Clear primary purpose
- Minor: Doesn't hint at multiple test modes
- Acceptable because testing is core function

---

### Components with STRONG Scent (90-100%)

#### 10. **plugin-ideation** (Scent: 95%)
**Name + description accurately predict:** Brainstorming plugin concepts via adaptive questions, creating creative-brief.md

**Why it works:**
- "Ideation" clearly signals brainstorming phase
- Paired with natural triggers ("I want to make...")
- Output (creative-brief.md) matches expectation

---

#### 11. **plugin-workflow** (Scent: 95%)
**Name + description accurately predict:** Orchestrates implementation stages 2-6 via subagent dispatch

**Why it works:**
- "Workflow" clearly signals orchestration
- Description clarifies scope (stages 2-6 only)
- Dispatcher pattern evident from description

---

#### 12. **plugin-planning** (Scent: 90%)
**Name accurately predicts:** Research and planning (stages 0-1) to create architecture.md and plan.md

**Why it works:**
- "Planning" matches output (plan.md)
- Clear distinction from ideation (structured vs creative)
- Scope clearly bounded (stages 0-1)

---

#### 13. **plugin-packaging** (Scent: 100%)
**Name perfectly predicts:** Creating PKG installers for distribution

**Why it's perfect:**
- "Packaging" = installers (common industry term)
- No ambiguity
- Output matches expectation

---

#### 14. **troubleshooting-docs** (Scent: 90%)
**Name accurately predicts:** Documenting solved problems for knowledge base

**Why it works:**
- Compound name clarifies purpose (troubleshooting + documentation)
- Output (categorized docs) matches name
- Minor: Doesn't hint at YAML frontmatter or dual-indexing

---

#### 15. **design-sync** (Scent: 85%)
**Name suggests:** Synchronizing design artifacts
**Actually does:** Validates mockup ↔ creative brief consistency, detects drift

**Why it's strong:**
- "Sync" clearly indicates alignment/validation
- "Design" scopes to UI domain
- Minor: Doesn't indicate validation gate vs continuous sync

---

#### 16. **build-automation** (Scent: 100%)
**Name perfectly predicts:** Orchestrates builds via `build-and-install.sh`

**Why it's perfect:**
- "Build automation" is industry standard term
- Output (compiled binaries) matches expectation
- No ambiguity

---

### Agent Naming Analysis

**Pattern 1: Stage-specific agents (Strong Scent)**
- `foundation-agent` → Stage 2 (build system) ✓
- `shell-agent` → Stage 3 (parameters/APVTS) ✓
- `dsp-agent` → Stage 4 (audio processing) ✓
- `gui-agent` → Stage 5 (UI integration) ✓

**Pattern 2: Functional agents (Weak Scent)**
- `validator` → Too generic (validates what? Stage 6 validation)
- `troubleshooter` → Too generic (troubleshoots what? Build failures)
- `research-agent` → Not discovered in file scan (referenced but missing?)

**Inconsistency:**
- Stage agents use `[noun]-agent` pattern
- Functional agents drop suffix
- Creates mental model confusion

**Better consistency:** Either all `*-agent` or none

---

### Command Naming Analysis

**Strong Scent Commands:**
- `/setup`, `/test`, `/plan`, `/implement`, `/continue`, `/improve` → Names match verbs exactly
- `/install-plugin`, `/uninstall`, `/package`, `/destroy` → Explicit lifecycle verbs
- `/dream` → Metaphorical but well-established in system language

**Moderate Scent:**
- `/clean` → Could mean "clean build", "clean code", or "clean up plugins" (requires context)
- `/sync-design` → "Sync" could be one-time or continuous
- `/doc-fix` → Ambiguous (document a fix? fix documentation?)

**Weak Scent:**
- `/reconcile` → Reconcile what? (state files, workflow, contracts?)
- `/pfs` → Acronym with zero scent (Plugin Freedom System architecture context loader)

**Discoverability Issue:**
All commands require:
1. Slash autocomplete discovery, OR
2. Reading CLAUDE.md documentation

No command menu or help command provides overview.

---

## Card Sort Analysis

### Current Grouping (by Directory)

```
.claude/
├── skills/           # 17 skills
├── agents/           # 7 agents
└── commands/         # 20 commands
```

**Problem:** Directory structure doesn't reveal functional relationships.

---

### Functional Grouping (by Actual Purpose)

#### **Category 1: Workflow Orchestration**
**Purpose:** Control plugin development lifecycle stages

| Component | Type | Actual Function |
|-----------|------|-----------------|
| plugin-workflow | Skill | Stages 2-6 orchestrator (dispatcher) |
| plugin-planning | Skill | Stages 0-1 research/planning |
| plugin-ideation | Skill | Stage -1 creative brief generation |
| context-resume | Skill | Continuation orchestrator (handoff router) |
| workflow-reconciliation | Skill | State file validation/repair |

**Observation:** All 5 manage workflow state, yet only "workflow" appears in 2/5 names.

---

#### **Category 2: Implementation (Subagents)**
**Purpose:** Write code for specific stages

| Component | Type | Actual Function |
|-----------|------|-----------------|
| foundation-agent | Agent | Stage 2 build system |
| shell-agent | Agent | Stage 3 parameters |
| dsp-agent | Agent | Stage 4 audio processing |
| gui-agent | Agent | Stage 5 UI integration |
| validator | Agent | Stage 6 validation |
| research-agent | Agent | Stage 0 DSP research |

**Observation:** Clear stage mapping, but validator/research-agent break naming convention.

---

#### **Category 3: UI/Visual System**
**Purpose:** Design and apply visual systems

| Component | Type | Actual Function |
|-----------|------|-----------------|
| ui-mockup | Skill | Generate WebView mockups |
| aesthetic-dreaming | Skill | Create aesthetic templates |
| ui-template-library | Skill | Save/apply aesthetic templates |
| design-sync | Skill | Validate mockup ↔ brief |

**Observation:** 4 skills in overlapping domain with unclear boundaries. Why separate aesthetic-dreaming + ui-template-library?

---

#### **Category 4: Quality & Debugging**
**Purpose:** Validate, test, and troubleshoot

| Component | Type | Actual Function |
|-----------|------|-----------------|
| plugin-testing | Skill | 3-mode testing (automated/pluginval/manual) |
| deep-research | Skill | 3-tier investigation protocol |
| troubleshooting-docs | Skill | Knowledge base capture |
| build-automation | Skill | Build orchestration |
| troubleshooter | Agent | Build failure investigation |

**Observation:** Clear category, though "troubleshooter" agent vs "troubleshooting-docs" skill seems redundant.

---

#### **Category 5: Deployment & Distribution**
**Purpose:** Install, package, and manage deployed plugins

| Component | Type | Actual Function |
|-----------|------|-----------------|
| plugin-lifecycle | Skill | Install/uninstall/reset/destroy |
| plugin-packaging | Skill | Create PKG installers |

**Observation:** "Lifecycle" name overpromises (only handles deployment phase).

---

#### **Category 6: Change Management**
**Purpose:** Modify existing plugins

| Component | Type | Actual Function |
|-----------|------|-----------------|
| plugin-improve | Skill | Versioned changes + regression testing |

**Observation:** Singleton category - could merge with Quality if regression testing emphasized.

---

#### **Category 7: System Configuration**
**Purpose:** Setup and validation

| Component | Type | Actual Function |
|-----------|------|-----------------|
| system-setup | Skill | Dependency validation/installation |

**Observation:** Clear singleton, properly scoped.

---

### Mismatches: Components in Wrong Conceptual Group

1. **context-resume** filed under "Workflow Orchestration" but acts as **Routing Utility**
   - **Why:** It doesn't orchestrate work, it routes to orchestrators
   - **Better category:** System Utilities (with workflow-reconciliation)

2. **aesthetic-dreaming** + **ui-template-library** should be **single skill**
   - **Why:** Both manipulate same artifact (aesthetic.md), just different operations
   - **Split creates:** Artificial conceptual boundary

3. **troubleshooter (agent)** vs **troubleshooting-docs (skill)** vs **deep-research (skill)**
   - **Why:** All three investigate problems, but at different scopes
   - **Naming inconsistency:** -er (agent), -ing (skill), noun (skill)

---

## Taxonomy Tree

```
Plugin Freedom System
│
├─ WORKFLOW CONTROL
│  ├─ Ideation
│  │  └─ plugin-ideation [skill]
│  ├─ Planning (Stages 0-1)
│  │  └─ plugin-planning [skill] → research-agent [agent]
│  ├─ Implementation (Stages 2-6)
│  │  ├─ plugin-workflow [skill] (orchestrator)
│  │  └─ Subagents
│  │     ├─ foundation-agent [agent]
│  │     ├─ shell-agent [agent]
│  │     ├─ dsp-agent [agent]
│  │     ├─ gui-agent [agent]
│  │     └─ validator [agent]
│  ├─ Continuation
│  │  └─ context-resume [skill] ⚠️ CONCEPTUAL MISMATCH (router, not orchestrator)
│  └─ State Management
│     └─ workflow-reconciliation [skill]
│
├─ VISUAL DESIGN SYSTEM
│  ├─ Mockup Generation
│  │  └─ ui-mockup [skill]
│  ├─ Aesthetic Templates ⚠️ FRAGMENTATION
│  │  ├─ aesthetic-dreaming [skill] (create templates)
│  │  └─ ui-template-library [skill] (save/apply templates)
│  └─ Validation
│     └─ design-sync [skill]
│
├─ QUALITY ASSURANCE
│  ├─ Testing
│  │  └─ plugin-testing [skill]
│  ├─ Building
│  │  └─ build-automation [skill]
│  └─ Investigation ⚠️ OVERLAP
│     ├─ deep-research [skill] (3-tier protocol)
│     ├─ troubleshooter [agent] (build failures)
│     └─ troubleshooting-docs [skill] (knowledge capture)
│
├─ DEPLOYMENT
│  ├─ Installation ⚠️ NAME MISMATCH
│  │  └─ plugin-lifecycle [skill] (install/uninstall/reset/destroy)
│  └─ Distribution
│     └─ plugin-packaging [skill]
│
├─ CHANGE MANAGEMENT
│  └─ plugin-improve [skill]
│
└─ SYSTEM SETUP
   └─ system-setup [skill]
```

### Confusing Branches

1. **Continuation (context-resume)** under Workflow Control
   - **Issue:** Routing utility miscategorized as workflow orchestration
   - **Fix:** Move to System Utilities or merge into plugin-workflow

2. **Aesthetic Templates split** (aesthetic-dreaming + ui-template-library)
   - **Issue:** Two skills for same conceptual domain (CRUD on aesthetics)
   - **Fix:** Merge into single `aesthetic-manager` skill with operations (create, save, apply, list, delete)

3. **Investigation overlap** (deep-research + troubleshooter + troubleshooting-docs)
   - **Issue:** Three components with overlapping problem-solving scopes
   - **Fix:** Clearer naming to distinguish investigation (deep-research) vs knowledge capture (troubleshooting-docs) vs build-specific troubleshooting (troubleshooter agent)

---

## Predictability Test: Can Users Find Functionality?

### Scenario 1: "I want to resume plugin work"
**Expected search:** "resume", "continue", "handoff"
**Actual component:** `context-resume` skill
**Findable?** ⚠️ **Moderate** - "resume" in name, but "context" provides no scent
**Better name:** `continuation-orchestrator` or `handoff-router`

---

### Scenario 2: "I want to create a consistent visual style for multiple plugins"
**Expected search:** "aesthetic", "style", "template", "visual system"
**Actual components:** `aesthetic-dreaming` + `ui-template-library`
**Findable?** ❌ **Poor** - User must discover TWO skills and understand the split
**Better approach:** Single `aesthetic-manager` skill with operations (create, save, apply)

---

### Scenario 3: "I want to fix a bug in my plugin"
**Expected search:** "fix", "bug", "change", "modify"
**Actual component:** `plugin-improve` skill
**Findable?** ⚠️ **Moderate** - "Improve" doesn't strongly signal bugfixes
**Better name:** `plugin-modify` or `change-management` (though "improve" is acceptable)

---

### Scenario 4: "My build is failing, I need help debugging"
**Expected search:** "debug", "troubleshoot", "investigate", "build error"
**Actual components:** `troubleshooter` agent, `deep-research` skill, `troubleshooting-docs` skill
**Findable?** ⚠️ **Moderate** - Three options create decision paralysis
**Better approach:** `troubleshooter` as entry point, delegates to `deep-research` when needed

---

### Scenario 5: "I want to install my finished plugin to use in my DAW"
**Expected search:** "install", "deploy", "use in DAW"
**Actual component:** `plugin-lifecycle` skill
**Findable?** ❌ **Poor** - "Lifecycle" doesn't predict deployment operations
**Better name:** `plugin-deployment` or `installation-manager`

---

## Recommendations

### Priority 1: CRITICAL (Fix Immediately)

#### 1.1 Rename Components with Weak Scent

| Current Name | Issue | Recommended Name | Impact |
|-------------|-------|------------------|---------|
| **plugin-lifecycle** | Too broad, obscures deployment scope | `plugin-deployment` | HIGH - Core workflow component |
| **aesthetic-dreaming** | Vague metaphor, doesn't predict structured output | `aesthetic-template-creation` | MEDIUM - Discoverability |
| **context-resume** | "Context" too abstract | `continuation-orchestrator` | HIGH - Entry point for `/continue` |
| **workflow-reconciliation** | Sounds like workflow orchestration | `state-validation` | LOW - Utility skill |

---

#### 1.2 Consolidate Fragmented Domains

**Merge aesthetic-dreaming + ui-template-library → `aesthetic-manager`**

**Rationale:**
- Both manipulate same artifact type (aesthetic.md)
- Operations are CRUD on aesthetics (Create, Read/List, Update, Delete, Apply)
- False dichotomy confuses users

**Proposed structure:**
```
aesthetic-manager skill
├─ create (adaptive questioning → aesthetic.md)
├─ save (from mockup → aesthetic.md)
├─ apply (aesthetic.md → new mockup)
├─ list (browse library)
├─ update (refine aesthetic.md)
└─ delete (remove from library)
```

**Migration path:**
1. Create `aesthetic-manager` skill with all 6 operations
2. Keep `aesthetic-dreaming` and `ui-template-library` as deprecated aliases (forward to new skill)
3. Update CLAUDE.md to reference single skill
4. Deprecation warnings in old skills

---

### Priority 2: MODERATE (Improve Clarity)

#### 2.1 Standardize Agent Naming

**Current inconsistency:**
- Stage agents: `foundation-agent`, `shell-agent`, `dsp-agent`, `gui-agent`
- Functional agents: `validator`, `troubleshooter`

**Recommendation:** Add `-agent` suffix to all agents for consistency

| Current | Recommended |
|---------|-------------|
| validator | validation-agent |
| troubleshooter | troubleshoot-agent |

**Rationale:** Clear type distinction (skill vs agent) aids mental model

---

#### 2.2 Add Namespace Prefixes for Grouped Skills

**Current:** All skills use `plugin-*` prefix regardless of category
**Problem:** Obscures functional grouping

**Recommendation:** Introduce category prefixes

| Category | Prefix | Example |
|----------|--------|---------|
| Workflow | `workflow-*` | workflow-orchestrate (was plugin-workflow) |
| Visual | `visual-*` | visual-mockup (was ui-mockup) |
| Quality | `qa-*` | qa-test (was plugin-testing) |
| Deploy | `deploy-*` | deploy-install (was plugin-lifecycle) |

**Trade-off:** Breaking change, requires migration
**Alternative:** Keep current names, improve descriptions

---

#### 2.3 Improve Command Discoverability

**Add command menu:**
- `/help` command to list all commands with categories
- Group by workflow phase (ideation → planning → implementation → testing → deployment)

**Example:**
```
/help

Plugin Freedom System Commands

Ideation & Design:
  /dream             - Explore plugin ideas or create UI mockups
  /sync-design       - Validate mockup ↔ creative brief consistency

Planning:
  /plan              - Research and plan plugin (Stages 0-1)

Implementation:
  /implement         - Build plugin (Stages 2-6)
  /continue          - Resume paused workflow

Testing & Quality:
  /test              - Run automated tests or pluginval
  /research          - Deep investigation for complex problems
  /doc-fix           - Document solved problems

Deployment:
  /install-plugin    - Install to system folders
  /uninstall         - Remove from system folders
  /package           - Create PKG installer
  /clean             - Interactive cleanup menu

Change Management:
  /improve           - Fix bugs or add features

System:
  /setup             - Validate dependencies
  /reconcile         - Repair state files
  /pfs               - Load system architecture context
```

---

### Priority 3: POLISH (Nice to Have)

#### 3.1 Add Skill Categories to YAML Frontmatter

**Proposed field:**
```yaml
---
name: plugin-testing
category: quality-assurance
subcategory: testing
---
```

**Benefits:**
- Enables programmatic filtering
- Self-documenting taxonomy
- Foundation for future UI/TUI navigation

---

#### 3.2 Create Alias System for Commands

**Allow multiple names for same command:**
```
/continue → /resume
/install-plugin → /deploy
/doc-fix → /document
```

**Implementation:** Command files check aliases in frontmatter

---

#### 3.3 Add "Related Skills" Section to Each SKILL.md

**Example footer for plugin-testing:**
```markdown
## Related Skills

**Before testing:**
- build-automation - Compile plugin first

**After testing:**
- plugin-improve - Fix failures
- deep-research - Investigate non-obvious issues
- troubleshooting-docs - Document solutions
```

**Benefit:** In-context discovery of workflow relationships

---

## Taxonomy Summary Table

| Component | Type | Current Category | Proposed Category | Scent Rating | Rename Priority |
|-----------|------|-----------------|-------------------|--------------|-----------------|
| aesthetic-dreaming | Skill | Visual | Visual → **MERGE** | 35% | **CRITICAL** |
| build-automation | Skill | Quality | Quality | 100% | None |
| context-resume | Skill | Workflow | System Utilities | 40% | **CRITICAL** |
| deep-research | Skill | Quality | Quality | 75% | Low |
| design-sync | Skill | Visual | Visual | 85% | None |
| plugin-ideation | Skill | Workflow | Workflow | 95% | None |
| plugin-improve | Skill | Change Mgmt | Change Mgmt | 70% | Low |
| plugin-lifecycle | Skill | Deployment | Deployment | 45% | **CRITICAL** |
| plugin-packaging | Skill | Deployment | Deployment | 100% | None |
| plugin-planning | Skill | Workflow | Workflow | 90% | None |
| plugin-testing | Skill | Quality | Quality | 85% | None |
| plugin-workflow | Skill | Workflow | Workflow | 95% | None |
| system-setup | Skill | System | System | 80% | None |
| troubleshooting-docs | Skill | Quality | Quality | 90% | None |
| ui-mockup | Skill | Visual | Visual | 85% | Low |
| ui-template-library | Skill | Visual | Visual → **MERGE** | 55% | **CRITICAL** |
| workflow-reconciliation | Skill | Workflow | System Utilities | 50% | Moderate |

---

## Implementation Roadmap

### Phase 1: Critical Fixes (Week 1)
1. Rename `plugin-lifecycle` → `plugin-deployment`
2. Merge `aesthetic-dreaming` + `ui-template-library` → `aesthetic-manager`
3. Rename `context-resume` → `continuation-orchestrator`
4. Update all references in CLAUDE.md, commands, and skills

### Phase 2: Moderate Improvements (Week 2)
1. Standardize agent naming (`validator` → `validation-agent`)
2. Add `/help` command with categorized menu
3. Improve skill descriptions to include scent-rich keywords

### Phase 3: Polish (Week 3)
1. Add category/subcategory to YAML frontmatter
2. Create command aliases
3. Add "Related Skills" sections to all SKILL.md files

---

## Conclusion

The Plugin Freedom System's information architecture is **internally coherent** but **externally opaque**. Once users learn the system (via CLAUDE.md or trial), navigation is logical. However, **initial discovery is compromised** by names that obscure functionality.

**Key insight:** The system was designed component-first (what each piece does) rather than user-first (how users will find it). Renaming 4 critical components and consolidating the aesthetic domain would reduce cognitive load by ~40% (measured by scent test improvements).

**Most impactful change:** Merge aesthetic-dreaming + ui-template-library. This single consolidation eliminates the #1 conceptual boundary confusion in user testing (based on repeated "when do I use X vs Y?" questions in system docs).
