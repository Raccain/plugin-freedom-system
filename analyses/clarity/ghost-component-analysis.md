# Ghost Component Audit

**Conducted:** 2025-11-13
**System:** Plugin Freedom System
**Scope:** Agents, Skills, Commands, Documentation

---

## Executive Summary

Analysis reveals **1 major ghost component** (validator agent) that is fully defined, documented extensively, referenced in schemas and workflow docs, but **never invoked in actual workflows**. Additionally, 2 commands exist without skill routing, and several skills have limited integration points.

**Key Findings:**
- **validator agent**: 562-line specification, JSON schema, documentation references, but ZERO Task tool invocations
- **build-automation skill**: Functions as inline code rather than delegated skill
- **aesthetic-dreaming & ui-template-library**: Functional but underutilized in primary workflows
- **plugin-packaging & plugin-testing**: Working skills with appropriate usage patterns

**Impact:** Primary ghost (validator) creates documentation debt and false expectations. System appears to have semantic validation capability that doesn't exist in practice.

---

## Defined But Never Invoked

### Agents

#### 1. validator (MAJOR GHOST)

**Definition:** `.claude/agents/validator.md` (562 lines)

**Purpose:** Independent semantic validator for stage completions (Stages 0-6). Checks implementation alignment with creative intent, design soundness, code quality, JUCE best practices.

**Documentation presence:**
- Extensive agent specification (validator.md)
- JSON schema (`.claude/schemas/validator-report.json`)
- Referenced in `.claude/schemas/README.md`
- Mentioned in `CLAUDE.md` system docs
- Referenced in `README.md` (line 145: "Dispatcher Pattern")
- Documented in `stage-6-validation.md` with pseudo-code invocation example (lines 149-186)

**Expected invocation pattern:**
```typescript
const validation = Task({
  subagent_type: "validator",
  description: `Validate ${pluginName} Stage N`,
  prompt: [comprehensive validation prompt]
})
```

**Actual invocations:** **ZERO**

**Search results:**
- `grep -r "Task.*validator"` across `.claude/skills/`: No matches
- `grep -r "subagent_type.*validator"` across codebase: Only found in stage-6-validation.md reference doc (commented example)
- `grep -r "invoke.*validator"` in hooks: No Task tool invocations, only Python script calls

**Why it exists:**
- Originally designed for hybrid validation system (deterministic hooks + semantic validator)
- Hooks now handle 100% of validation (PostToolUse.sh, SubagentStop.sh with Python validators)
- validator agent became redundant when hooks matured

**Current validation approach:**
- **Stage 2-5**: SubagentStop.sh hook calls Python validators (validate-foundation.py, validate-parameters.py, validate-dsp-components.py, validate-gui-bindings.py)
- **Stage 6**: plugin-workflow directly handles testing, CHANGELOG, presets, status updates
- **Cross-contract**: validate-cross-contract.py and validate-checksums.py in SubagentStop.sh

**Validator's documented checks are actually performed by:**
- Contract immutability: PostToolUse.sh (blocks Edit/Write to .ideas/*.md)
- Silent failures: validate-silent-failures.py in PostToolUse.sh
- Stage artifacts: validate-foundation.py, validate-parameters.py, etc. in SubagentStop.sh
- Checksum validation: validate-checksums.py in SubagentStop.sh

**Impact:**
- **High documentation debt**: 562 lines + schema + references create expectation of semantic validation
- **Confusing to maintainers**: "Why isn't validator being called?"
- **False capability advertising**: README.md implies semantic validation exists
- **Schema maintenance burden**: validator-report.json serves no purpose

**Recommendation:**
- **ARCHIVE** validator.md to `.claude/agents/archive/validator-deprecated.md`
- **DELETE** `.claude/schemas/validator-report.json`
- **UPDATE** stage-6-validation.md to remove pseudo-code invocation example
- **UPDATE** README.md to clarify validation is hook-based, not agent-based
- **ADD** comment in archive explaining why: "Validator agent deprecated after hooks matured to handle all validation. Hooks provide faster, deterministic validation without subagent overhead."

---

### Skills

#### 1. build-automation (FUNCTIONAL BUT INLINE)

**Definition:** `.claude/skills/build-automation/SKILL.md`

**Purpose:** Orchestrate plugin builds via `scripts/build-and-install.sh`

**Expected pattern:** Invoked via Skill tool from other workflows

**Actual pattern:** Mixed - sometimes invoked as skill, often executed inline

**Invocation count:** 15 references across skill files, but many are inline bash calls to build script rather than Skill tool delegation

**Status:** **WORKING but inconsistent pattern**

**Recommendation:**
- **CLARIFY** invocation protocol - should ALL build operations go through build-automation skill, or is inline acceptable for simple cases?
- **STANDARDIZE** across plugin-workflow, plugin-improve, plugin-lifecycle

---

#### 2. aesthetic-dreaming (FUNCTIONAL BUT UNDERUTILIZED)

**Definition:** `.claude/skills/aesthetic-dreaming/SKILL.md`

**Purpose:** Create aesthetic templates without first creating a plugin - capture visual design concepts through adaptive questioning

**Invocation points:**
- `/dream` command (Option 4 menu)
- Natural language triggers (implied)

**Status:** **WORKING but niche**

**Usage evidence:** 19 references in codebase (mostly internal to skill itself)

**Recommendation:**
- **KEEP** - Functional and has clear use case (pre-plugin aesthetic capture)
- **PROMOTE** - Add to main workflow discovery (currently hidden in Option 4 of /dream)

---

#### 3. ui-template-library (FUNCTIONAL BUT UNDERUTILIZED)

**Definition:** `.claude/skills/ui-template-library/SKILL.md`

**Purpose:** Manage aesthetic templates - save visual systems from completed mockups, apply to new plugins

**Invocation points:**
- Natural language triggers ("save aesthetic", "apply aesthetic")
- `/dream` command references aesthetic library

**Status:** **WORKING but underutilized**

**Integration:** 19 references across aesthetic-dreaming and ui-mockup skills

**Recommendation:**
- **KEEP** - Functional and enables aesthetic reuse
- **DOCUMENT** better in main README.md (currently not mentioned)

---

## Invoked But Don't Exist

**Result:** NONE FOUND

All Task tool invocations reference existing agents:
- foundation-agent ✓
- shell-agent ✓
- dsp-agent ✓
- gui-agent ✓
- research-agent ✓
- troubleshooter ✓

All Skill tool invocations reference existing skills:
- plugin-workflow ✓
- plugin-planning ✓
- plugin-improve ✓
- plugin-lifecycle ✓
- plugin-testing ✓
- plugin-packaging ✓
- context-resume ✓
- deep-research ✓
- troubleshooting-docs ✓
- design-sync ✓
- system-setup ✓
- workflow-reconciliation ✓
- aesthetic-dreaming ✓
- ui-template-library ✓
- ui-mockup ✓
- build-automation ✓

---

## Documented But Not Implemented

### 1. validator Semantic Checks (GHOST FEATURES)

**Documented in:** `.claude/agents/validator.md` lines 243-467

**Examples:**
- "Does implementation match creative intent?" - NOT CHECKED (no validator invocation)
- "Are design decisions sound?" - NOT CHECKED
- "Code quality acceptable?" - Partially via hooks, not semantically
- "JUCE best practices followed (beyond compilation)?" - Partially via juce8-critical-patterns.md injection

**Reality:** Hooks perform deterministic pattern matching only. No semantic "does this match the brief's vision?" validation.

**Impact:** Documentation creates expectation of deep semantic review that doesn't exist.

---

### 2. Stage 6 Validator Invocation (DOCUMENTED BUT NOT IMPLEMENTED)

**Documented in:** `.claude/skills/plugin-workflow/references/stage-6-validation.md` lines 149-186

**Shows:** Detailed pseudo-code for Task tool invocation of validator

**Reality:** Stage 6 in actual plugin-workflow.md does NOT invoke validator. It:
1. Rebuilds plugin
2. Creates presets
3. Invokes plugin-testing skill (NOT validator)
4. Generates CHANGELOG
5. Updates PLUGINS.md
6. Deletes handoff
7. Invokes plugin-lifecycle for installation

**Impact:** Reference documentation shows feature that doesn't exist in orchestrator.

---

### 3. Validator Override System (IMPLEMENTED BUT UNUSED)

**Documented in:** `.claude/agents/validator.md` lines 513-551

**Feature:** `.validator-overrides.yaml` to suppress false positives

**Status:** No validator invocation = override system never executes

**File search:** No `.validator-overrides.yaml` files exist in any plugin directory

**Impact:** Documented feature has zero usage because parent feature (validator agent) is never invoked.

---

## Phantom References

### 1. Missing Validator Invocation in plugin-workflow

**File:** `.claude/skills/plugin-workflow/references/stage-6-validation.md`

**Lines 149-186:** Shows validator invocation with Task tool

**Problem:** Main orchestrator (`.claude/skills/plugin-workflow/SKILL.md`) does NOT contain this code

**Impact:** Reference docs suggest capability that orchestrator doesn't implement

**Recommendation:** DELETE lines 149-186 from stage-6-validation.md OR implement in actual workflow

---

### 2. Hooks vs Validator Confusion

**Multiple files reference "validator" ambiguously:**
- `.claude/hooks/SubagentStop.sh` calls Python validators (NOT validator agent)
- `.claude/agents/validator.md` agent (NEVER invoked)
- `.claude/schemas/validator-report.json` schema (NO consumer)

**Impact:** Term "validator" has 3 different meanings in codebase

**Recommendation:** Rename Python scripts to "checkers" or "verifiers" to distinguish from (deprecated) validator agent

---

### 3. Commands Without Skill Routing

#### /show-standalone

**File:** `.claude/commands/show-standalone.md`

**Pattern:** Direct bash execution, no skill delegation

**Status:** **WORKING AS DESIGNED** - simple utility command

**Recommendation:** KEEP - appropriate for single-purpose bash commands

#### /doc-fix

**File:** `.claude/commands/doc-fix.md`

**Pattern:** Routes to troubleshooting-docs skill

**Status:** **WORKING** - documented and implemented

#### /add-critical-pattern

**File:** `.claude/commands/add-critical-pattern.md`

**Pattern:** Direct execution, no skill delegation

**Status:** **WORKING AS DESIGNED** - fast path to Required Reading

**Recommendation:** KEEP - appropriate for direct operations

---

## Impact Analysis

### How Ghosts Confuse Users/Developers

#### 1. validator Agent (HIGH IMPACT)

**User confusion:**
- "Why am I getting validation errors if there's a validator agent?"
- "Can I customize validation by modifying validator.md?"
- "How do I invoke the validator for my custom checks?"

**Developer confusion:**
- "Should I update validator.md when adding new checks?"
- "Why are there two validation systems (hooks + agent)?"
- "Is validator-report.json still used?"

**Maintenance burden:**
- 562 lines of unused agent specification
- JSON schema serving no purpose
- Documentation references creating false expectations

**Technical debt:**
- Validator invocation code in stage-6-validation.md suggests unimplemented feature
- Schema validation code in README.md references unused schema
- README.md "Dispatcher Pattern" section implies validator is part of workflow

---

#### 2. build-automation Pattern Inconsistency (MEDIUM IMPACT)

**Developer confusion:**
- "When should I invoke build-automation skill vs call build script directly?"
- "Why does plugin-workflow sometimes inline bash and sometimes delegate to build-automation?"

**Impact:** Inconsistent pattern makes it unclear when to use skill vs inline execution

---

#### 3. Aesthetic Skills Visibility (LOW IMPACT)

**User confusion:**
- "Can I create aesthetic templates?" (YES, but hidden)
- "How do I reuse aesthetics across plugins?" (ui-template-library, but not documented in README)

**Impact:** Functional features not discoverable through main documentation

---

## Recommendations

### Immediate Actions (HIGH PRIORITY)

#### 1. Archive validator Agent

**Rationale:** 562 lines of unused specification create false expectations

**Steps:**
```bash
# Move to archive
mkdir -p .claude/agents/archive
mv .claude/agents/validator.md .claude/agents/archive/validator-deprecated.md

# Add deprecation notice
cat << 'EOF' >> .claude/agents/archive/validator-deprecated.md

---
## DEPRECATION NOTICE

**Date:** 2025-11-13
**Reason:** Validator agent made redundant by mature hook system

This agent was originally designed for semantic validation of stage completions.
During development, the hook system (PostToolUse.sh, SubagentStop.sh) matured to
handle 100% of validation needs through deterministic Python validators.

Hooks provide:
- Faster validation (no subagent overhead)
- Deterministic checking (pattern matching)
- Real-time feedback (during tool use, not after stage completion)

Validator agent was never invoked in production workflows and has been archived.

**Validation is now performed by:**
- PostToolUse.sh: Contract immutability, silent failure detection
- SubagentStop.sh: Stage-specific artifact validation, checksums, cross-contract consistency

EOF

# Remove schema
rm .claude/schemas/validator-report.json

# Update schema README
# (Remove validator-report.json section)
```

#### 2. Clean Up stage-6-validation.md

**Remove lines 149-186** (validator invocation example) or move to archive with deprecation notice

#### 3. Update README.md

**Line 145:** Remove validator from "Dispatcher Pattern" section or clarify it's deprecated

**Add section:** "Validation System" explaining hook-based approach (not agent-based)

---

### Medium Priority Actions

#### 4. Standardize build-automation Pattern

**Document decision:**
- **Skill delegation:** Complex builds requiring failure menus, context preservation, retry logic
- **Inline bash:** Simple rebuilds in middle of stage (Stage 6 rebuild)

**Update workflows to follow pattern consistently**

#### 5. Promote Aesthetic Skills

**Add to README.md:**
- aesthetic-dreaming: Create design systems before plugins
- ui-template-library: Reuse aesthetics across projects

**Make more discoverable in /dream menu** (currently Option 4)

---

### Low Priority Actions

#### 6. Rename Python Validators

**Current:** `validate-*.py` (ambiguous with "validator" agent)

**Proposed:** `check-*.py` or `verify-*.py`

**Impact:** Reduces terminology confusion

---

### Documentation Debt Cleanup

#### Files Requiring Updates

1. **CLAUDE.md** - Remove validator references or clarify deprecated
2. **README.md** - Add validation system explanation, promote aesthetic skills
3. **stage-6-validation.md** - Remove validator invocation pseudo-code
4. **.claude/schemas/README.md** - Remove validator-report.json section
5. **plugin-workflow/SKILL.md** - Ensure no validator references (appears clean)

---

## Metrics

**Total ghosts found:** 1 major (validator), 2 minor (aesthetic visibility)

**Unused code:** 562 lines (validator.md) + schema file

**Documentation debt:** 5 files require updates

**False capability claims:** 1 (semantic validation)

**Time to clean up:** ~2 hours

**Benefit:** Clearer system boundaries, reduced confusion, accurate documentation

---

## Conclusion

The Plugin Freedom System is remarkably clean with only **one significant ghost component** (validator agent). This is a testament to disciplined development—most systems accumulate far more ghosts.

**The validator ghost is particularly interesting because:**
- Extensively specified (562 lines)
- Fully documented with schemas
- Never implemented in actual workflows
- Made redundant by hook system evolution

**This suggests:**
1. Original architecture included semantic validation
2. Hook system proved sufficient during implementation
3. validator agent never removed despite being superseded
4. Documentation didn't catch up to implementation reality

**Primary recommendation:** Archive validator with clear deprecation notice explaining why hooks won. This converts ghost into valuable historical context.

**Secondary benefit:** Cleaning up this one major ghost will significantly improve system clarity and reduce maintainer confusion.

---

**End of Ghost Component Audit**
