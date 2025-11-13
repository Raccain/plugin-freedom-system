# Taxonomy Tree Audit

## Executive Summary

The Plugin Freedom System shows **inconsistent hierarchical organization** where naming conventions suggest relationships that don't align with actual system architecture. The most critical issue: **the `plugin-*` family implies a flat feature list when it actually contains a hierarchical workflow** (ideation → planning → workflow → improve → testing → lifecycle → packaging).

**Key findings:**

1. **Broken workflow hierarchy**: `plugin-*` components suggest siblings, but they form a sequential process
2. **Agent naming inconsistency**: Mixed patterns (`*-agent` vs bare names like `validator`, `troubleshooter`)
3. **Command-skill-agent misalignment**: Commands don't consistently map to same-named skills
4. **Orphaned components**: `context-resume`, `workflow-reconciliation`, `aesthetic-dreaming` don't fit any tree
5. **UI family fragmentation**: `ui-mockup` and `ui-template-library` are separated from `aesthetic-dreaming`

**Impact**: Users must mentally reconstruct the actual workflow from flat naming. The taxonomy obscures rather than reveals system structure.

---

## Current Implied Taxonomy

### Family 1: `plugin-*` (7 skills)

```
plugin-*
├─ plugin-workflow          # Orchestrator (Stages 2-6)
├─ plugin-planning          # Planning (Stages 0-1)
├─ plugin-ideation          # Concept generation (pre-Stage 0)
├─ plugin-improve           # Post-completion modifications
├─ plugin-testing           # Validation suite
├─ plugin-lifecycle         # Install/uninstall management
└─ plugin-packaging         # PKG installer creation
```

**Implied relationship**: All siblings at same level
**Actual relationship**: Sequential workflow with branches

```
ACTUAL FLOW:
plugin-ideation
    ↓
plugin-planning (Stages 0-1)
    ↓
plugin-workflow (Stages 2-6) ───→ plugin-testing
    ↓
plugin-lifecycle ───→ plugin-packaging
    ↓
plugin-improve ───→ plugin-testing
```

**Problem**: Flat naming hides temporal/dependency structure

---

### Family 2: `*-agent` (5 of 7 agents)

```
*-agent
├─ foundation-agent         # Stage 2
├─ shell-agent              # Stage 3
├─ dsp-agent                # Stage 4
├─ gui-agent                # Stage 5
└─ research-agent           # Deep research (not in workflow)

ORPHANED AGENTS (missing suffix):
├─ validator                # Stage 6 (should be: validation-agent)
└─ troubleshooter           # Error investigation (should be: troubleshooting-agent)
```

**Implied relationship**: All agents use `-agent` suffix
**Actual relationship**: 5/7 follow convention, 2 don't

**Problem**: Inconsistent naming breaks pattern recognition

---

### Family 3: `ui-*` (2 skills, 1 orphan)

```
ui-*
├─ ui-mockup                # WebView mockup generation
└─ ui-template-library      # Save/apply aesthetic templates

ORPHANED (should be ui-*):
└─ aesthetic-dreaming       # Template exploration (pre-mockup)
```

**Implied relationship**: UI components share `ui-*` prefix
**Actual relationship**:

```
ACTUAL FLOW:
aesthetic-dreaming (optional exploration)
    ↓
ui-template-library (save concepts)
    ↓
ui-mockup (generate from template or creative brief)
```

**Problem**: Workflow split across `ui-*` and standalone name

---

### Family 4: Research/Investigation (3 components, inconsistent naming)

```
*research / troubleshoot*
├─ deep-research            # Multi-tier investigation skill
├─ troubleshooting-docs     # Knowledge base capture skill
├─ research-agent           # Deep research subagent
└─ troubleshooter           # Error investigation subagent
```

**Implied relationship**: Research and troubleshooting are separate concerns
**Actual relationship**:

```
ACTUAL INTEGRATION:
deep-research (skill) ───→ research-agent (subagent)
    ↓
troubleshooting-docs (captures solutions)

troubleshooter (subagent) ───→ troubleshooting-docs
```

**Problem**:
- `deep-research` vs `research-agent` suggests separation, but agent serves skill
- `troubleshooting-docs` vs `troubleshooter` inconsistent noun forms

---

### Family 5: Lifecycle Commands (6 commands)

```
Lifecycle actions:
├─ /dream                   → plugin-ideation
├─ /plan                    → plugin-planning
├─ /implement               → plugin-workflow
├─ /improve                 → plugin-improve
├─ /continue                → context-resume
└─ /test                    → plugin-testing

Deployment actions:
├─ /install-plugin          → plugin-lifecycle
├─ /uninstall               → plugin-lifecycle
├─ /clean                   → plugin-lifecycle
├─ /destroy                 → plugin-lifecycle
└─ /reset-to-ideation       → plugin-lifecycle
```

**Implied relationship**: Commands map 1:1 to skills
**Actual relationship**: Mostly 1:1, but some split (lifecycle commands → 1 skill)

**Problem**: Inconsistent granularity (5 lifecycle commands → 1 skill, but 1 ideation command → 1 skill)

---

### Family 6: Quality/Validation (4 components)

```
Quality assurance:
├─ /test                    → plugin-testing skill
├─ /sync-design             → design-sync skill
├─ /research                → deep-research skill
├─ /doc-fix                 → troubleshooting-docs skill
└─ /add-critical-pattern    → troubleshooting-docs skill
```

**Implied relationship**: Standalone quality tools
**Actual relationship**: Integrated into workflow gates

```
ACTUAL INTEGRATION:
design-sync → Mandatory gate before Stage 2
plugin-testing → Optional after Stages 4, 5, 6
deep-research → On-demand investigation
troubleshooting-docs → Knowledge capture
```

---

### Orphaned Components (No Clear Family)

```
Orphans (no prefix/family):
├─ context-resume           # Workflow resumption
├─ workflow-reconciliation  # State file reconciliation
├─ build-automation         # Build pipeline coordination
├─ system-setup             # Dependency validation
└─ aesthetic-dreaming       # Template exploration (UI-related but not ui-*)
```

**Problem**: Critical system components lack taxonomic organization

**What they actually are:**

- **context-resume**: Workflow orchestration utility (should be `workflow-*` or `plugin-resume`)
- **workflow-reconciliation**: State management utility (should be `workflow-*`)
- **build-automation**: Infrastructure skill (could be `system-*` or `build-*`)
- **system-setup**: Infrastructure skill (already has appropriate prefix)
- **aesthetic-dreaming**: UI workflow component (should be `ui-*` or `ui-dreaming`)

---

## Broken Hierarchies

### 1. Plugin Workflow Appears Flat (Critical)

**Current naming suggests:**
```
plugin-ideation, plugin-planning, plugin-workflow, plugin-improve, plugin-testing, plugin-lifecycle, plugin-packaging
(7 siblings)
```

**Actual structure:**
```
plugin-ideation
    ↓
plugin-planning
    ↓
plugin-workflow ─┬─→ plugin-testing
                 └─→ plugin-improve ─→ plugin-testing
    ↓
plugin-lifecycle ─→ plugin-packaging
```

**Why broken**: Prefix `plugin-*` implies feature list, not process flow. Users can't infer execution order from names.

---

### 2. Agent Naming Inconsistency (High)

**Current:**
```
foundation-agent, shell-agent, dsp-agent, gui-agent, research-agent
validator, troubleshooter
```

**Expected pattern:**
```
foundation-agent, shell-agent, dsp-agent, gui-agent, research-agent
validation-agent, troubleshooting-agent
```

**Why broken**: 5 agents follow `-agent` suffix, 2 don't. Pattern recognition fails.

---

### 3. UI Family Split (Medium)

**Current:**
```
ui-mockup, ui-template-library
aesthetic-dreaming (orphan)
```

**Actual flow:**
```
aesthetic-dreaming → ui-template-library → ui-mockup
```

**Why broken**: Workflow spans both `ui-*` family and orphan component. First step in UI process doesn't signal it belongs to UI family.

---

### 4. Research/Troubleshooting Fragmentation (Medium)

**Current:**
```
deep-research (skill)
research-agent (subagent)
troubleshooting-docs (skill)
troubleshooter (subagent)
```

**Implied separation**: "Research" and "troubleshooting" are different concerns

**Actual integration**: Both feed into same knowledge base, both investigate problems

**Why broken**: Naming suggests separation when components are tightly coupled

---

## Confusing Relationships

### 1. Command-to-Skill Mapping Inconsistency

| Command | Implied Skill | Actual Skill | Match? |
|---------|---------------|--------------|--------|
| `/dream` | `plugin-dream` | `plugin-ideation` | No |
| `/plan` | `plugin-plan` | `plugin-planning` | Close |
| `/implement` | `plugin-implement` | `plugin-workflow` | No |
| `/improve` | `plugin-improve` | `plugin-improve` | Yes |
| `/continue` | `plugin-continue` | `context-resume` | No |
| `/test` | `plugin-test` | `plugin-testing` | Close |
| `/research` | `deep-research` | `deep-research` | Yes |
| `/doc-fix` | `troubleshooting-doc` | `troubleshooting-docs` | Close |

**Confusion**:
- `/implement` doesn't map to `plugin-implementation` (it's `plugin-workflow`)
- `/continue` doesn't map to `plugin-continue` (it's `context-resume`)
- `/dream` doesn't map to `plugin-dream` (it's `plugin-ideation`)

Users can't predict skill names from command names.

---

### 2. Subagent-to-Stage Mapping Hidden

**Current agent names:**
```
foundation-agent, shell-agent, dsp-agent, gui-agent, validator
```

**Stage they implement:**
```
foundation-agent → Stage 2
shell-agent → Stage 3
dsp-agent → Stage 4
gui-agent → Stage 5
validator → Stage 6
```

**Confusion**: Nothing in names indicates stage mapping. Users must consult documentation to know which agent runs when.

**Alternative naming** (stage-explicit):
```
stage-2-foundation-agent
stage-3-shell-agent
stage-4-dsp-agent
stage-5-gui-agent
stage-6-validation-agent
```

This would make workflow order immediately clear.

---

### 3. "Workflow" Name Collision

**Components containing "workflow":**
- `plugin-workflow` (orchestrates Stages 2-6)
- `workflow-reconciliation` (fixes state file drift)

**Confusion**: Both have "workflow" but serve completely different purposes:
- One IS the workflow (orchestrator)
- One FIXES the workflow (state repair)

Better distinction:
- `plugin-workflow` → `plugin-orchestrator` or `plugin-implementation`
- `workflow-reconciliation` → `state-reconciliation` or `checkpoint-repair`

---

## Ideal Taxonomy

### Proposed Structure

Organize by **lifecycle phase** rather than flat prefixes:

```
1. IDEATION PHASE
   ├─ ideation                      # Generate concepts
   ├─ ui-dreaming                   # Explore aesthetics (renamed from aesthetic-dreaming)
   └─ ui-template-library           # Save/apply templates

2. PLANNING PHASE
   ├─ planning                      # Research & planning (Stages 0-1)
   ├─ ui-mockup                     # Visual design
   └─ design-validation             # Brief ↔ mockup consistency (renamed from design-sync)

3. IMPLEMENTATION PHASE
   ├─ implementation-orchestrator   # Stages 2-6 coordinator (renamed from plugin-workflow)
   ├─ stage-2-foundation-agent      # Build system
   ├─ stage-3-shell-agent           # APVTS parameters
   ├─ stage-4-dsp-agent             # Audio processing
   ├─ stage-5-gui-agent             # UI integration
   └─ stage-6-validation-agent      # Testing (renamed from validator)

4. VALIDATION PHASE
   ├─ testing                       # Automated validation suite
   └─ build-pipeline                # Compilation coordination (renamed from build-automation)

5. DEPLOYMENT PHASE
   ├─ lifecycle                     # Install/uninstall
   └─ packaging                     # PKG creation

6. MAINTENANCE PHASE
   ├─ improvement                   # Bug fixes & features
   ├─ state-reconciliation          # Fix state drift (renamed from workflow-reconciliation)
   └─ checkpoint-resume             # Resume from handoff (renamed from context-resume)

7. SUPPORT PHASE
   ├─ investigation                 # Multi-tier research (renamed from deep-research)
   ├─ investigation-agent           # Research subagent (renamed from research-agent)
   ├─ troubleshooting-agent         # Error analysis (renamed from troubleshooter)
   └─ knowledge-capture             # Document solutions (renamed from troubleshooting-docs)

8. SYSTEM PHASE
   └─ system-setup                  # Dependency validation
```

---

### Proposed Naming Conventions

**1. Lifecycle Phase Prefix**

Instead of `plugin-*` for everything, use phase-based prefixes:

- `ideation-*` for concept generation
- `planning-*` for research and planning
- `implementation-*` for code generation
- `validation-*` for testing
- `deployment-*` for installation
- `maintenance-*` for ongoing changes
- `support-*` for investigation
- `system-*` for infrastructure

**2. Component Type Suffix**

- `*-orchestrator` for workflow coordinators
- `*-agent` for all subagents (no exceptions)
- `*-pipeline` for build/deployment automation
- No suffix for leaf skills (e.g., `testing`, `packaging`)

**3. Stage-Explicit Agent Names**

All implementation agents include stage number:

- `stage-2-foundation-agent`
- `stage-3-shell-agent`
- `stage-4-dsp-agent`
- `stage-5-gui-agent`
- `stage-6-validation-agent`

**4. Command-Skill Name Alignment**

Commands should match skill names closely:

| Command | Skill (Current) | Skill (Proposed) | Aligned? |
|---------|-----------------|------------------|----------|
| `/ideate` | `plugin-ideation` | `ideation` | Yes |
| `/plan` | `plugin-planning` | `planning` | Yes |
| `/implement` | `plugin-workflow` | `implementation-orchestrator` | Close |
| `/improve` | `plugin-improve` | `improvement` | Close |
| `/continue` | `context-resume` | `checkpoint-resume` | Close |
| `/test` | `plugin-testing` | `testing` | Yes |
| `/investigate` | `deep-research` | `investigation` | Yes |
| `/install` | `plugin-lifecycle` | `deployment-lifecycle` | Close |
| `/package` | `plugin-packaging` | `deployment-packaging` | Close |

---

## Recommendations

### Priority 1: Fix Agent Naming Consistency (Immediate Impact)

**Change:**
```
validator → stage-6-validation-agent
troubleshooter → troubleshooting-agent
```

**Rationale**: Establishes consistent `-agent` suffix pattern. Stage prefix makes workflow order visible.

**Impact**: Low (2 renames), High clarity gain

---

### Priority 2: Align Command Names with Skills (High Confusion)

**Changes:**

| Current Command | Current Skill | Proposed Command | Proposed Skill |
|----------------|---------------|------------------|----------------|
| `/dream` | `plugin-ideation` | `/ideate` | `ideation` |
| `/implement` | `plugin-workflow` | `/implement` | `implementation-orchestrator` |
| `/continue` | `context-resume` | `/resume` | `checkpoint-resume` |

**Rationale**: Predictable mapping reduces cognitive load

**Impact**: Medium (3 command renames + 3 skill renames), High usability gain

---

### Priority 3: Reorganize UI Family (Medium Impact)

**Change:**
```
aesthetic-dreaming → ui-dreaming
```

**Rationale**: All UI workflow components share `ui-*` prefix

**Flow becomes:**
```
ui-dreaming → ui-template-library → ui-mockup
```

**Impact**: Low (1 rename), Medium clarity gain

---

### Priority 4: Namespace Plugin Lifecycle (Consistency)

**Current:**
```
plugin-workflow, plugin-planning, plugin-ideation, plugin-improve, plugin-testing, plugin-lifecycle, plugin-packaging
```

**Proposed Option A** (Remove `plugin-` prefix):
```
ideation, planning, implementation-orchestrator, improvement, testing, lifecycle, packaging
```

**Proposed Option B** (Keep but make hierarchical):
```
plugin-ideation
plugin-planning
plugin-implementation-orchestrator
plugin-improvement
plugin-validation-testing
plugin-deployment-lifecycle
plugin-deployment-packaging
```

**Rationale**:
- Option A: Simpler, phase-based organization
- Option B: Maintains `plugin-*` namespace but adds structure

**Recommendation**: Option A (simpler)

**Impact**: High (7 renames), but fundamental improvement to system clarity

---

### Priority 5: Stage-Explicit Agent Names (Documentation Value)

**Change:**
```
foundation-agent → stage-2-foundation-agent
shell-agent → stage-3-shell-agent
dsp-agent → stage-4-dsp-agent
gui-agent → stage-5-gui-agent
validator → stage-6-validation-agent
```

**Rationale**: Workflow order visible in component names. New users understand sequence immediately.

**Impact**: Medium (5 renames), High documentation clarity

---

### Priority 6: Resolve "Workflow" Name Collision (Low Impact)

**Change:**
```
plugin-workflow → implementation-orchestrator
workflow-reconciliation → state-reconciliation
```

**Rationale**: Eliminates ambiguity between workflow orchestrator and workflow repair tool

**Impact**: Low (2 renames), Medium clarity gain

---

## Migration Path

### Phase 1: Low-Hanging Fruit (1-2 hours)

1. Rename agents for consistency:
   - `validator` → `stage-6-validation-agent`
   - `troubleshooter` → `troubleshooting-agent`

2. Rename orphaned UI component:
   - `aesthetic-dreaming` → `ui-dreaming`

3. Update references in:
   - CLAUDE.md
   - Command files
   - Skill invocations

**Impact**: 3 renames, establishes pattern consistency

---

### Phase 2: Command-Skill Alignment (2-3 hours)

1. Rename skills to match commands:
   - `plugin-ideation` → `ideation`
   - `plugin-workflow` → `implementation-orchestrator`
   - `context-resume` → `checkpoint-resume`
   - `workflow-reconciliation` → `state-reconciliation`

2. Update command mappings:
   - `/dream` → `/ideate`
   - `/continue` → `/resume`

3. Update all references:
   - CLAUDE.md
   - Skill descriptions
   - Integration points

**Impact**: 4 skill renames + 2 command renames, major usability improvement

---

### Phase 3: Full Taxonomy Reorganization (4-6 hours)

1. Remove `plugin-*` prefix from all skills:
   - `plugin-planning` → `planning`
   - `plugin-improve` → `improvement`
   - `plugin-testing` → `testing`
   - `plugin-lifecycle` → `lifecycle`
   - `plugin-packaging` → `packaging`

2. Add stage prefixes to agents:
   - `foundation-agent` → `stage-2-foundation-agent`
   - `shell-agent` → `stage-3-shell-agent`
   - `dsp-agent` → `stage-4-dsp-agent`
   - `gui-agent` → `stage-5-gui-agent`

3. Rename support components:
   - `deep-research` → `investigation`
   - `research-agent` → `investigation-agent`
   - `troubleshooting-docs` → `knowledge-capture`
   - `build-automation` → `build-pipeline`
   - `design-sync` → `design-validation`

4. Update all references throughout system:
   - CLAUDE.md
   - All commands
   - All skills
   - All agents
   - All documentation

**Impact**: 15+ component renames, complete taxonomy clarity

---

### Backward Compatibility Strategy

**Option 1: Alias Period (Recommended)**

- Keep old names as symlinks/aliases for 1-2 releases
- Add deprecation warnings when old names used
- Remove aliases in future major version

**Option 2: Hard Cutover**

- Change all names at once
- Update all documentation
- Announce breaking change

**Recommendation**: Option 1 for user-facing components (commands, skills)

---

## Validation Criteria

After reorganization, the taxonomy should satisfy:

1. **Discoverability**: User can infer component purpose from name alone
2. **Consistency**: Similar components follow same naming pattern
3. **Hierarchy**: Related components share prefix/namespace
4. **Flow**: Sequential components have order-indicating names
5. **Simplicity**: Shortest name that conveys meaning

**Test**: Can a new user understand the system structure by reading component names in alphabetical order?

---

## Appendix: Complete Component Inventory

### Skills (17)

```
aesthetic-dreaming
build-automation
context-resume
deep-research
design-sync
plugin-ideation
plugin-improve
plugin-lifecycle
plugin-packaging
plugin-planning
plugin-testing
plugin-workflow
system-setup
troubleshooting-docs
ui-mockup
ui-template-library
workflow-reconciliation
```

### Agents (7)

```
dsp-agent
foundation-agent
gui-agent
research-agent
shell-agent
troubleshooter
validator
```

### Commands (20)

```
add-critical-pattern
clean
continue
destroy
doc-fix
dream
implement
improve
install-plugin
package
pfs
plan
reconcile
research
reset-to-ideation
setup
show-standalone
sync-design
test
uninstall
```

---

## Conclusion

The Plugin Freedom System's taxonomy reflects **incremental growth without architectural reorganization**. Components were named as features were added, resulting in inconsistent patterns that obscure the underlying workflow structure.

**The core issue**: The `plugin-*` namespace suggests a flat feature set when the system implements a **7-stage sequential workflow with branching validation and maintenance paths**.

**Recommended approach**:

1. **Short-term** (Priority 1-2): Fix agent naming, align command-skill mapping
2. **Medium-term** (Priority 3-4): Reorganize UI family, add phase prefixes
3. **Long-term** (Priority 5-6): Full taxonomy reorganization with lifecycle-based namespacing

**Expected outcome**: Component names reveal system structure. New users understand workflow from directory listing.
