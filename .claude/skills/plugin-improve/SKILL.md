---
name: plugin-improve
description: Fix bugs and add features with versioning, regression testing, and backups
allowed-tools:
  - Read
  - Write
  - Edit
  - Bash
  - Task # For deep-research (Tier 3) and plugin-testing (regression tests)
preconditions:
  - Plugin status must be ✅ Working OR 📦 Installed
  - Plugin must NOT be 🚧 In Development
---

# plugin-improve Skill

**Purpose:** Make changes to completed plugins with versioning, backups, changelog automation, and root cause investigation.

**Integration with deep-research:**

This skill can receive pre-computed research findings from the deep-research skill. When user runs `/research` to investigate a problem and then chooses "Apply solution", deep-research outputs a routing instruction that signals the main conversation to invoke this skill. Phase 0.45 detects the research findings in conversation history and skips investigation, proceeding directly to implementation approval.

**Workflow:**
1. User: `/research [problem]` → deep-research investigates
2. deep-research: Presents findings with "Apply solution" option
3. User: Selects "Apply solution"
4. deep-research: Outputs routing instruction: "Invoking plugin-improve skill..."
5. Main conversation: Sees routing instruction, invokes plugin-improve skill
6. plugin-improve: Detects research in history (Phase 0.45), extracts findings, skips investigation
7. plugin-improve: Implements with versioning, backups, testing

## Precondition Checking

**Before starting, verify:**

1. Read PLUGINS.md:

```bash
grep "^### $PLUGIN_NAME$" PLUGINS.md
```

2. Check status:
   - If status = ✅ Working or 📦 Installed → OK to proceed
   - If status = 🚧 In Development → BLOCK with message:
     ```
     [PluginName] is still in development (Stage [N]).
     Complete the workflow first with /continue [PluginName].
     Cannot use /improve on in-progress plugins.
     ```
   - If status = 💡 Ideated → BLOCK with message:
     ```
     [PluginName] is not implemented yet (Status: 💡 Ideated).
     Use /implement [PluginName] to build it first.
     ```
   - If not found → BLOCK with message:
     ```
     Plugin [PluginName] not found in PLUGINS.md.
     ```

## Phase 0: Specificity Detection

**Check if request is specific:**

Request IS specific if it has:

- Feature name (e.g., "resonance parameter", "bypass switch")
- Action (e.g., "add", "remove", "fix", "change from X to Y")
- Acceptance criteria (e.g., "range 0-1", "increase to 500px", "reduce by 3dB")

Request IS vague if lacking above:

- "improve the filters"
- "better presets"
- "UI feels cramped"
- "make it sound warmer"

**Assess specificity:**

- **Specific enough (1-2 clarification questions max):** Proceed to Phase 0.3 (4-question clarification batch)
- **Vague:** Present choice using AskUserQuestion:

```
Question:
  question: "Your request needs more detail. How should I proceed?"
  header: "Approach"
  options:
    - label: "Brainstorm approaches together", description: "I'll ask questions to explore options"
    - label: "Implement something reasonable", description: "I'll investigate and propose a solution"

Handle responses:
- Option 1 → Invoke plugin-ideation skill in improvement mode, then return here when ready
- Option 2 → Proceed to Phase 0.45 (Research Detection)
```

## Phase 0.3: Clarification Questions (For Specific Requests)

**If request is specific enough, ask 4 clarification questions using AskUserQuestion:**

**Question Priority Tiers:**

- **Tier 1 (Critical):** Current behavior, proposed solution, breaking changes
- **Tier 2 (Implementation):** Testing approach, backward compatibility, version impact
- **Tier 3 (Context):** Rationale, success metrics, alternative approaches

**Generate exactly 4 questions based on what's missing:**

```
Question 1:
  question: "Current behavior that needs changing?"
  header: "Problem"
  options:
    - label: "Describe the issue", description: "Explain what's wrong or limited"
    - label: "Show example", description: "Provide specific example"
    - label: "Already described", description: "Skip this question"
    - label: "Other", description: "Different approach"

Question 2:
  question: "Proposed solution?"
  header: "Fix"
  options:
    - label: "Add new feature", description: "Extend functionality"
    - label: "Modify existing", description: "Change current behavior"
    - label: "Remove/replace", description: "Take something out"
    - label: "Other", description: "Different solution"

Question 3:
  question: "Testing approach?"
  header: "Verification"
  options:
    - label: "Load test session", description: "Use existing project"
    - label: "A/B compare", description: "Before/after comparison"
    - label: "Measure performance", description: "CPU/memory metrics"
    - label: "Other", description: "Different testing method"

Question 4:
  question: "Breaking changes acceptable?"
  header: "Compatibility"
  options:
    - label: "Yes", description: "Can break existing presets/sessions"
    - label: "Must maintain compatibility", description: "No breaking changes"
    - label: "Only if worth it", description: "Evaluate trade-offs"
    - label: "Other", description: "Different constraint"
```

**After receiving answers:**

1. Merge with initial request
2. Proceed to decision gate

## Phase 0.4: Decision Gate (For Specific Requests)

**Use AskUserQuestion with 3 options after clarification questions:**

```
Question:
  question: "Ready to implement this improvement?"
  header: "Next step"
  options:
    - label: "Yes, implement it", description: "Proceed with implementation"
    - label: "Ask me 4 more questions", description: "Need more clarification"
    - label: "Let me add more context first", description: "Provide additional details"

Route based on answer:
- Option 1 → Proceed to Phase 0.45 (Research Detection)
- Option 2 → Return to Phase 0.3 (re-analyze gaps, generate next 4 questions)
- Option 3 → Collect free-form text, merge with context, return to Phase 0.3
```

## Phase 0.45: Research Detection

**BEFORE starting investigation, check conversation history for deep-research findings:**

**Scan recent messages for:**

- Routing instruction: "Invoking plugin-improve skill..." (signals deep-research handoff)
- Messages from deep-research skill
- Research reports (Level 1/2/3)
- Problem analysis and root cause
- Recommended solutions
- Implementation roadmap

**If deep-research findings found:**

```markdown
✓ Research already completed (deep-research Level N)

**Problem:** [Extracted from research]
**Root Cause:** [Extracted from research]
**Recommended Solution:** [Extracted from research]
**Implementation Steps:** [Extracted from research]

Skipping Phase 0.5 investigation (research already done).

Ready to implement? (y/n): _
```

Wait for user approval, then proceed to Phase 0.9 (Backup Verification).

**If NO research findings found:**

Proceed to Phase 0.5 (Investigation) - perform fresh root cause analysis.

**Why this matters:**

- Avoids duplicate investigation (user already ran /research)
- Preserves expensive research context (Opus + extended thinking)
- Maintains separation: research finds solutions, improve implements them
- Clear handoff: research outputs routing instruction → main conversation invokes improve

## Phase 0.5: Investigation (3-Tier)

**Purpose:** Find root causes, prevent band-aid fixes

**Tier Selection:**

- **Tier 1 (5-10 min):** Cosmetic changes, simple fixes, obvious issues
- **Tier 2 (15-30 min):** Logic errors, parameter issues, integration bugs
- **Tier 3 (30-60 min):** Complex bugs, performance issues, architectural problems

**Tier 1: Basic Code Inspection**

Read relevant source files:

- PluginProcessor.cpp/h
- PluginEditor.cpp/h
- Relevant JUCE modules

Check for:

- Obvious typos or errors
- Known pattern matches
- Simple logic issues

**Tier 2: Root Cause Analysis**

Deeper investigation:

- Trace logic flow from symptom to cause
- Check integration points between components
- Review parameter definitions and usage
- Examine state management
- Check threading issues (processBlock vs GUI)

**Tier 3: Deep Research**

Invoke `deep-research` skill for complex issues:

```
Complex issue detected. Invoking deep-research skill...
```

Use Skill tool to invoke deep-research:
- Provide problem context, plugin name, stage
- deep-research performs graduated investigation (Levels 1-3)
- Returns structured findings with recommendations
- Continue with Phase 0.5 "Present findings" using research output

**Present findings:**

```markdown
## Investigation Findings

### Problem Analysis

[What's actually wrong and why it's happening]

### Root Cause

[Technical explanation of the underlying issue]

### Affected Files

- plugins/[Name]/Source/[File]:[Line]
- plugins/[Name]/Source/[File]:[Line]

### Recommended Approach

[How to fix it properly - not a workaround]

### Alternative Approaches

[Other valid options with trade-offs explained]

### Backward Compatibility

[Will this break existing presets/sessions?]

Proceed with recommended approach? (y/n): \_
```

**Wait for approval before implementing.**

If user says no, ask which alternative or if they want different approach.

## Phase 0.9: Backup Verification

**Goal:** Ensure rollback is possible if improvement fails

**Check if backup exists:**

```bash
BACKUP_PATH="backups/${PLUGIN_NAME}/v${CURRENT_VERSION}/"
if [[ ! -d "$BACKUP_PATH" ]]; then
  echo "⚠️ No backup found for v${CURRENT_VERSION}"
  CREATE_BACKUP=true
fi
```

**Create backup if missing:**

```bash
mkdir -p "backups/${PLUGIN_NAME}/v${CURRENT_VERSION}/"
cp -r "plugins/${PLUGIN_NAME}/" "backups/${PLUGIN_NAME}/v${CURRENT_VERSION}/"
echo "✓ Backup created: backups/${PLUGIN_NAME}/v${CURRENT_VERSION}/"
```

**Verify backup integrity:**

```bash
# Use verify-backup.sh script
./scripts/verify-backup.sh "${PLUGIN_NAME}" "${CURRENT_VERSION}"
```

**Present verification results:**

```
✓ Backup verified: backups/[PluginName]/v[CurrentVersion]/

- All source files present
- CMakeLists.txt valid
- Dry-run build successful

Rollback available if needed.
```

## Phase 1: Pre-Implementation Checks

**Load current state:**

1. Read CHANGELOG.md:

```bash
cat plugins/[PluginName]/CHANGELOG.md
```

Extract current version (e.g., v1.2.3).

2. Read PLUGINS.md entry for additional context.

3. Check recent commits:

```bash
git log --oneline plugins/[PluginName]/ -10
```

**Determine version bump:**

Present choice:

```
Current version: v[X.Y.Z]

What type of change is this?
1. PATCH (v[X.Y.Z] → v[X.Y.Z+1]) - Bug fixes, cosmetic changes
2. MINOR (v[X.Y] → v[X.Y+1]) - New features, enhancements
3. MAJOR (v[X] → v[X+1]) - Breaking changes (presets won't load, parameters changed)

Choose (1-3): _
```

**If Major version selected, warn:**

```
⚠️ Major version bump will break compatibility.

Breaking changes include:
- Changed parameter IDs (presets won't load)
- Removed parameters (sessions will have missing automation)
- Changed state format (existing sessions corrupted)

Are you sure? This should be rare. (y/n): _
```

Calculate new version based on selection.

## Phase 2: Backup Creation

**Before ANY code changes:**

```bash
mkdir -p backups/
cp -r plugins/[PluginName] backups/[PluginName]-v[X.Y.Z]-$(date +%Y%m%d-%H%M%S)
```

Confirm backup created:

```
✓ Backup created: backups/[PluginName]-v[X.Y.Z]-[timestamp]

This backup can be restored if anything goes wrong.
```

## Phase 3: Implementation

**Make the approved changes:**

1. Modify source files according to investigation findings
2. Update build configuration if needed (CMakeLists.txt)
3. Adjust UI if required (PluginEditor.cpp)
4. Update parameter definitions if needed (PluginProcessor.cpp)

**Follow best practices:**

- Real-time safety in processBlock
- No allocations in audio thread
- Thread-safe parameter access
- JUCE API correctness

**Log changes as you go for CHANGELOG.**

## Phase 4: Enhanced CHANGELOG Update

**Update CHANGELOG.md with enhanced format:**

Add new version entry at top with technical details:

```markdown
# Changelog

All notable changes to [PluginName] will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [X.Y.Z] - [YYYY-MM-DD]

### Added

- [New feature 1]
  - [Detailed description]
  - [User benefit]

### Changed

- [Modified behavior 1]

### Fixed

- [Bug fix 1]
  - Root cause: [Technical explanation]
  - Impact: [What was affected]

### Technical

- Parameter changes:
  - Added: [PARAM_ID] (ID: "paramId", range: [min,max], default: value)
  - Modified: [PARAM_ID] (changed range from X to Y)
  - Removed: [PARAM_ID] (deprecated, use [NEW_PARAM] instead)
- DSP changes:
  - [Description of processBlock modifications]
  - [Algorithm improvements]
- UI changes:
  - [WebView or native UI updates]
  - [Layout modifications]
- Dependencies:
  - [New JUCE modules added]
  - [External dependencies]

### Testing

- Regression tests: [✅ X/Y passing | ⚠️ See notes below]
- Baseline version: v[X.Y.Z]
- New test coverage: [Description of new tests if added]

### Migration Notes (MAJOR versions only)

- v[X-1].x presets compatible: [Yes/No + explanation]
- Breaking changes:
  - [List parameter ID changes]
  - [List removed parameters]
  - [State format changes]
- Workarounds:
  - [How to adapt existing projects]
  - [Preset conversion steps if needed]

## [Previous Version] - [Date]

[Previous entries remain...]
```

**Sections to use:**

- **Added:** New features (with detailed descriptions)
- **Changed:** Changes to existing functionality
- **Fixed:** Bug fixes (with root cause explanations)
- **Removed:** Removed features
- **Technical:** Implementation details (parameters, DSP, UI, dependencies)
- **Testing:** Regression test results and coverage
- **Migration Notes:** Breaking changes and workarounds (MAJOR versions only)

**Change type auto-detection:**

- PATCH (v1.0.0 → v1.0.1): Use "Fixed" section primarily
- MINOR (v1.0.0 → v1.1.0): Use "Added" or "Changed" sections
- MAJOR (v1.0.0 → v2.0.0): Include "Migration Notes" section

**Enhanced changelog example:**

```markdown
## [1.1.0] - 2025-11-10

### Added

- Tempo sync parameter (TEMPO_SYNC)
  - Locks delay time to DAW tempo
  - Supports 1/16 to 4 bars
  - Automatic BPM detection via AudioPlayHead

### Changed

- Improved feedback algorithm for warmer saturation

### Technical

- Added parameter: TEMPO_SYNC (ID: "tempoSync", range: [0,1], default: 0)
- Added juce::AudioPlayHead integration in processBlock
- New WebView relay: tempoSyncRelay
- Modified DSP: DelayLine now supports tempo-locked timing

### Testing

- Regression tests: ✅ 5/5 passing (baseline: v1.0.0)
- Added test: Tempo sync accuracy (±1ms)

### Migration Notes

- v1.0 presets fully compatible (TEMPO_SYNC defaults to false, preserves behavior)
- No breaking changes
```

## Phase 5: Build & Test

**1. Build plugin:**

Invoke `build-automation` skill (full build with installation):

```
Invoking build-automation skill to build and install updated plugin...
```

build-automation will:

- Run build script: `scripts/build-and-install.sh [PluginName]` (full build)
- Build VST3 and AU formats in parallel
- Install to system folders
- Clear DAW caches
- Verify installation

If build succeeds:

- build-automation displays success message with installation paths
- Returns control to plugin-improve
- Proceed to Phase 5, step 2 (Run tests)

If build fails:

- build-automation presents 4-option failure protocol:
  1. Investigate (troubleshooter agent)
  2. Show build log
  3. Show code
  4. Wait for manual fix
- After resolution and successful retry, returns to plugin-improve
- Proceed to Phase 5, step 2 (Run tests)

**Note:** Build failure handling is entirely managed by build-automation skill. plugin-improve does not need custom build error menus.

**2. Run tests:**

Invoke `plugin-testing` skill (Phase 1b Task 8):

Present test method choice:

```
Build successful. How would you like to test?

1. Automated stability tests (if Tests/ exists)
2. Build and run pluginval (recommended)
3. Manual DAW testing checklist
4. Skip testing (not recommended)

Choose (1-4): _
```

If tests fail, present investigation options.

## Phase 5.5: Regression Testing

**Check:** Does `.claude/skills/plugin-testing/SKILL.md` exist?

**If NO:** Skip to Phase 6 (add warning to changelog: "Manual regression testing required")

**If YES:** Run regression tests

### Regression Test Process

**1. Determine baseline version:**

- If improving v1.0.0 → v1.1.0, baseline is v1.0.0
- Check if backup exists: `backups/[Plugin]/v[baseline]/`
- If no backup: Skip regression tests (warn user)

**2. Build baseline version:**

```bash
# Temporarily checkout baseline
cd backups/[Plugin]/v[baseline]/
../../scripts/build-and-install.sh --no-install
```

**3. Run tests on baseline:**

- Invoke plugin-testing skill on baseline build
- Capture results: BASELINE_RESULTS

**4. Run tests on current version:**

- Invoke plugin-testing skill on new build
- Capture results: CURRENT_RESULTS

**5. Compare results:**

```typescript
interface RegressionReport {
  baseline_version: string;
  current_version: string;
  tests_run: number;
  baseline_passing: number;
  current_passing: number;
  new_failures: TestCase[]; // Regressions!
  new_passes: TestCase[]; // Fixed bugs
  unchanged: number;
}
```

**6. Present findings:**

#### If No Regressions:

```
✓ Regression tests passed (5/5 tests, no new failures)

What's next?
1. Continue to git workflow (recommended)
2. Review test details
3. Other
```

#### If Regressions Detected:

```
⚠️ Regression detected - new failures found

**Baseline (v1.0.0):** 5/5 tests passing
**Current (v1.1.0):** 3/5 tests passing

**New failures:**
1. Parameter Response Test - GAIN parameter no longer affects audio
2. CPU Performance Test - Real-time factor increased from 0.05 to 0.15

**Recommendation:** Fix regressions before proceeding

What's next?
1. Investigate failures - Debug issues (recommended)
2. View test output - See detailed logs
3. Continue anyway - Accept regressions (not recommended)
4. Rollback changes - Revert to v1.0.0
5. Other
```

### Rollback Mechanism

If user chooses "Rollback changes":

1. Verify backup exists: `backups/[Plugin]/v[baseline]/`
2. Copy all files from backup to `plugins/[Plugin]/`
3. Rebuild and reinstall
4. Update PLUGINS.md status
5. Git reset if commits were made
6. Confirm rollback success

```bash
# Rollback script
rm -rf plugins/[Plugin]
cp -r backups/[Plugin]/v[baseline]/ plugins/[Plugin]/
cd plugins/[Plugin]
../../scripts/build-and-install.sh
```

## Phase 6: Git Workflow

**Stage changes:**

```bash
git add plugins/[PluginName]/
git add backups/[PluginName]-v[X.Y.Z]-[timestamp]/  # Include backup in git
```

**Commit with conventional format:**

```bash
# Format: improve: [PluginName] v[X.Y.Z] - [brief description]
# Example: improve: MicroGlitch v1.3.0 - add preset system

git commit -m "improve: [PluginName] v[X.Y.Z] - [description]"
```

**Tag release:**

```bash
git tag -a "v[X.Y.Z]" -m "[PluginName] v[X.Y.Z]"
```

Note: User handles actual git operations, we stage only.

**Confirm git ready:**

```
✓ Changes staged for commit
✓ Tag ready: v[X.Y.Z]

Git commit message:
  improve: [PluginName] v[X.Y.Z] - [description]

You can commit these changes when ready.
```

## Phase 7: Installation

**Present decision:**

```
Build and tests successful. Install to system folders?

1. Yes, install now (recommended for 📦 Installed plugins)
2. No, test from build folder first
3. Skip installation

Choose (1-3): _
```

**If user chooses 1:**

Invoke `plugin-lifecycle` skill (Phase 1b Task 9):

```
Installing [PluginName] v[X.Y.Z]...
```

**Update PLUGINS.md:**

Update version number:

```markdown
**Version:** [X.Y.Z]
**Last Updated:** [YYYY-MM-DD]
```

If status was ✅ Working and now installed, change to 📦 Installed.

## Phase 8: Completion

**Present decision menu:**

```
✓ [PluginName] v[X.Y.Z] complete

What's next?
1. Test in DAW (recommended)
2. Make another improvement
3. Create new plugin
4. Document this change
5. Other

Choose (1-5): _
```

**Handle responses:**

- Option 1 → Provide manual testing guidance
- Option 2 → Ask what to improve, restart workflow
- Option 3 → Suggest `/dream` or `/implement`
- Option 4 → Suggest creating documentation
- Option 5 → Ask what they'd like to do

## Breaking Change Detection

**Automatically detect breaking changes:**

If implementation modifies:

- Parameter IDs
- Parameter count (removed parameters)
- State format (APVTS structure)
- Audio routing (channel count)

Warn:

```
⚠️ Breaking change detected

This change will cause:
- Existing presets may not load correctly
- Session automation may break
- Users need to recreate presets

This requires a MAJOR version bump (v[X].0.0).

Proceed with breaking change? (y/n): _
```

If user confirms, force MAJOR version bump.

## Rollback Support

**If anything goes wrong:**

Provide rollback instructions:

```
To restore backup:

1. Remove modified version:
   rm -rf plugins/[PluginName]

2. Restore backup:
   cp -r backups/[PluginName]-v[X.Y.Z]-[timestamp] plugins/[PluginName]

3. Rebuild:
   cmake --build build

Backup location: backups/[PluginName]-v[X.Y.Z]-[timestamp]
```

## Version Bump Logic

**PATCH (v1.2.3 → v1.2.4):**

- Bug fixes
- Performance improvements
- Cosmetic UI changes
- Documentation updates
- No new features
- No breaking changes

**MINOR (v1.2 → v1.3):**

- New features added
- New parameters added
- UI enhancements
- Backward compatible
- Existing presets still work

**MAJOR (v1 → v2):**

- Breaking changes
- Parameter IDs changed
- Parameters removed
- State format changed
- Requires preset migration
- Should be rare

## Phase 7 Enhancements

This skill has been enhanced in Phase 7 with:

- **Regression testing** (Phase 5.5): Automatically detects breaking changes by comparing baseline version tests to current version
- **Enhanced changelogs** (Phase 4): Technical details section includes parameter changes, DSP modifications, UI updates, dependencies. Migration notes for MAJOR versions.
- **Backup verification** (Phase 0.9): Ensures rollback is possible before making any changes. Uses `verify-backup.sh` script.
- **Rollback mechanism**: One-command restore to previous version if regressions detected

**Benefits:**

- Catch breaking changes automatically before deployment
- Document technical implementation details for future reference
- Safe iteration (always have verified backup)
- Quick rollback if improvement introduces problems

**See:**

- `architecture/17-testing-strategy.md` - Regression test specifications
- `scripts/verify-backup.sh` - Backup integrity verification script

## Integration Points

**Invoked by:**

- `/improve` command
- Natural language: "Fix [plugin]", "Add [feature] to [plugin]"
- `plugin-ideation` skill (after improvement brief)

**Invokes:**

- `plugin-ideation` skill (if vague request, user chooses brainstorm)
- `deep-research` skill (Tier 3 investigation) - Phase 7
- `build-automation` skill (building) - Phase 4
- `plugin-testing` skill (validation)
- `plugin-lifecycle` skill (installation)

**Updates:**

- CHANGELOG.md (adds version entry)
- PLUGINS.md (version number, last updated)
- Source files (implementation changes)

**Creates:**

- Backup in `backups/[PluginName]-v[X.Y.Z]-[timestamp]/`
- Git tag `v[X.Y.Z]`

## Error Handling

**Build failure:**
Present investigation menu, wait for user decision.

**Test failure:**
Present investigation menu, don't proceed to installation.

**Breaking change detected:**
Warn user, require confirmation, force MAJOR version.

**Backup creation fails:**
STOP immediately, don't proceed with changes.

**Git operations fail:**
Log warning, continue (non-critical).

## Success Criteria

Improvement is successful when:

- Root cause investigated (not band-aid fix)
- Backup created before changes
- Changes implemented correctly
- CHANGELOG updated with version entry
- Build succeeds without errors
- Tests pass
- Git staged with conventional commit message
- PLUGINS.md updated
- User knows how to rollback if needed
