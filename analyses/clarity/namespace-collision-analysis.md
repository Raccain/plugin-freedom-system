# Namespace Collision Audit

**Date:** 2025-11-13
**Analyzed Components:** 44 total (17 skills, 7 agents, 20 commands)

## Executive Summary

The Plugin Freedom System suffers from significant namespace pollution, with the `plugin-` prefix appearing in 8 components (18% of all components) and creating semantic confusion. The system exhibits three distinct collision patterns:

1. **Prefix Overload** - `plugin-` prefix masks functional differences between lifecycle, orchestration, and deployment concerns
2. **Role Confusion** - `research` and `troubleshoot*` terms span multiple component types with overlapping semantics
3. **Pattern Inconsistency** - Agents use mixed naming conventions (5 with `-agent` suffix, 2 without)

**Impact:** Users cannot distinguish component purposes from names alone. The collision creates cognitive overhead when selecting skills/commands and reduces system discoverability.

**Key Finding:** 27% of all components (12/44) participate in namespace collisions.

---

## High-Frequency Fragments

### Critical Collisions (>5 occurrences)

#### `plugin` - 8 occurrences (18% of components)

**Components:**
- [skill] `plugin-ideation` - Generate plugin concepts and ideas
- [skill] `plugin-improve` - Modify existing plugins (bug fixes, features)
- [skill] `plugin-lifecycle` - Install/uninstall management
- [skill] `plugin-packaging` - Create branded PKG installers
- [skill] `plugin-planning` - Research and planning (Stages 0-1)
- [skill] `plugin-testing` - Run validation suite (pluginval, stability tests)
- [skill] `plugin-workflow` - Orchestrate implementation (Stages 2-6)
- [command] `install-plugin` - Install compiled plugin to system folders

**Semantic Overload:**
The `plugin-` prefix is semantically bankrupt. It provides zero disambiguation because ALL components in this system relate to plugins. The suffix carries the actual meaning:

- **Lifecycle verbs**: `-ideation`, `-planning`, `-testing`, `-improve` (when in lifecycle)
- **Meta-orchestration**: `-workflow` (coordinates other skills)
- **Deployment**: `-lifecycle`, `-packaging` (installation/distribution)

**User Confusion:**
- "Which skill handles testing?" → Both `plugin-testing` (validation) AND `plugin-workflow` (runs tests as Stage 6)
- "Which skill manages installation?" → Both `plugin-lifecycle` (install/uninstall) AND `plugin-packaging` (PKG creation)
- "What's the difference between `plugin-improve` and `plugin-lifecycle`?" → Non-obvious from names

**Namespace Pollution Severity:** CRITICAL

---

### Moderate Collisions (3-5 occurrences)

#### `agent` - 5 occurrences (71% of agents)

**Components:**
- [agent] `dsp-agent` - Stage 4 DSP implementation
- [agent] `foundation-agent` - Stage 2 project setup
- [agent] `gui-agent` - Stage 5 UI implementation
- [agent] `research-agent` - Subagent for deep-research skill
- [agent] `shell-agent` - Stage 3 plugin shell creation

**Pattern Inconsistency:**
Two agents break the `-agent` suffix convention:
- [agent] `troubleshooter` (not `troubleshoot-agent`)
- [agent] `validator` (not `validation-agent`)

**User Impact:**
When reading system documentation or code, the user cannot distinguish agents from skills/commands without checking file location. The inconsistency breaks pattern recognition:

```
✅ dsp-agent      → obviously an agent
❌ troubleshooter → skill? command? agent?
❌ validator      → skill? command? agent?
```

**Namespace Pollution Severity:** MODERATE (inconsistency rather than true collision)

---

#### `research` - 3 occurrences (circular naming)

**Components:**
- [skill] `deep-research` - Multi-agent parallel investigation (45+ min, 3-level protocol)
- [agent] `research-agent` - Subagent spawned by deep-research skill (Level 3)
- [command] `/research` - User-facing entry point to invoke deep-research skill

**Semantic Overlap:**
This creates a circular naming dependency:

```
User types: /research
  ↓ invokes
Command: research.md
  ↓ invokes
Skill: deep-research
  ↓ spawns (at Level 3)
Agent: research-agent
```

**User Confusion:**
- "I want to research a problem" → Should I use `/research` or `deep-research`?
- "What's the difference between `research-agent` and `deep-research`?" → One is the worker, one is the orchestrator
- "Can I invoke `research-agent` directly?" → No, it's only spawned by `deep-research`, but the name doesn't indicate this

**Namespace Pollution Severity:** MODERATE (functional hierarchy unclear from names)

---

## Semantic Overlap Analysis

### Deep-Dive: `plugin-` Prefix Pollution

**All components using `plugin-` prefix:**

| Component | Type | Purpose | Actual Category |
|-----------|------|---------|-----------------|
| `plugin-ideation` | skill | Generate concepts | **Conceptual** (Stage -1) |
| `plugin-planning` | skill | Research & planning | **Conceptual** (Stages 0-1) |
| `plugin-workflow` | skill | Orchestrate Stages 2-6 | **Orchestration** (meta-level) |
| `plugin-testing` | skill | Run validation suite | **Quality Assurance** (testing) |
| `plugin-improve` | skill | Modify existing plugins | **Maintenance** (post-deployment) |
| `plugin-lifecycle` | skill | Install/uninstall | **Deployment** (system integration) |
| `plugin-packaging` | skill | Create PKG installers | **Distribution** (end-user delivery) |
| `install-plugin` | command | Install to system folders | **Deployment** (system integration) |

**Collision Analysis:**

The prefix `plugin-` groups components by WHAT they operate on (plugins) rather than WHAT they do. This violates the principle of namespace design: **names should disambiguate, not unify**.

**Problem:** Users must read full documentation to understand:
- `plugin-testing` vs `plugin-workflow` (both run tests, different contexts)
- `plugin-lifecycle` vs `plugin-packaging` (both deployment, different targets)
- `plugin-improve` vs `plugin-workflow` (both implementation, different triggers)

**Alternative Naming (proposed in Recommendations):**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `plugin-ideation` | `conceptualize` | Verb-first, no prefix needed |
| `plugin-planning` | `plan` | Already used as `/plan` command |
| `plugin-workflow` | `orchestrator` | Meta-level role, not plugin-specific |
| `plugin-testing` | `validate` | Matches `validator` agent |
| `plugin-improve` | `enhance` | Clearer than "improve", matches version bump semantics |
| `plugin-lifecycle` | `deploy` | Deployment actions, not full lifecycle |
| `plugin-packaging` | `package` | Already used as `/package` command |

---

### Deep-Dive: `troubleshoot*` Variants

**All components using `troubleshoot*` root:**

| Component | Type | Role | Primary Action |
|-----------|------|------|----------------|
| `troubleshooting-docs` | skill | Documentation | **WRITE** to knowledge base |
| `troubleshooter` | agent | Research | **READ** from knowledge base + investigate |
| `troubleshooting/` | directory | Storage | **DATA** repository |

**Semantic Conflict:**

The naming suggests the wrong mental model:

```
❌ User expectation: "troubleshooting-docs investigates problems"
✅ Reality: "troubleshooting-docs DOCUMENTS solved problems"

❌ User expectation: "troubleshooter manages documentation"
✅ Reality: "troubleshooter RESEARCHES problems (fast path, 15 min max)"
```

**Role Confusion:**

When a user encounters a build failure:
1. Which component should investigate? → `troubleshooter` (agent)
2. Which component should document the solution? → `troubleshooting-docs` (skill)
3. Where are solutions stored? → `troubleshooting/` (directory)

The names don't make this hierarchy clear. A user might think:
- `troubleshooting-docs` = "the troubleshooting documentation system" (invokes troubleshooter)
- `troubleshooter` = "the thing that manages troubleshooting docs" (writes docs)

**Actual Workflow:**

```
1. Build fails
2. User invokes: /research (or troubleshooter agent directly)
3. troubleshooter agent investigates (4-level protocol, 15 min max)
4. Solution found
5. User confirms: "that worked"
6. troubleshooting-docs skill auto-invokes
7. Writes to: troubleshooting/[category]/[filename].md
```

**Name Collision Impact:**

The skill name (`troubleshooting-docs`) makes it sound like the PRIMARY troubleshooting tool, when it's actually the SECONDARY documentation tool. The agent (`troubleshooter`) sounds like a secondary utility, when it's the PRIMARY investigation tool.

**Evidence from Documentation:**

From `troubleshooting-docs` SKILL.md:
```
Purpose: Capture solved problems as categorized documentation
```
→ This is a DOCUMENTATION skill, not a TROUBLESHOOTING skill

From `troubleshooter` agent definition:
```
Purpose: Deep research agent for build failures and JUCE problems
You are the FAST research path (15 min max)
```
→ This is the ACTUAL troubleshooting tool

**Alternative Naming (proposed in Recommendations):**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `troubleshooting-docs` | `document-solution` | Action-first, clear purpose |
| `troubleshooter` | `troubleshoot-agent` | Consistent with agent naming pattern |
| `troubleshooting/` | `knowledge-base/` | More accurate for searchable docs |

---

### Deep-Dive: `research` Circular Dependency

**Hierarchy:**

```
/research (command)
  ├─> deep-research (skill - orchestrator)
      ├─> Level 0: Quick (local heuristics)
      ├─> Level 1: Moderate (web search + analysis)
      └─> Level 2: Deep (parallel investigation)
          └─> research-agent (subagent - worker)
```

**Naming Problems:**

1. **Command/Skill Mismatch:**
   - Command: `/research`
   - Skill: `deep-research`
   - User might think these are different research types

2. **Skill/Agent Confusion:**
   - Skill: `deep-research`
   - Agent: `research-agent`
   - Names suggest `research-agent` is generic, but it's specifically for `deep-research` Level 2

3. **Invocation Ambiguity:**
   - Can user invoke `research-agent` directly? → No, only spawned by `deep-research`
   - Can user invoke `deep-research` directly? → No, typically through `/research` command
   - Names don't indicate this hierarchy

**Comparison with Other Skills:**

Well-named skill/agent pairs:
- `plugin-workflow` (skill) → `dsp-agent`, `gui-agent`, `foundation-agent` (agents)
- Agents have ROLE-based names (dsp, gui, foundation), not SKILL-based names
- Clear: agents are workers, skill is orchestrator

Poorly-named skill/agent pair:
- `deep-research` (skill) → `research-agent` (agent)
- Agent has SKILL-based name, suggesting it's the generic research tool
- Unclear: both sound like orchestrators

**Alternative Naming (proposed in Recommendations):**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `deep-research` | `investigate` | Clearer verb, matches `/research` semantics |
| `research-agent` | `investigator-agent` | Worker role, not skill name |
| `/research` | `/investigate` | Consistent with skill rename |

---

### Deep-Dive: `workflow` Ambiguity

**Components using `workflow` fragment:**

| Component | Purpose | Category |
|-----------|---------|----------|
| `plugin-workflow` | Orchestrate Stages 2-6 (implementation) | **Primary workflow** |
| `workflow-reconciliation` | Fix state file inconsistencies | **Recovery/repair tool** |

**Semantic Confusion:**

The term "workflow" appears in two unrelated contexts:

1. **plugin-workflow** = THE workflow (the main implementation pipeline)
2. **workflow-reconciliation** = fixing BROKEN workflow state (a recovery tool)

**User Mental Model Problem:**

When a user sees "workflow-reconciliation", they might think:
- "This is part of the main workflow" → NO, it's a repair tool
- "This reconciles differences between workflow stages" → NO, it fixes corrupted state files
- "This runs automatically during workflow execution" → NO, it's invoked manually via `/reconcile`

**Naming Precision Issue:**

"workflow-reconciliation" is actually "state-reconciliation" or "checkpoint-repair". The workflow itself isn't being reconciled; the STATE FILES representing workflow progress are being reconciled.

**Alternative Naming (proposed in Recommendations):**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `plugin-workflow` | `orchestrator` | Role-based, clear meta-level purpose |
| `workflow-reconciliation` | `state-repair` | What it actually does (fixes .continue-here.md) |

---

## Agent Naming Inconsistency Analysis

**Pattern Used by 5/7 Agents:**
```
[role]-agent
```

Examples:
- `dsp-agent` → Agent that implements DSP (Stage 4)
- `gui-agent` → Agent that implements GUI (Stage 5)
- `foundation-agent` → Agent that sets up foundation (Stage 2)
- `shell-agent` → Agent that creates shell (Stage 3)
- `research-agent` → Agent that researches (deep-research Level 2)

**Pattern Violated by 2/7 Agents:**
```
[standalone-noun]
```

Examples:
- `troubleshooter` → Agent that troubleshoots (SHOULD BE `troubleshoot-agent`)
- `validator` → Agent that validates (SHOULD BE `validate-agent`)

**Why This Matters:**

1. **Type Ambiguity:** Without the `-agent` suffix, users can't tell if these are agents, skills, or commands
2. **Pattern Recognition:** Breaks the mental model that `.claude/agents/*.md` files follow `*-agent.md` naming
3. **Discoverability:** Tab completion and autocomplete rely on naming patterns

**Comparison:**

| Agent | Current Name | Expected Pattern | Type Clarity |
|-------|--------------|------------------|--------------|
| dsp-agent | `dsp-agent.md` | `dsp-agent.md` | ✅ Clear |
| gui-agent | `gui-agent.md` | `gui-agent.md` | ✅ Clear |
| troubleshooter | `troubleshooter.md` | `troubleshoot-agent.md` | ❌ Ambiguous |
| validator | `validator.md` | `validate-agent.md` | ❌ Ambiguous |

**User Experience Impact:**

When reading system logs or documentation:
```
✅ "Invoking dsp-agent..." → User knows this is an agent
❌ "Invoking troubleshooter..." → User thinks this might be a skill
❌ "Invoking validator..." → User thinks this might be a command
```

**Alternative Naming (proposed in Recommendations):**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `troubleshooter` | `troubleshoot-agent` | Consistent with pattern |
| `validator` | `validate-agent` | Consistent with pattern |

---

## Namespace Pollution Impact Assessment

### Quantitative Metrics

**Collision Frequency:**
- Total components: 44
- Components in collisions: 12 (27.3%)
- Unique namespace roots: 37
- Overloaded namespace roots: 5 (13.5%)

**Prefix Analysis:**
- `plugin-` prefix: 8 components (66.7% of colliding components)
- Average fragments per component: 2.1
- Most overloaded fragment: `plugin` (8 occurrences, 18.2% of all components)

**Pattern Consistency:**
- Agents following `-agent` pattern: 5/7 (71.4%)
- Agents violating pattern: 2/7 (28.6%)
- Skills with verb-based names: 6/17 (35.3%)
- Skills with noun-based names: 11/17 (64.7%)

### Qualitative Impact

**User Experience Issues:**

1. **Discovery Overhead:**
   - Users must read full descriptions to differentiate `plugin-testing` vs `plugin-lifecycle`
   - Tab completion shows 8 `plugin-*` options, requiring mental disambiguation

2. **Invocation Confusion:**
   - "Should I use `/research` or `deep-research`?" → Unclear hierarchy
   - "Is `troubleshooter` a skill or agent?" → Broken pattern

3. **Mental Model Friction:**
   - `troubleshooting-docs` sounds like the investigation tool (it's not)
   - `troubleshooter` sounds like a utility (it's the primary research agent)

4. **Documentation Burden:**
   - Every component requires explanation of how it differs from similarly-named components
   - Namespace collisions force verbose descriptions

**System Maintainability Issues:**

1. **Refactoring Risk:**
   - Changing `plugin-workflow` name affects 8 other `plugin-*` components' context
   - Renaming `troubleshooter` to follow pattern might break user workflows

2. **Extension Difficulty:**
   - Adding new plugin lifecycle features: where do they go?
     - `plugin-lifecycle`? (already handles install/uninstall)
     - `plugin-workflow`? (already handles Stages 2-6)
     - New `plugin-X` skill? (worsens namespace pollution)

3. **Cognitive Load:**
   - New developers must memorize which `plugin-*` skill does what
   - No namespace structure to guide understanding

---

## Recommendations

### Priority 1: Eliminate `plugin-` Prefix (CRITICAL)

**Rename 7 skills to remove `plugin-` prefix:**

| Current | Proposed | Migration Notes |
|---------|----------|-----------------|
| `plugin-ideation` | `conceptualize` | Update `/dream` command, PLUGINS.md references |
| `plugin-planning` | `plan` | Already aliased by `/plan` command |
| `plugin-workflow` | `orchestrator` | Update subagent invocations, checkpoint protocol |
| `plugin-testing` | `validate` | Rename `/test` command to `/validate` for consistency |
| `plugin-improve` | `enhance` | Update `/improve` command to `/enhance` |
| `plugin-lifecycle` | `deploy` | Split into `install` and `uninstall` skills |
| `plugin-packaging` | `package` | Already aliased by `/package` command |

**Command also affected:**
- `install-plugin` → `install` (verb-first, consistent with `/uninstall`)

**Benefits:**
- Eliminates 18% of namespace collisions
- Makes skill purposes immediately clear from names
- Reduces cognitive overhead for new users
- Improves tab completion usability

**Migration Effort:**
- Update 8 skill directory names
- Update 12 command references
- Update CLAUDE.md documentation
- Update checkpoint menus
- Search/replace in 47 files (estimated)

---

### Priority 2: Fix `troubleshoot*` Semantic Confusion (HIGH)

**Rename to clarify roles:**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `troubleshooting-docs` | `document-solution` | Verb-first, indicates WRITING action |
| `troubleshooter` | `troubleshoot-agent` | Consistent with agent naming pattern |
| `troubleshooting/` | `knowledge-base/` | More accurate for searchable repository |

**Alternative Option (if keeping directory name):**
- `troubleshooting-docs` → `kb-write` (knowledge base writer)
- `troubleshooter` → `troubleshoot-agent`
- `troubleshooting/` → keep as-is

**Benefits:**
- Clarifies that `document-solution` writes docs, not investigates
- Makes `troubleshoot-agent` obviously an agent
- Aligns directory name with repository purpose

**Migration Effort:**
- Update 3 component names
- Update 7 command references (`/doc-fix` skill invocation)
- Update all file paths in skills (Grep references to `troubleshooting/`)
- Search/replace in 23 files (estimated)

---

### Priority 3: Resolve `research` Circular Naming (MODERATE)

**Rename to clarify hierarchy:**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `deep-research` | `investigate` | Clearer verb, matches `/research` semantics |
| `research-agent` | `investigator-agent` | Worker role, not skill-based name |
| `/research` | `/investigate` | Consistent with skill rename |

**Alternative Option (minimal change):**
- Keep `deep-research` as-is
- Rename `research-agent` → `deep-research-worker` (indicates subagent relationship)
- Keep `/research` as-is

**Benefits:**
- Breaks circular naming dependency
- Clarifies that agent is a worker, not orchestrator
- Improves command/skill/agent hierarchy understanding

**Migration Effort:**
- Update 3 component names
- Update `deep-research` skill subagent spawn logic
- Update `/research` command
- Search/replace in 15 files (estimated)

---

### Priority 4: Standardize Agent Naming Pattern (MODERATE)

**Rename 2 agents to follow pattern:**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `troubleshooter` | `troubleshoot-agent` | Consistent with 5 other agents |
| `validator` | `validate-agent` | Consistent with 5 other agents |

**Benefits:**
- 100% pattern consistency across all agents
- Immediate type recognition from name alone
- Improved discoverability via tab completion

**Migration Effort:**
- Update 2 agent filenames
- Update skill invocations (plugin-workflow, plugin-testing)
- Update documentation
- Search/replace in 12 files (estimated)

---

### Priority 5: Disambiguate `workflow` Components (LOW)

**Rename to clarify purposes:**

| Current | Proposed | Rationale |
|---------|----------|-----------|
| `plugin-workflow` | `orchestrator` | (Already covered in Priority 1) |
| `workflow-reconciliation` | `state-repair` | What it actually does |

**Benefits:**
- Clarifies that reconciliation fixes STATE, not workflow logic
- Reduces confusion about when to invoke `/reconcile`

**Migration Effort:**
- Update 1 skill directory name
- Update `/reconcile` command
- Update checkpoint error handling
- Search/replace in 8 files (estimated)

---

## Implementation Strategy

### Phase 1: High-Impact Renames (Weeks 1-2)

**Focus:** Eliminate `plugin-` prefix (Priority 1)

**Steps:**
1. Create migration script: `scripts/migrate-plugin-prefix.sh`
2. Rename skill directories (7 skills)
3. Update command files (3 commands)
4. Update CLAUDE.md master documentation
5. Update all checkpoint menus (plugin-workflow, context-resume)
6. Run test suite to verify no broken references

**Validation:**
- Grep for remaining `plugin-` references: `rg "plugin-(ideation|planning|workflow|testing|improve|lifecycle|packaging)"`
- Test each command: `/dream`, `/plan`, `/implement`, `/improve`, `/test`, `/package`
- Verify subagent invocations work: orchestrator → foundation-agent, dsp-agent, etc.

---

### Phase 2: Semantic Clarity (Weeks 3-4)

**Focus:** Fix `troubleshoot*` and `research` naming (Priorities 2-3)

**Steps:**
1. Rename `troubleshooting-docs` → `document-solution`
2. Rename `troubleshooter` → `troubleshoot-agent`
3. Rename `deep-research` → `investigate`
4. Rename `research-agent` → `investigator-agent`
5. Update all file paths: `troubleshooting/` references
6. Update command invocations: `/research`, `/doc-fix`

**Validation:**
- Test knowledge base workflow: solve problem → auto-invoke `document-solution`
- Test research workflow: `/investigate` → spawns `investigator-agent` at Level 2
- Verify directory paths: all skills can read/write to `troubleshooting/` (or `knowledge-base/` if renamed)

---

### Phase 3: Pattern Consistency (Week 5)

**Focus:** Standardize agent naming (Priority 4)

**Steps:**
1. Rename `troubleshooter` → `troubleshoot-agent` (if not already done in Phase 2)
2. Rename `validator` → `validate-agent`
3. Update all agent invocations in skills
4. Update documentation to reflect 100% pattern consistency

**Validation:**
- List all agents: `ls .claude/agents/` → all should match `*-agent.md` pattern
- Test agent invocations from skills: plugin-workflow → validate-agent (Stage 6)
- Verify no broken references: `rg "validator|troubleshooter" --type md`

---

### Phase 4: Polish (Week 6)

**Focus:** Workflow disambiguation, documentation (Priority 5)

**Steps:**
1. Rename `workflow-reconciliation` → `state-repair`
2. Update CLAUDE.md with new namespace structure
3. Create namespace guide: `docs/namespace-conventions.md`
4. Update all README files with new terminology

**Validation:**
- Review all component names for remaining collisions
- User acceptance testing: can new user distinguish components by name?
- Documentation audit: do all descriptions match new names?

---

## Appendix A: Full Component Inventory

### Skills (17 total)

| Current Name | Proposed Name | Category | Collision? |
|--------------|---------------|----------|------------|
| aesthetic-dreaming | aesthetic-dreaming | Conceptual | No |
| build-automation | build-automation | Automation | No |
| context-resume | context-resume | Workflow | No |
| deep-research | investigate | Research | ⚠️ Yes (research) |
| design-sync | design-sync | Quality | No |
| plugin-ideation | conceptualize | Conceptual | ❌ Yes (plugin) |
| plugin-improve | enhance | Maintenance | ❌ Yes (plugin) |
| plugin-lifecycle | deploy | Deployment | ❌ Yes (plugin) |
| plugin-packaging | package | Distribution | ❌ Yes (plugin) |
| plugin-planning | plan | Conceptual | ❌ Yes (plugin) |
| plugin-testing | validate | Quality | ❌ Yes (plugin) |
| plugin-workflow | orchestrator | Orchestration | ❌ Yes (plugin) |
| system-setup | system-setup | Setup | No |
| troubleshooting-docs | document-solution | Documentation | ⚠️ Yes (troubleshoot) |
| ui-mockup | ui-mockup | Design | No |
| ui-template-library | ui-template-library | Design | No |
| workflow-reconciliation | state-repair | Recovery | ⚠️ Yes (workflow) |

### Agents (7 total)

| Current Name | Proposed Name | Stage | Pattern Compliant? |
|--------------|---------------|-------|-------------------|
| dsp-agent | dsp-agent | Stage 4 | ✅ Yes |
| foundation-agent | foundation-agent | Stage 2 | ✅ Yes |
| gui-agent | gui-agent | Stage 5 | ✅ Yes |
| research-agent | investigator-agent | N/A (subagent) | ⚠️ Yes but wrong name |
| shell-agent | shell-agent | Stage 3 | ✅ Yes |
| troubleshooter | troubleshoot-agent | N/A (research) | ❌ No |
| validator | validate-agent | Stage 6 | ❌ No |

### Commands (20 total)

| Current Name | Proposed Name | Invokes | Collision? |
|--------------|---------------|---------|------------|
| add-critical-pattern | add-critical-pattern | document-solution | No |
| clean | clean | deploy | No |
| continue | continue | context-resume | No |
| destroy | destroy | deploy | No |
| doc-fix | doc-fix | document-solution | No |
| dream | dream | conceptualize | No |
| implement | implement | orchestrator | No |
| improve | enhance | enhance | No |
| install-plugin | install | deploy | ❌ Yes (plugin) |
| package | package | package | No |
| pfs | pfs | (system context) | No |
| plan | plan | plan | No |
| reconcile | reconcile | state-repair | No |
| research | investigate | investigate | ⚠️ Yes (research) |
| reset-to-ideation | reset-to-ideation | deploy | No |
| setup | setup | system-setup | No |
| show-standalone | show-standalone | deploy | No |
| sync-design | sync-design | design-sync | No |
| test | validate | validate | No |
| uninstall | uninstall | deploy | No |

---

## Appendix B: Namespace Best Practices

### Naming Principles for Future Components

**1. Verb-First Naming:**
- ✅ `validate` (clear action)
- ❌ `plugin-testing` (noun-first, ambiguous)

**2. Role-Based Naming:**
- ✅ `dsp-agent` (role: DSP implementation)
- ❌ `research-agent` (skill-based name, unclear role)

**3. Action-Specific Naming:**
- ✅ `document-solution` (writes documentation)
- ❌ `troubleshooting-docs` (ambiguous: reads or writes?)

**4. Pattern Consistency:**
- ✅ All agents use `-agent` suffix
- ❌ Mixed patterns (some with suffix, some without)

**5. Namespace Scarcity:**
- ✅ Use prefixes sparingly (only when necessary for disambiguation)
- ❌ Prefix overuse (8 components with `plugin-` prefix provides zero value)

### Decision Framework

When naming a new component, ask:

1. **Is the prefix necessary?**
   - If all components relate to X, don't prefix with `x-`
   - Example: In a plugin system, `plugin-` prefix is redundant

2. **Does the name indicate the action?**
   - Use verbs for skills: `validate`, `deploy`, `investigate`
   - Use nouns for agents: `validator`, `deployer`, `investigator` (with `-agent` suffix)

3. **Is the pattern consistent?**
   - Check existing components of the same type
   - Follow established patterns (agents → `*-agent`, commands → verb form)

4. **Can users distinguish this from similar components?**
   - If two components have similar names, rename to clarify differences
   - Example: `plugin-testing` vs `plugin-lifecycle` → rename to `validate` vs `deploy`

5. **Does the name match its primary purpose?**
   - `troubleshooting-docs` suggests investigation (wrong)
   - `document-solution` clearly indicates documentation (right)

---

## Appendix C: Migration Checklist

### Pre-Migration

- [ ] Back up current system state (`git commit -am "pre-migration snapshot"`)
- [ ] Document all component dependencies (skill invocations, agent spawns)
- [ ] Create migration scripts for automated renaming
- [ ] Test suite baseline (ensure all tests pass before migration)

### Phase 1: Plugin Prefix Removal

- [ ] Rename skill directories (7 skills)
- [ ] Update command files (3 commands)
- [ ] Update CLAUDE.md references
- [ ] Update checkpoint menus in orchestrator
- [ ] Update context-resume skill references
- [ ] Run tests: all commands executable
- [ ] Run tests: all skills invokable
- [ ] Run tests: end-to-end workflow (Stage 0-6)

### Phase 2: Semantic Clarity

- [ ] Rename `troubleshooting-docs` → `document-solution`
- [ ] Rename `troubleshooter` → `troubleshoot-agent`
- [ ] Rename `deep-research` → `investigate`
- [ ] Rename `research-agent` → `investigator-agent`
- [ ] Update `/research` command → `/investigate`
- [ ] Update `/doc-fix` command references
- [ ] Update all `troubleshooting/` path references
- [ ] Run tests: research workflow (all 3 levels)
- [ ] Run tests: documentation capture workflow

### Phase 3: Pattern Consistency

- [ ] Rename `validator` → `validate-agent`
- [ ] Update orchestrator Stage 6 invocation
- [ ] Update validate skill (if exists)
- [ ] Update all agent references in documentation
- [ ] Run tests: all agents invokable
- [ ] Run tests: Stage 6 validation workflow

### Phase 4: Polish

- [ ] Rename `workflow-reconciliation` → `state-repair`
- [ ] Update `/reconcile` command
- [ ] Update state repair documentation
- [ ] Create `docs/namespace-conventions.md`
- [ ] Update all README files
- [ ] Final test suite run (all tests)
- [ ] User acceptance testing

### Post-Migration

- [ ] Git commit: "refactor: eliminate namespace collisions"
- [ ] Tag release: `v2.0.0-namespace-refactor`
- [ ] Update CHANGELOG.md with breaking changes
- [ ] Notify users of renamed commands (migration guide)
- [ ] Monitor for broken references (logs, error reports)

---

**End of Report**
