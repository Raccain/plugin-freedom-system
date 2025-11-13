# Direction Test Audit - Plugin Freedom System

**Date:** 2025-11-13
**Auditor:** Claude (Sonnet 4.5)
**Files Analyzed:** 17 skills, 5 agents

## Executive Summary

The Plugin Freedom System shows **moderate direction clarity** with significant room for improvement. Most components (14 of 22) have ambiguous I/O direction in their names alone, requiring context or description parsing to determine what they read vs. write. The clearest naming is in data flow skills (input-clear: `context-resume`, `system-setup`; output-clear: `plugin-packaging`, `aesthetic-dreaming`). The most ambiguous naming is in operational verbs that don't indicate data flow (`plugin-improve`, `plugin-workflow`, `design-sync`).

**Key Finding:** The system heavily relies on operational verbs (improve, sync, test) rather than data-flow verbs (capture, generate, validate, load) which creates directional ambiguity.

---

## Clear Direction Examples

### Input-Clear (Read/Consume)

These components **clearly read/load/fetch** data:

1. **context-resume** (skill)
   - **Name clarity:** ✅ EXCELLENT - "resume" implies loading saved state
   - **Actual function:** Loads `.continue-here.md` handoff files to resume work
   - **I/O profile:** Reads handoff → Outputs workflow resumption
   - **Why clear:** "Resume" universally means "continue from saved state" = read operation

2. **system-setup** (skill)
   - **Name clarity:** ✅ GOOD - "setup" implies configuration/validation
   - **Actual function:** Validates dependencies, reads system state
   - **I/O profile:** Reads system config → Writes `system-config.json`
   - **Why clear:** "Setup" implies inspection first, configuration second

### Output-Clear (Write/Produce)

These components **clearly write/generate/produce** data:

1. **plugin-packaging** (skill)
   - **Name clarity:** ✅ EXCELLENT - "packaging" implies creating distribution artifacts
   - **Actual function:** Creates PKG installer files for distribution
   - **I/O profile:** Reads plugin binaries → Writes PKG installer
   - **Why clear:** "Packaging" universally means wrapping/bundling = output operation

2. **aesthetic-dreaming** (skill)
   - **Name clarity:** ✅ GOOD - "dreaming" implies ideation/creation
   - **Actual function:** Captures aesthetic templates through questioning, writes `aesthetic.md`
   - **I/O profile:** User input → Writes aesthetic files
   - **Why clear:** "Dreaming" implies creation/generation

3. **ui-mockup** (skill)
   - **Name clarity:** ✅ GOOD - "mockup" implies visual creation
   - **Actual function:** Generates UI mockup files (YAML, HTML, C++)
   - **I/O profile:** Design input → Writes 7 mockup files
   - **Why clear:** "Mockup" is a concrete deliverable = output

4. **troubleshooting-docs** (skill)
   - **Name clarity:** ⚠️ MISLEADING - sounds like reading docs, actually **writes** them
   - **Actual function:** **Captures** solved problems as documentation
   - **I/O profile:** Problem solution → Writes resolution docs
   - **Direction confusion:** "docs" noun is ambiguous (read or write?)
   - **Better name:** `capture-solutions` or `document-fixes`

### Transform-Clear (Bidirectional)

These components **clearly do both read and write**:

1. **plugin-improve** (skill)
   - **Name clarity:** ⚠️ AMBIGUOUS - "improve" doesn't indicate I/O
   - **Actual function:** Reads plugin code → Modifies → Tests → Documents → Commits
   - **I/O profile:** Heavy bidirectional (reads source, writes changes, reads tests, writes CHANGELOG)
   - **Why transform:** Improves *existing* code (input) and produces *modified* code (output)

2. **workflow-reconciliation** (skill)
   - **Name clarity:** ⚠️ AMBIGUOUS - "reconciliation" doesn't indicate read vs write
   - **Actual function:** Reads state files → Validates → Writes corrections
   - **I/O profile:** Reads `.continue-here.md`, PLUGINS.md → Writes corrected state
   - **Why transform:** Compares (read) and fixes (write) state

---

## Ambiguous Direction

### Cannot Determine from Name Alone

These components require reading descriptions to understand I/O:

#### Skills

1. **plugin-ideation**
   - **Name:** Generic operational verb
   - **Direction:** Bidirectional (questionnaire → writes creative-brief.md)
   - **Ambiguity:** "Ideation" sounds like thinking/brainstorming, not clear if it reads or writes
   - **Suggested:** `capture-plugin-idea` (output-clear)

2. **plugin-planning**
   - **Name:** Generic operational verb
   - **Direction:** Bidirectional (reads brief → writes architecture.md, plan.md)
   - **Ambiguity:** "Planning" could mean reviewing plans or creating them
   - **Suggested:** `generate-implementation-plan` (output-clear)

3. **plugin-testing**
   - **Name:** Verb without I/O implication
   - **Direction:** Bidirectional (reads plugin → runs tests → writes logs)
   - **Ambiguity:** "Testing" = action, not data flow
   - **Suggested:** `validate-plugin` (still ambiguous) or `run-validation-suite` (clearer)

4. **plugin-workflow**
   - **Name:** Abstract orchestration term
   - **Direction:** Orchestrator (reads contracts → invokes subagents → writes state)
   - **Ambiguity:** "Workflow" = process, not data movement
   - **Suggested:** `orchestrate-implementation` (still abstract but clearer)

5. **plugin-lifecycle**
   - **Name:** Abstract management term
   - **Direction:** Bidirectional (reads binaries → installs → writes to system folders)
   - **Ambiguity:** "Lifecycle" covers install/uninstall/destroy = multiple directions
   - **Suggested:** Keep as-is (compound operations legitimately bidirectional)

6. **design-sync**
   - **Name:** **Highly ambiguous** verb
   - **Direction:** Bidirectional (reads brief + mockup → validates → writes updated contracts)
   - **Ambiguity:** "Sync" could mean fetch, push, or bidirectional synchronization
   - **Suggested:** `validate-design-alignment` (input-focus) or `detect-design-drift` (clearer)

7. **deep-research**
   - **Name:** Process-oriented
   - **Direction:** Input-heavy (reads troubleshooting docs, web → outputs findings)
   - **Ambiguity:** "Research" = investigation, not clear if reads or produces
   - **Suggested:** `investigate-problem` (still ambiguous) or `fetch-research-findings` (clearer)

8. **build-automation**
   - **Name:** Process-oriented
   - **Direction:** Transform (reads source → compiles → writes binaries)
   - **Ambiguity:** "Automation" doesn't indicate I/O
   - **Suggested:** `compile-plugin` (clearer verb)

9. **ui-template-library**
   - **Name:** **Noun-based** (library = storage)
   - **Direction:** Bidirectional (save/apply/list operations)
   - **Ambiguity:** "Library" is a container, not an action
   - **Suggested:** Split into `save-aesthetic` and `apply-aesthetic` operations (already done internally)

10. **workflow-reconciliation**
    - **Name:** Technical term without clear direction
    - **Direction:** Bidirectional (reads state → validates → writes fixes)
    - **Ambiguity:** "Reconciliation" = comparison + correction
    - **Suggested:** `repair-workflow-state` (output-focus on fixing)

#### Agents

All 5 agents have **ambiguous direction** because they're named by **stage** not **function**:

1. **foundation-agent**
   - **Name:** Stage identifier, not function
   - **Direction:** Output (generates CMakeLists.txt, boilerplate)
   - **Ambiguity:** "Foundation" = base layer, not an action
   - **Suggested:** `generate-build-system` (output-clear)

2. **shell-agent**
   - **Name:** Stage identifier, not function
   - **Direction:** Bidirectional (reads parameter-spec.md → writes APVTS code)
   - **Ambiguity:** "Shell" = wrapper/structure, not data flow
   - **Suggested:** `implement-parameters` (transform-clear)

3. **dsp-agent**
   - **Name:** Domain identifier (DSP = Digital Signal Processing)
   - **Direction:** Transform (reads architecture.md → writes processBlock code)
   - **Ambiguity:** "DSP" is a noun, not a verb
   - **Suggested:** `implement-audio-processing` (transform-clear)

4. **gui-agent**
   - **Name:** Domain identifier (GUI = Graphical User Interface)
   - **Direction:** Transform (reads mockup → integrates WebView → writes PluginEditor)
   - **Ambiguity:** "GUI" is a noun, not a verb
   - **Suggested:** `integrate-webview-ui` (transform-clear)

5. **research-agent** (mentioned but file not read)
   - **Name:** Process-oriented
   - **Direction:** Likely input-heavy (researches → produces findings)
   - **Ambiguity:** "Research" doesn't indicate read vs produce
   - **Suggested:** `investigate-architecture` or `fetch-juce-patterns`

---

## Specific Issues

### Read vs Write Confusion

#### troubleshooting-docs (CRITICAL CONFUSION)

**Name suggests:** Reading troubleshooting documentation
**Actual function:** **Writing/capturing** troubleshooting documentation
**Impact:** HIGH - Users might invoke expecting to read docs, but it captures new docs
**Fix:** Rename to `capture-solutions`, `document-fixes`, or `save-troubleshooting-entry`

#### ui-template-library (MODERATE CONFUSION)

**Name suggests:** A static library (read-only reference)
**Actual function:** Bidirectional save/apply operations
**Impact:** MODERATE - "Library" implies storage, not clear if read-only or read-write
**Fix:** Already mitigated by operation names (save_aesthetic, apply_aesthetic), but skill name could be `manage-aesthetic-templates`

### Bidirectional Ambiguity

These components do both read and write but names don't indicate it:

1. **plugin-improve**
   - **Problem:** "Improve" is an operational verb, doesn't show I/O
   - **Reality:** Reads source → Investigates → Modifies → Tests → Commits → Documents
   - **Fix:** `modify-plugin` (clearer) or `apply-plugin-changes` (bidirectional-clear)

2. **design-sync**
   - **Problem:** "Sync" has multiple interpretations (fetch? push? compare?)
   - **Reality:** Reads contracts → Compares → May write updates
   - **Fix:** `validate-design-alignment` or `check-contract-drift`

3. **workflow-reconciliation**
   - **Problem:** "Reconciliation" is accounting jargon
   - **Reality:** Reads state → Detects gaps → Writes fixes
   - **Fix:** `repair-workflow-state` or `fix-state-drift`

---

## Recommendations

### High Priority (Critical Confusion)

1. **Rename `troubleshooting-docs` → `capture-solutions`**
   - **Reason:** Current name misleads (sounds like reading docs)
   - **Impact:** HIGH - Prevents user confusion about purpose
   - **Verb:** "capture" = output-clear

2. **Rename `design-sync` → `validate-design-alignment`**
   - **Reason:** "Sync" is ambiguous (fetch/push/both?)
   - **Impact:** HIGH - Clarifies read-focus with potential write
   - **Verb:** "validate" = input-focus (reads to check)

### Medium Priority (Operational Clarity)

3. **Rename `plugin-improve` → `modify-plugin` or `apply-changes`**
   - **Reason:** "Improve" doesn't indicate I/O direction
   - **Impact:** MEDIUM - "Modify" clearly implies transform
   - **Verb:** "modify" = transform-clear

4. **Rename `plugin-ideation` → `capture-plugin-idea`**
   - **Reason:** "Ideation" is abstract (brainstorming ≠ data flow)
   - **Impact:** MEDIUM - "Capture" clearly implies output
   - **Verb:** "capture" = output-clear

5. **Rename `plugin-planning` → `generate-implementation-plan`**
   - **Reason:** "Planning" could mean review or create
   - **Impact:** MEDIUM - "Generate" clearly implies output
   - **Verb:** "generate" = output-clear

### Low Priority (Consistency Improvements)

6. **Rename agents to function-based names:**
   - `foundation-agent` → `generate-build-system`
   - `shell-agent` → `implement-parameters`
   - `dsp-agent` → `implement-audio-processing`
   - `gui-agent` → `integrate-webview-ui`
   - **Reason:** Stage names (foundation, shell, dsp, gui) don't indicate what they do
   - **Impact:** LOW - Descriptions compensate, but names could be clearer
   - **Benefit:** New users understand function from name alone

7. **Rename `build-automation` → `compile-plugin`**
   - **Reason:** "Automation" doesn't indicate compile operation
   - **Impact:** LOW - Context makes it clear
   - **Verb:** "compile" = transform-clear

8. **Rename `deep-research` → `investigate-problem`**
   - **Reason:** "Research" is process-oriented, not data-oriented
   - **Impact:** LOW - "Investigate" is clearer but still abstract
   - **Verb:** "investigate" = input-focus

---

## Naming Pattern Analysis

### Current Patterns

The system uses these naming patterns:

1. **Operational verbs** (14/22 components): `plugin-improve`, `plugin-testing`, `design-sync`
   - **Problem:** Operation ≠ data flow
   - **Result:** Ambiguous I/O direction

2. **Domain nouns** (5/22 components): `dsp-agent`, `gui-agent`, `ui-mockup`
   - **Problem:** Noun describes subject, not action
   - **Result:** Ambiguous I/O direction

3. **Data-flow verbs** (3/22 components): `context-resume`, `plugin-packaging`, `aesthetic-dreaming`
   - **Success:** Clear directionality
   - **Result:** CLEAR I/O direction

### Recommended Pattern

**Use data-flow verbs that indicate I/O direction:**

| Direction | Verb Examples | Plugin Freedom Usage |
|-----------|---------------|----------------------|
| **Input** (read/consume) | `load`, `fetch`, `read`, `validate`, `check`, `resume`, `parse` | `context-resume` ✅, `system-setup` ✅ |
| **Output** (write/produce) | `generate`, `create`, `write`, `capture`, `save`, `build`, `package` | `plugin-packaging` ✅, `aesthetic-dreaming` ✅, `ui-mockup` ✅ |
| **Transform** (both) | `modify`, `transform`, `convert`, `process`, `compile`, `implement` | `build-automation` (could use `compile`), agents (could use `implement`) |

### Avoid

- **Abstract operations:** `improve`, `sync`, `reconcile`, `automate`
- **Stage identifiers:** `foundation`, `shell`, `dsp`, `gui`
- **Nouns without verbs:** `library`, `workflow`, `lifecycle`

---

## Comparison: Before vs After Recommendations

### Skills

| Current Name | Direction | Recommended Name | Direction Clarity |
|--------------|-----------|------------------|-------------------|
| aesthetic-dreaming | ⚠️ Ambiguous | `capture-aesthetic` | ✅ Output-clear |
| build-automation | ⚠️ Ambiguous | `compile-plugin` | ✅ Transform-clear |
| context-resume | ✅ Input-clear | *(keep)* | ✅ Input-clear |
| deep-research | ⚠️ Ambiguous | `investigate-problem` | ✅ Input-clear |
| design-sync | ❌ Very ambiguous | `validate-design-alignment` | ✅ Input-clear |
| plugin-ideation | ⚠️ Ambiguous | `capture-plugin-idea` | ✅ Output-clear |
| plugin-improve | ⚠️ Ambiguous | `modify-plugin` | ✅ Transform-clear |
| plugin-lifecycle | ⚠️ Ambiguous | *(keep - compound operations)* | ⚠️ Legitimately complex |
| plugin-packaging | ✅ Output-clear | *(keep)* | ✅ Output-clear |
| plugin-planning | ⚠️ Ambiguous | `generate-implementation-plan` | ✅ Output-clear |
| plugin-testing | ⚠️ Ambiguous | `validate-plugin` | ✅ Input-clear |
| plugin-workflow | ⚠️ Ambiguous | `orchestrate-implementation` | ⚠️ Still abstract (orchestrator) |
| system-setup | ✅ Input-clear | *(keep)* | ✅ Input-clear |
| troubleshooting-docs | ❌ Misleading | `capture-solutions` | ✅ Output-clear |
| ui-mockup | ✅ Output-clear | *(keep)* | ✅ Output-clear |
| ui-template-library | ⚠️ Ambiguous | `manage-aesthetic-templates` | ⚠️ Still abstract (manager) |
| workflow-reconciliation | ⚠️ Ambiguous | `repair-workflow-state` | ✅ Output-clear |

### Agents

| Current Name | Direction | Recommended Name | Direction Clarity |
|--------------|-----------|------------------|-------------------|
| foundation-agent | ⚠️ Ambiguous | `generate-build-system` | ✅ Output-clear |
| shell-agent | ⚠️ Ambiguous | `implement-parameters` | ✅ Transform-clear |
| dsp-agent | ⚠️ Ambiguous | `implement-audio-processing` | ✅ Transform-clear |
| gui-agent | ⚠️ Ambiguous | `integrate-webview-ui` | ✅ Transform-clear |

---

## Impact Assessment

### Critical (Fix Immediately)

1. **troubleshooting-docs** - Misleading name creates confusion
2. **design-sync** - "Sync" has multiple meanings

### High (Fix Soon)

3. **plugin-improve** - Core workflow component, used frequently
4. **plugin-ideation** - Entry point for new plugins

### Medium (Fix Eventually)

5. **plugin-planning** - Used less frequently, context compensates
6. **Agents** - Stage names conventional but unclear

### Low (Optional)

7. **build-automation** - Context makes purpose clear
8. **ui-template-library** - Operation names already clear

---

## Conclusion

The Plugin Freedom System would benefit from **systematic renaming** to use **data-flow verbs** instead of **operational verbs**. This would make component purposes immediately clear without requiring description parsing. The most critical fixes are `troubleshooting-docs` → `capture-solutions` and `design-sync` → `validate-design-alignment`, which currently mislead users about I/O direction.

**Overall Score:** 3/10 direction clarity (14 of 22 components ambiguous)
**After Recommendations:** 8/10 direction clarity (18 of 22 components clear)
