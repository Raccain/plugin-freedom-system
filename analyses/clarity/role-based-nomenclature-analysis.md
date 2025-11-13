# Role-Based Nomenclature Audit

**Date:** 2025-11-13
**Auditor:** Claude Code
**Pattern Evaluated:** `{action}-{domain}-{type}`

---

## Executive Summary

The Plugin Freedom System exhibits **inconsistent adherence** to role-based naming conventions. While type suffixes (-agent, -skill, -command) are applied uniformly, **action verbs and domain qualifiers are frequently absent or weak**, resulting in noun-noun patterns that obscure purpose.

**Key Findings:**
- **17 Skills:** 6 strong verbs (35%), 11 weak/missing verbs (65%)
- **7 Agents:** 5 weak verbs (71%), 2 strong verbs (29%)
- **20+ Commands:** 8 strong verbs (40%), 12+ weak/noun forms (60%)
- **Domain Qualifiers:** Inconsistently applied; plugin-* prefix overused without action clarity

**Impact:** Names fail to communicate "what it does" at a glance. New users cannot infer function from component names alone.

**Recommendation Priority:**
1. **HIGH:** Skills without clear action verbs (aesthetic-dreaming, context-resume, design-sync)
2. **MEDIUM:** Agents with generic verbs (validator, troubleshooter)
3. **LOW:** Commands already using strong verbs (setup, implement, improve)

---

## Pattern Compliance Analysis

### Ideal Pattern: `{action}-{domain}-{type}`

**Examples of compliant names:**
- `validate-plugin-agent` (action: validate, domain: plugin, type: agent)
- `build-automation-skill` (action: build, domain: automation, type: skill)
- `generate-mockup-command` (action: generate, domain: mockup, type: command)

**Why this matters:**
- **Action verbs** answer "What does it do?"
- **Domain qualifiers** prevent namespace collision
- **Type suffixes** identify component category

---

## Components Following Pattern

### Strong Action Verbs + Domain Context

**Skills (6/17):**
1. ✅ `build-automation` → BUILD (action) + automation (domain) + skill (type)
2. ✅ `plugin-testing` → TEST (action) + plugin (domain) + skill (type)
3. ✅ `plugin-packaging` → PACKAGE (action) + plugin (domain) + skill (type)
4. ✅ `plugin-improve` → IMPROVE (action) + plugin (domain) + skill (type)
5. ✅ `troubleshooting-docs` → DOCUMENT (action, implicit) + troubleshooting (domain) + skill (type)
6. ✅ `system-setup` → SETUP (action) + system (domain) + skill (type)

**Agents (2/7):**
1. ✅ `validator` → VALIDATE (action, implicit in noun) + (missing domain) + agent (type)
2. ✅ `troubleshooter` → TROUBLESHOOT (action, implicit in noun) + (missing domain) + agent (type)

**Commands (8/20+):**
1. ✅ `/setup` → SETUP (clear action verb)
2. ✅ `/implement` → IMPLEMENT (clear action verb)
3. ✅ `/improve` → IMPROVE (clear action verb)
4. ✅ `/test` → TEST (clear action verb)
5. ✅ `/uninstall` → UNINSTALL (clear action verb)
6. ✅ `/destroy` → DESTROY (clear action verb)
7. ✅ `/package` → PACKAGE (clear action verb)
8. ✅ `/reconcile` → RECONCILE (clear action verb)

---

## Components Violating Pattern

### Weak or Missing Action Verbs

**Skills (11/17):**

1. ❌ **aesthetic-dreaming** → NOUN-NOUN pattern
   - Missing: Action verb
   - Domain: aesthetic (clear)
   - Issue: "dreaming" is not an action - it's a metaphorical noun
   - Fix: `design-aesthetic-skill` or `capture-aesthetic-skill`

2. ❌ **ui-mockup** → NOUN-NOUN pattern
   - Missing: Action verb
   - Domain: ui (clear)
   - Issue: "mockup" is a noun, not an action
   - Fix: `generate-ui-mockup-skill` or `create-ui-mockup-skill`

3. ❌ **plugin-lifecycle** → NOUN-NOUN pattern
   - Missing: Action verb
   - Domain: plugin (clear)
   - Issue: "lifecycle" is a noun describing scope, not action
   - Fix: `manage-plugin-lifecycle-skill`

4. ❌ **ui-template-library** → NOUN-NOUN-NOUN pattern
   - Missing: Action verb
   - Domain: ui-template (nested)
   - Issue: "library" is a storage concept, not an action
   - Fix: `manage-template-library-skill` or `apply-template-library-skill`

5. ❌ **context-resume** → NOUN-VERB pattern (weak verb)
   - Verb present but weak: "resume" is generic
   - Domain: context (vague)
   - Issue: Doesn't communicate what's being resumed (workflow? plugin? state?)
   - Fix: `resume-workflow-context-skill`

6. ❌ **plugin-ideation** → NOUN-NOUN pattern
   - Missing: Action verb
   - Domain: plugin (clear)
   - Issue: "ideation" is a noun (the process of ideating), not a verb
   - Fix: `brainstorm-plugin-skill` or `ideate-plugin-skill`

7. ❌ **workflow-reconciliation** → NOUN-NOUN pattern
   - Missing: Action verb
   - Domain: workflow (clear)
   - Issue: "reconciliation" is a noun (the result of reconciling)
   - Fix: `reconcile-workflow-state-skill`

8. ❌ **design-sync** → NOUN-NOUN pattern
   - Missing: Action verb (partially present)
   - Domain: design (clear)
   - Issue: "sync" could be noun or verb - ambiguous
   - Fix: `validate-design-sync-skill` or `sync-design-contracts-skill`

9. ❌ **deep-research** → ADJECTIVE-NOUN pattern
   - Missing: Action verb
   - Domain: research (clear but vague)
   - Issue: "deep" is adjective, "research" is noun
   - Fix: `investigate-problem-deeply-skill` or `research-juce-apis-skill`

10. ❌ **plugin-workflow** → NOUN-NOUN pattern
    - Missing: Action verb
    - Domain: plugin (clear)
    - Issue: "workflow" describes a process, not an action
    - Fix: `orchestrate-plugin-workflow-skill`

11. ❌ **plugin-planning** → NOUN-NOUN pattern
    - Missing: Action verb
    - Domain: plugin (clear)
    - Issue: "planning" is gerund (noun form of verb), but reads passive
    - Fix: `plan-plugin-architecture-skill`

**Agents (5/7):**

1. ❌ **research-agent** → NOUN-AGENT pattern
   - Missing: Clear action verb
   - Domain: research (vague - research what?)
   - Issue: "research" is a noun here, not a verb
   - Fix: `investigate-architecture-agent` or `research-dsp-apis-agent`

2. ❌ **foundation-agent** → NOUN-AGENT pattern
   - Missing: Action verb
   - Domain: foundation (vague - foundation of what?)
   - Issue: "foundation" describes a stage, not an action
   - Fix: `build-foundation-agent` or `scaffold-project-agent`

3. ❌ **shell-agent** → NOUN-AGENT pattern
   - Missing: Action verb
   - Domain: shell (ambiguous - shell as in terminal? or APVTS shell?)
   - Issue: "shell" is a metaphor for "parameter scaffolding"
   - Fix: `implement-parameters-agent` or `scaffold-apvts-agent`

4. ❌ **dsp-agent** → NOUN-AGENT pattern
   - Missing: Action verb
   - Domain: dsp (clear)
   - Issue: "dsp" is just a domain abbreviation
   - Fix: `implement-dsp-agent` or `process-audio-agent`

5. ❌ **gui-agent** → NOUN-AGENT pattern
   - Missing: Action verb
   - Domain: gui (clear)
   - Issue: "gui" is just a domain abbreviation
   - Fix: `integrate-gui-agent` or `bind-webview-agent`

**Commands (12+/20+):**

1. ❌ `/dream` → NOUN (metaphor)
   - Issue: "dream" is a metaphor for brainstorming - not immediately clear
   - Fix: `/brainstorm` or `/ideate-plugin`

2. ❌ `/plan` → WEAK VERB
   - Issue: Verb present but too generic - plan what?
   - Fix: `/plan-plugin` or `/design-architecture`

3. ❌ `/continue` → WEAK VERB
   - Issue: Verb present but generic - continue what?
   - Fix: `/continue-workflow` or `/resume-plugin`

4. ❌ `/clean` → WEAK VERB
   - Issue: Verb present but ambiguous - clean what? (files? builds? state?)
   - Fix: `/cleanup-plugin` or `/remove-binaries`

5. ❌ `/pfs` → ABBREVIATION
   - Issue: No verb, just an acronym
   - Fix: `/load-pfs-context` or `/show-system-architecture`

6. ❌ `/show-standalone` → VERB-NOUN (good!)
   - Actually COMPLIANT - verb + domain
   - No fix needed

7. ❌ `/install-plugin` → VERB-NOUN (good!)
   - Actually COMPLIANT - verb + domain
   - No fix needed

8. ❌ `/reset-to-ideation` → VERB-PREPOSITION-NOUN
   - Issue: Verbose and awkward phrasing
   - Fix: `/reset-plugin` or `/rollback-to-ideation`

9. ❌ `/doc-fix` → NOUN-NOUN (abbreviation + noun)
   - Issue: "doc" is abbreviated noun, "fix" could be noun or verb
   - Fix: `/document-fix` or `/capture-solution`

10. ❌ `/sync-design` → VERB-NOUN (good!)
    - Actually COMPLIANT - verb + domain
    - No fix needed

11. ❌ `/add-critical-pattern` → VERB-ADJECTIVE-NOUN (good!)
    - Actually COMPLIANT - verb + qualified noun
    - No fix needed

12. ❌ `/research` → NOUN/VERB (ambiguous)
    - Issue: Could be noun or verb - unclear intent
    - Fix: `/investigate-problem` or `/research-juce-apis`

---

## Verb Analysis

### Clear Action Verbs Present

**Skills:**
- ✅ `build-automation` → BUILD
- ✅ `plugin-testing` → TEST
- ✅ `plugin-packaging` → PACKAGE
- ✅ `plugin-improve` → IMPROVE
- ✅ `system-setup` → SETUP
- ✅ `troubleshooting-docs` → DOCUMENT (implicit in "troubleshooting")

**Agents:**
- ✅ `validator` → VALIDATE (implicit in noun form)
- ✅ `troubleshooter` → TROUBLESHOOT (implicit in noun form)

**Commands:**
- ✅ `/setup`, `/implement`, `/improve`, `/test`, `/uninstall`, `/destroy`, `/package`, `/reconcile`

### Weak/Missing Verbs

**Skills with noun-noun patterns:**
- ❌ `aesthetic-dreaming` → No verb
- ❌ `ui-mockup` → No verb
- ❌ `plugin-lifecycle` → No verb
- ❌ `ui-template-library` → No verb
- ❌ `plugin-ideation` → No verb (gerund used as noun)
- ❌ `workflow-reconciliation` → No verb (gerund used as noun)
- ❌ `deep-research` → No verb (adjective + noun)

**Skills with weak verbs:**
- ⚠️ `context-resume` → "resume" is too generic
- ⚠️ `design-sync` → "sync" is ambiguous (noun or verb?)

**Agents with noun-agent patterns:**
- ❌ `research-agent` → "research" used as noun
- ❌ `foundation-agent` → No verb
- ❌ `shell-agent` → No verb (metaphorical noun)
- ❌ `dsp-agent` → No verb (abbreviation)
- ❌ `gui-agent` → No verb (abbreviation)

**Commands with weak verbs:**
- ⚠️ `/dream` → Metaphorical noun, not clear verb
- ⚠️ `/plan` → Verb too generic
- ⚠️ `/continue` → Verb too generic
- ⚠️ `/clean` → Verb ambiguous
- ❌ `/pfs` → No verb (abbreviation)

---

## Domain Qualifier Analysis

### Components with Clear Domain Context

**Well-qualified:**
- ✅ `plugin-testing` → Domain: plugin
- ✅ `plugin-packaging` → Domain: plugin
- ✅ `plugin-improve` → Domain: plugin
- ✅ `plugin-lifecycle` → Domain: plugin
- ✅ `plugin-ideation` → Domain: plugin
- ✅ `plugin-workflow` → Domain: plugin
- ✅ `plugin-planning` → Domain: plugin
- ✅ `ui-mockup` → Domain: ui
- ✅ `ui-template-library` → Domain: ui-template
- ✅ `system-setup` → Domain: system
- ✅ `design-sync` → Domain: design
- ✅ `workflow-reconciliation` → Domain: workflow

**Underqualified (missing or vague domain):**
- ⚠️ `validator` → Missing domain (validates what? contracts? builds? state?)
- ⚠️ `troubleshooter` → Missing domain (troubleshoots what? builds? DSP? JUCE APIs?)
- ⚠️ `deep-research` → Vague domain (researches what? DSP? JUCE? architecture?)
- ⚠️ `context-resume` → Vague domain (what context? workflow? plugin? session?)
- ⚠️ `aesthetic-dreaming` → Domain present but verb missing
- ⚠️ `troubleshooting-docs` → Domain present but inverted (docs about troubleshooting)

**Overqualified (redundant plugin- prefix):**
- Plugin-specific skills use `plugin-*` prefix consistently, but this creates redundancy:
  - `plugin-testing`, `plugin-packaging`, `plugin-improve`, `plugin-lifecycle`, `plugin-ideation`, `plugin-workflow`, `plugin-planning`
  - All exist in plugin development context - prefix may be redundant
  - Consider: Is "plugin" necessary if everything is plugin-related?

### Domain Collision Risk

**Low collision risk (unique domains):**
- `ui-mockup`, `ui-template-library` → UI domain is clear
- `system-setup` → System-level operations
- `design-sync` → Design validation domain

**Potential collision (generic domains):**
- `research-agent` → Could collide with any research activity
- `validator` → Could validate contracts, builds, state, parameters - unclear
- `troubleshooter` → Could troubleshoot builds, DSP, JUCE, parameters - unclear

**Recommendation:** Add domain qualifiers to agents:
- `research-agent` → `research-architecture-agent`
- `validator` → `validate-contracts-agent`
- `troubleshooter` → `troubleshoot-builds-agent`

---

## Type Suffix Consistency

### Analysis

**Type suffix usage is 100% consistent and meaningful:**

- **Skills (17):** All use implicit `-skill` suffix (in directory name)
  - Files: `.claude/skills/[name]/SKILL.md`
  - Clear component category: reusable workflow units

- **Agents (7):** All use explicit `-agent` suffix
  - Files: `.claude/agents/[name]-agent.md`
  - Clear component category: subagents for stages

- **Commands (20+):** All use implicit `-command` suffix (in directory name)
  - Files: `.claude/commands/[name].md`
  - Clear component category: slash commands

**Type suffixes correctly distinguish:**
1. **Skills** → Complex workflows, can invoke other skills/agents
2. **Agents** → Isolated subagents, single-purpose, fresh context
3. **Commands** → Entry points, expand to prompts

**No recommendations for type suffixes - this aspect is working perfectly.**

---

## Recommendations

### Priority 1: HIGH (Critical for Usability)

Rename components with missing or weak verbs where function is unclear:

| Current Name | Issue | Recommended Name | Rationale |
|--------------|-------|------------------|-----------|
| `aesthetic-dreaming` | NOUN-NOUN | `design-aesthetic-template` | Clear action: design/capture aesthetic |
| `ui-mockup` | NOUN-NOUN | `generate-ui-mockup` | Clear action: generate/create mockup |
| `plugin-lifecycle` | NOUN-NOUN | `manage-plugin-lifecycle` | Clear action: manage installation/removal |
| `ui-template-library` | NOUN-NOUN-NOUN | `apply-aesthetic-template` | Clear action: apply saved aesthetics |
| `context-resume` | WEAK VERB | `resume-workflow-context` | Clear domain: workflow (not session/plugin) |
| `plugin-ideation` | NOUN-NOUN | `brainstorm-plugin-concept` | Clear action: brainstorm/ideate |
| `workflow-reconciliation` | NOUN-NOUN | `reconcile-workflow-state` | Clear action: reconcile (verb form) |
| `design-sync` | AMBIGUOUS | `validate-design-alignment` | Clear action: validate consistency |
| `deep-research` | ADJ-NOUN | `investigate-juce-problem` | Clear action: investigate/research |

### Priority 2: MEDIUM (Improves Clarity)

Add domain qualifiers to generic agents:

| Current Name | Issue | Recommended Name | Rationale |
|--------------|-------|------------------|-----------|
| `validator` | MISSING DOMAIN | `validate-stage-output` | Clarifies what's validated |
| `troubleshooter` | MISSING DOMAIN | `troubleshoot-build-failure` | Clarifies what's troubleshot |
| `research-agent` | NOUN-AGENT | `research-dsp-architecture` | Clear action + domain |
| `foundation-agent` | NOUN-AGENT | `scaffold-project-foundation` | Clear action (scaffold/build) |
| `shell-agent` | NOUN-AGENT | `implement-apvts-parameters` | Clear action + technical term |
| `dsp-agent` | NOUN-AGENT | `implement-audio-processing` | Clear action + domain |
| `gui-agent` | NOUN-AGENT | `integrate-webview-ui` | Clear action + technology |

### Priority 3: LOW (Minor Improvements)

Commands already using strong verbs, but some could be more specific:

| Current Name | Issue | Recommended Name | Rationale |
|--------------|-------|------------------|-----------|
| `/dream` | METAPHOR | `/brainstorm-plugin` | Clearer intent |
| `/plan` | TOO GENERIC | `/plan-architecture` | Specifies what's planned |
| `/continue` | TOO GENERIC | `/resume-workflow` | Specifies what's continued |
| `/clean` | AMBIGUOUS | `/cleanup-plugin-files` | Specifies what's cleaned |
| `/pfs` | ABBREVIATION | `/load-system-context` | Clear action |
| `/doc-fix` | ABBREVIATION | `/document-solution` | Clear action |
| `/research` | AMBIGUOUS | `/investigate-problem` | Clearer action |

### Pattern to Follow for Future Components

When creating new components, use this template:

```
{ACTION}-{DOMAIN}-{TYPE}
```

**Examples:**
- ✅ `validate-preset-files-skill` → VALIDATE (action) + preset files (domain) + skill (type)
- ✅ `compile-plugin-binaries-agent` → COMPILE (action) + plugin binaries (domain) + agent (type)
- ✅ `/export-installer-package` → EXPORT (action) + installer package (domain) + command (implicit)

**Action verb list (strong verbs to use):**
- CREATE, GENERATE, BUILD, SCAFFOLD
- VALIDATE, VERIFY, CHECK, TEST
- IMPLEMENT, INTEGRATE, BIND, CONNECT
- MANAGE, ORCHESTRATE, COORDINATE
- INVESTIGATE, RESEARCH, TROUBLESHOOT
- DOCUMENT, CAPTURE, RECORD
- APPLY, USE, LOAD, INVOKE
- INSTALL, DEPLOY, PACKAGE, DISTRIBUTE
- REMOVE, DESTROY, CLEANUP, UNINSTALL
- UPDATE, MODIFY, IMPROVE, ENHANCE
- SYNC, RECONCILE, ALIGN
- RESUME, CONTINUE, RESTORE

**Avoid weak verbs:**
- ❌ "handle" → Too vague (handle what? how?)
- ❌ "process" → Too vague (process what? how?)
- ❌ "manage" → Often too generic (be specific about what's managed)
- ❌ "work" → No semantic value
- ❌ Gerunds as nouns: "planning", "testing", "ideation" → Use verb forms: "plan", "test", "ideate"

---

## Impact Assessment

### Current Problems

1. **Discoverability:** New users cannot infer function from names
   - `aesthetic-dreaming` → What does this do? Dream about aesthetics?
   - `shell-agent` → Shell? Like terminal? Or something else?
   - `validator` → Validates what?

2. **Namespace collision:** Generic names create ambiguity
   - `research-agent` → Researches DSP? JUCE APIs? User behavior?
   - `troubleshooter` → Troubleshoots builds? Runtime? Parameters?

3. **Cognitive load:** Developers must memorize purpose instead of inferring
   - 11/17 skills require documentation lookup to understand
   - 5/7 agents require context to disambiguate

### Benefits of Renaming

1. **Self-documenting system:** Names communicate purpose immediately
2. **Reduced onboarding time:** New developers understand system faster
3. **Better autocomplete:** IDE suggestions become more meaningful
4. **Easier maintenance:** Future developers can reason about component roles

---

## Migration Strategy

### Phased Rollout

**Phase 1: High-Priority Skills (Week 1)**
- Rename top 5 confusing skills
- Update all references in hooks, commands, skills
- Test workflow continuity

**Phase 2: Agents (Week 2)**
- Rename all 7 agents with clear action verbs
- Update plugin-workflow orchestration calls
- Test subagent dispatch

**Phase 3: Commands (Week 3)**
- Rename ambiguous commands
- Create aliases for backward compatibility
- Update documentation

**Phase 4: Documentation (Week 4)**
- Update CLAUDE.md with new names
- Update all skill/agent internal references
- Update README and getting started guides

### Backward Compatibility

**Option 1: Aliases**
- Keep old names as aliases for 2-4 weeks
- Log deprecation warnings
- Remove aliases after transition period

**Option 2: Automatic redirection**
- Map old names to new names in command routing
- Display "Redirecting to new name..." message
- Remove redirects after 1 month

**Option 3: Hard cutover**
- Rename everything at once
- Update all references simultaneously
- Communicate changes clearly to users

**Recommendation:** Option 1 (Aliases) for least disruption.

---

## Conclusion

The Plugin Freedom System needs systematic renaming to achieve role-based clarity. Current naming prioritizes brevity over discoverability, resulting in 65% of skills and 71% of agents using weak or missing verbs.

**Next Steps:**
1. Review this audit with team/maintainers
2. Prioritize renames (start with HIGH priority)
3. Implement phased rollout with backward compatibility
4. Establish naming guidelines for future components

**Success Criteria:**
- 100% of components follow `{action}-{domain}-{type}` pattern
- New developers can infer component purpose from name alone
- Reduced documentation lookup time for common operations
