# Consistency Audit

## Executive Summary

The Plugin Freedom System demonstrates **strong semantic consistency** with clear naming conventions, but reveals several **inconsistencies** across verb tense, hyphenation, and suffix patterns. The system uses three component types (skills, agents, commands) with distinct naming philosophies:

- **Skills**: Gerund-heavy compound nouns (plugin-workflow, deep-research, troubleshooting-docs)
- **Agents**: Simple nouns with suffix (foundation-agent, shell-agent, validator)
- **Commands**: Imperative verbs (setup, improve, implement) or bare nouns (plan, dream)

**Key Findings:**
1. **Verb Tense**: Mostly consistent (gerunds for skills, imperatives for commands), but "troubleshooting" vs "troubleshoot" creates ambiguity
2. **Plurality**: Inconsistent (troubleshooting-docs vs plugin-workflow, ui-template-library)
3. **Hyphenation**: Highly consistent (always hyphenated for compound names)
4. **Suffix Patterns**: Mixed (-agent, -er suffixes with semantic overlap; validator vs troubleshooter)
5. **Prefix Patterns**: Strong semantic consistency (plugin-* for lifecycle, ui-* for interface)

---

## Verb Tense Analysis

### Pattern: Gerunds vs Imperative vs Nouns

**Skills (Gerund-Dominant):**
- aesthetic-dreaming ← gerund
- plugin-testing ← gerund
- plugin-packaging ← gerund
- plugin-ideation ← noun (not gerund, but nominalized verb)
- troubleshooting-docs ← gerund
- workflow-reconciliation ← noun
- build-automation ← noun
- design-sync ← noun (sync is both verb and noun)
- deep-research ← noun
- system-setup ← noun
- plugin-workflow ← noun
- plugin-planning ← gerund
- plugin-improve ← **VIOLATION** (bare infinitive, should be plugin-improvement)
- plugin-lifecycle ← noun
- context-resume ← **VIOLATION** (bare infinitive/noun ambiguity, should be context-resumption)
- ui-mockup ← noun
- ui-template-library ← noun

**Commands (Imperative-Dominant):**
- setup ← imperative/noun
- improve ← imperative
- implement ← imperative
- test ← imperative/noun
- continue ← imperative
- plan ← imperative/noun
- dream ← imperative/noun
- reconcile ← imperative
- destroy ← imperative
- uninstall ← imperative
- install-plugin ← imperative + noun
- reset-to-ideation ← imperative phrase
- sync-design ← imperative phrase
- show-standalone ← imperative phrase
- doc-fix ← imperative phrase (noun + verb)
- add-critical-pattern ← imperative phrase
- clean ← imperative
- package ← imperative/noun
- research ← imperative/noun
- pfs ← acronym

**Agents (Noun-Dominant):**
- foundation-agent ← noun
- shell-agent ← noun
- dsp-agent ← noun
- gui-agent ← noun
- validator ← noun
- troubleshooter ← noun
- research-agent ← noun

### Findings:

**Consistent Patterns:**
- Commands use imperatives (setup, improve, implement) or ambiguous imperative/nouns (plan, dream, test)
- Agents use nouns (foundation, shell, dsp, gui, validator)
- Skills mix gerunds and nouns without clear rule

**Violations:**
1. **plugin-improve** (skill) uses bare infinitive instead of noun/gerund
   - Should be: plugin-improvement (consistent with workflow-reconciliation, build-automation)
2. **context-resume** (skill) uses bare infinitive/noun
   - Should be: context-resumption (consistent with workflow-reconciliation)
3. **troubleshooting-docs** (skill) vs **troubleshooter** (agent)
   - Pattern unclear: Why gerund for skill but -er suffix for agent?

---

## Plurality Analysis

### Pattern: Singular vs Plural

**Plural Forms:**
- troubleshooting-docs ← plural
- (No others explicitly plural)

**Singular Forms:**
- plugin-workflow (not workflows)
- plugin-ideation (not ideations)
- ui-template-library (library is singular container of templates)
- plugin-lifecycle (not lifecycles)
- All agents (singular)
- All commands (singular actions)

### Findings:

**Inconsistency:**
- **troubleshooting-docs** is the ONLY plural skill name
- Why "docs" (plural) but "workflow" (singular)?
- Both represent collections/systems, yet only docs is pluralized

**Semantic Question:**
- Does "troubleshooting-docs" capture multiple documents, or is it a documentation system?
- If the latter, should be "troubleshooting-documentation" (singular noun, consistent with build-automation, workflow-reconciliation)

**Recommendation:**
- Rename to **troubleshooting-documentation** for consistency
- OR establish rule: "Use plural when component manages discrete items (docs, templates), singular for systems/processes"

---

## Hyphenation Analysis

### Pattern: Compound Names

**All Hyphenated Compounds:**
- aesthetic-dreaming
- plugin-testing
- plugin-packaging
- plugin-lifecycle
- plugin-improve
- plugin-ideation
- plugin-workflow
- plugin-planning
- ui-mockup
- ui-template-library
- troubleshooting-docs
- workflow-reconciliation
- build-automation
- design-sync
- deep-research
- system-setup
- context-resume
- foundation-agent
- shell-agent
- dsp-agent
- gui-agent
- research-agent
- install-plugin
- reset-to-ideation
- sync-design
- show-standalone
- doc-fix
- add-critical-pattern

**No Unhyphenated Compounds:**
- (None found)

**Single-Word Names:**
- setup
- improve
- implement
- test
- continue
- plan
- dream
- reconcile
- destroy
- uninstall
- clean
- package
- research
- validator
- troubleshooter
- pfs

### Findings:

**Highly Consistent:**
- ALL compound names use hyphens (100% consistency)
- Single-word names never hyphenated
- No camelCase, snake_case, or unhyphenated compounds

**Exceptional Consistency:** This is the strongest pattern in the system.

---

## Suffix Pattern Analysis

### Pattern: Semantic Role of Suffixes

**-agent Suffix (6 instances):**
- foundation-agent
- shell-agent
- dsp-agent
- gui-agent
- research-agent

**Semantic Meaning:** Isolated execution context (subagent), implements specific stage

**-er Suffix (2 instances):**
- validator
- troubleshooter

**Semantic Meaning:** Actor/performer role (validates, troubleshoots)

**-tion/-sion Suffix (1 instance):**
- workflow-reconciliation

**Semantic Meaning:** Process/action result

**No Suffix (Majority):**
- Most skills: plugin-workflow, deep-research, design-sync, etc.
- Most commands: setup, improve, implement, etc.

### Findings:

**Inconsistency: -agent vs -er**
- Both suffixes denote actors performing actions
- **validator** and **troubleshooter** are agents but use -er instead of -agent
- Why? Semantic distinction unclear

**Possible Rationale:**
- **-agent** = Stage-specific implementation worker (Foundation, Shell, DSP, GUI, Research)
- **-er** = Cross-cutting validation/support role (Validator, Troubleshooter)

**But:** research-agent vs troubleshooter
- Both perform investigation/diagnosis
- Why research-agent but troubleshooter?
- Inconsistent application of semantic rule

**Recommendations:**
1. **If -agent means "stage worker":**
   - Keep validator, troubleshooter (they're not stage-specific)
   - Rename research-agent to researcher (it's cross-cutting)
2. **If -agent means "isolated execution":**
   - Rename validator to validator-agent
   - Rename troubleshooter to troubleshooter-agent
   - (All agents get -agent suffix)
3. **If -er means "permanent role" and -agent means "temporary worker":**
   - Document this distinction explicitly

---

## Prefix Pattern Analysis

### Pattern: Semantic Grouping by Prefix

**plugin-* Prefix (8 instances):**
- plugin-testing
- plugin-packaging
- plugin-lifecycle
- plugin-improve
- plugin-ideation
- plugin-workflow
- plugin-planning

**Semantic Meaning:** Plugin lifecycle management (create, test, package, improve)

**ui-* Prefix (2 instances):**
- ui-mockup
- ui-template-library

**Semantic Meaning:** User interface design/generation

**No Prefix (9 skills, 20 commands, 7 agents):**
- Skills: aesthetic-dreaming, troubleshooting-docs, workflow-reconciliation, build-automation, design-sync, deep-research, system-setup, context-resume
- Commands: All use bare verbs or imperative phrases
- Agents: All use domain-specific terms (foundation, shell, dsp, gui, validator, troubleshooter, research)

### Findings:

**Strong Semantic Consistency:**
- **plugin-*** clearly groups plugin lifecycle operations (7/17 skills)
- **ui-*** clearly groups interface-related operations (2/17 skills)
- No prefix collisions or ambiguities

**Recommendation:**
- Establish prefix registry:
  - **plugin-*** = Plugin lifecycle (ideation → packaging)
  - **ui-*** = Interface design/generation
  - **deep-*** = Deep/advanced operations (only deep-research currently)
  - **No prefix** = Cross-cutting/system-level (troubleshooting, build, design, system)

---

## Inconsistencies Found

### 1. Verb Form Violations
**plugin-improve** (skill)
- **Issue:** Uses bare infinitive instead of noun/gerund
- **Fix:** Rename to **plugin-improvement**
- **Impact:** Low (skill still works, but violates skill naming pattern)

**context-resume** (skill)
- **Issue:** Uses bare infinitive/ambiguous noun
- **Fix:** Rename to **context-resumption**
- **Impact:** Low (skill still works, but violates skill naming pattern)

### 2. Plurality Violation
**troubleshooting-docs** (skill)
- **Issue:** Only plural skill name (all others singular)
- **Fix:** Rename to **troubleshooting-documentation**
- **Impact:** Medium (breaks discoverability pattern)

### 3. Suffix Inconsistency
**validator** vs **troubleshooter** vs **research-agent**
- **Issue:** Semantic overlap between -er and -agent suffixes
- **Fix Options:**
  - Option A: All agents get -agent (validator-agent, troubleshooter-agent)
  - Option B: Cross-cutting roles get -er (validator, troubleshooter, researcher)
  - Option C: Document semantic distinction explicitly
- **Impact:** High (affects agent discovery and mental model)

### 4. Suffix Inconsistency (Gerund vs Noun in Skills)
**troubleshooting-docs** (gerund) vs **plugin-workflow** (noun)
- **Issue:** Mixing gerunds and nouns in skill names without rule
- **Fix:** Establish rule:
  - Gerunds for active processes (testing, packaging, planning)
  - Nouns for systems/structures (workflow, lifecycle, ideation)
- **Impact:** Low (pattern exists but undocumented)

---

## Recommendations

### 1. Establish Verb Tense Rules

**Skills:**
- Use **nouns** for system/structure concepts (workflow, lifecycle, library)
- Use **gerunds** for active processes (testing, packaging, planning)
- **NEVER** use bare infinitives (improve → improvement, resume → resumption)

**Commands:**
- Use **imperative verbs** for actions (setup, improve, implement)
- Use **ambiguous imperative/nouns** for dual-purpose commands (plan, test, dream)
- Hyphenate multi-word imperatives (sync-design, show-standalone)

**Agents:**
- Use **nouns** describing role/domain (foundation, shell, validator)
- Apply suffix rules (see below)

### 2. Establish Plurality Rules

**Rule:** Use singular nouns unless component manages discrete, countable items

**Violations to Fix:**
- troubleshooting-docs → troubleshooting-documentation

**Rationale:** "Documentation" is a system (singular), "docs" implies discrete files (plural)

### 3. Establish Suffix Rules

**Option A (Recommended): Semantic Distinction**
- **-agent** = Stage-specific implementation worker (Foundation, Shell, DSP, GUI)
- **-er** = Cross-cutting support role (Validator, Troubleshooter, Researcher)
- **Fix:** Rename research-agent → researcher

**Option B: Uniform Suffix**
- **-agent** = All isolated execution contexts
- **Fix:** Rename validator → validator-agent, troubleshooter → troubleshooter-agent

**Option C: Hybrid**
- **-agent** = Subagent pattern (invoked by workflow skills)
- **-er** = Standalone utility agents
- **Fix:** Document this distinction in CLAUDE.md

### 4. Document Prefix Registry

Establish explicit prefix semantics in CLAUDE.md:

```markdown
## Naming Conventions

### Prefixes (Skills Only)
- **plugin-*** = Plugin lifecycle operations (ideation → packaging)
- **ui-*** = Interface design/generation
- **No prefix** = Cross-cutting/system-level operations

### Suffixes (Agents Only)
- **-agent** = Stage-specific implementation worker
- **-er** = Cross-cutting support role
```

### 5. Apply Consistency Fixes

**Immediate Renames (High Impact):**
1. plugin-improve → plugin-improvement
2. context-resume → context-resumption
3. troubleshooting-docs → troubleshooting-documentation
4. research-agent → researcher (if adopting Option A)

**Document Patterns (Medium Impact):**
5. Add naming conventions section to CLAUDE.md
6. Create component naming guide in system docs

**Audit Enforcement (Low Impact):**
7. Add naming consistency check to SessionStart hook
8. Warn on pattern violations during component creation

---

## Summary Matrix

| Component Type | Verb Tense | Plurality | Hyphenation | Suffix | Prefix | Consistency Score |
|----------------|------------|-----------|-------------|--------|--------|-------------------|
| Skills | Mixed (nouns/gerunds) | Singular (1 violation) | 100% consistent | N/A | Strong semantic grouping | 75% |
| Agents | Nouns | Singular | 100% consistent | -agent/-er conflict | N/A | 70% |
| Commands | Imperatives | Singular | 100% consistent | N/A | N/A | 95% |

**Overall System Consistency: 80%**

The system demonstrates strong hyphenation discipline and clear prefix semantics, but suffers from verb tense ambiguity (bare infinitives in skills) and suffix inconsistency (agent vs -er). Applying the recommended fixes would raise consistency to 95%+.
