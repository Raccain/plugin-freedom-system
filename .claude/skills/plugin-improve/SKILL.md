---
name: plugin-improve
description: Fix bugs, add features to completed plugins. Includes versioning, backups, regression testing, changelog automation. Trigger terms - improve, fix, add feature, modify plugin, version bump, rollback
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

<handoff_protocol>
**Trigger:** deep-research invokes plugin-improve via Skill tool
**Detection:** Phase 0.45 scans conversation history for research findings (MANDATORY)
**Action:** Extract research findings, skip investigation (Phase 0.5)
**Benefits:** Preserve expensive research context (Opus + extended thinking)
</handoff_protocol>

Detection mechanism is implemented in Phase 0.45 below. See `references/handoff-protocols.md` for additional workflow documentation.

<gate_preconditions enforcement="strict">
## Precondition Checking

**MUST execute before any other phase. BLOCK if conditions not met.**

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

</gate_preconditions>

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
- **Vague:** Present inline decision menu:

```
Your request needs more detail. How should I proceed?

1. Brainstorm approaches together - I'll ask questions to explore options
2. Implement something reasonable - I'll investigate and propose a solution
3. Other

Choose (1-3): _
```

**Handle responses:**
- Option 1 → Invoke plugin-ideation skill in improvement mode
- Option 2 → Proceed to Phase 0.45 (Research Detection)
- Option 3 → Collect free-form text, reassess

## Phase 0.3: Clarification Questions (If Specific)

**If request is specific enough, ask targeted questions using inline menus:**

**Question 1 - What to change:**
```
What needs to change?

1. Fix a bug - Something is broken or behaving incorrectly
2. Add a feature - New capability or enhancement
3. Improve existing behavior - Refine how something works
4. Other

Choose (1-4): _
```

**Question 2 - Scope:**
```
How extensive is this change?

1. Single function/method - Localized change in one place
2. Multiple related components - Changes across a few files
3. System-wide change - Affects architecture or many components
4. Other

Choose (1-4): _
```

**Question 3 - Priority:**
```
Version bump priority?

1. PATCH (bug fix) - Backward compatible fix, increment 0.0.X
2. MINOR (feature) - New feature, backward compatible, increment 0.X.0
3. MAJOR (breaking) - Breaking change, incompatible API, increment X.0.0
4. Other

Choose (1-4): _
```

**Question 4 - Testing:**
```
Should I run regression tests?

1. Yes, full regression suite - Compare new behavior vs baseline backup
2. Yes, but manual review if failures - Show me what changed and let me decide
3. No, skip regression tests - Just build and verify compilation
4. Other

Choose (1-4): _
```

**Collect all responses before proceeding to Phase 0.4**

## Phase 0.4: Decision Gate

**Show user what you understand, ask for confirmation:**

```
I understand you want to:
- [Summary of change from Question 1]
- Scope: [Answer from Question 2]
- Version bump: [Answer from Question 3]
- Regression testing: [Answer from Question 4]

Is this correct?

1. Yes, proceed - Continue to Phase 0.45 (Research Detection)
2. No, refine - Ask me follow-up questions
3. No, cancel - Stop the workflow
4. Other

Choose (1-4): _
```

**Handle responses:**
- Option 1 → Proceed to Phase 0.45
- Option 2 → Return to Phase 0.3, ask follow-up questions
- Option 3 → Stop workflow, wait for new instruction
- Option 4 → Collect free-form text, reassess

<research_detection id="phase-0.45" enforcement_level="MANDATORY">
## Phase 0.45: Research Detection

**Purpose:** Detect if deep-research already investigated this issue and extract findings to avoid duplicate work.

**Why critical:** Deep-research Tier 3 uses Opus with extended thinking (expensive, minutes per query). If findings already exist in conversation history, reusing them saves time and cost.

**Trigger:** After Phase 0.3 or Phase 0.4 completes, BEFORE Phase 0.5 investigation

**Detection Algorithm:**

<scan_conversation_history>
Look for these markers in conversation history (starting from most recent):

1. **deep-research skill invocation:**
   - Search for: "deep-research" in previous messages
   - Look for tier/level indicators: "Tier 1", "Tier 2", "Tier 3", "Level 1", "Level 2", "Level 3"
   - Check for completion markers: "Research complete", "Investigation findings", "Investigation Summary"

2. **Research output sections:**
   - "Root Cause Analysis"
   - "Root Cause:"
   - "Solutions Identified"
   - "Recommended Solution"
   - "Recommendations"
   - "Investigation Summary"
   - "Implementation Steps"
   - "Implementation Roadmap"

3. **Handoff signals:**
   - "Invoking plugin-improve with findings"
   - "Invoking plugin-improve skill"
   - "Handing off to plugin-improve skill"
   - "[deep-research → plugin-improve] handoff"
   - "Ready to implement?"

4. **Context clues:**
   - User mentioned "after researching"
   - User said "based on the investigation"
   - User references prior troubleshooting
   - User said "from the research findings"
</scan_conversation_history>

**Extraction Logic (if markers found):**

When research markers detected in conversation history, extract:

- **Research Tier/Level:** Which investigation depth was used (1/2/3)
- **Problem Statement:** What issue was being investigated
- **Root Cause:** Technical explanation of underlying issue
- **Recommended Solution:** Primary approach suggested by research
- **Alternative Solutions:** Other valid approaches with trade-offs
- **Implementation Steps:** Ordered tasks to apply the solution
- **Affected Files:** Which source files need modification
- **Testing Notes:** How to verify the fix works

**Decision Logic:**

<if_research_detected>
When research findings detected in conversation history:

1. **Display findings summary:**
   ```
   ✓ Research handoff detected from deep-research skill

   Investigation: Tier/Level ${tier} (${tierDescription})
   Problem: ${problemStatement}
   Root Cause: ${rootCause}
   Recommended Solution: ${recommendedSolution}

   Using existing research findings (skipping Phase 0.5 investigation).
   ```

2. **Skip Phase 0.5:**
   - Do NOT run investigation logic
   - Do NOT invoke deep-research again
   - Proceed directly to Phase 0.9 (Backup Verification) with findings

3. **Set context for implementation:**
   - RESEARCH_SOURCE = "deep-research handoff"
   - ROOT_CAUSE = extracted root cause
   - PROPOSED_SOLUTION = extracted recommended solution
   - IMPLEMENTATION_STEPS = extracted steps
   - Use these in improvement planning and CHANGELOG

4. **Present implementation approval:**
   ```
   Ready to implement this solution?

   1. Yes, proceed with recommended solution
   2. No, use alternative approach - Show me alternatives
   3. No, investigate further - Run fresh investigation (Phase 0.5)
   4. Other

   Choose (1-4): _
   ```

   **Handle responses:**
   - Option 1 → Proceed to Phase 0.9 (Backup Verification)
   - Option 2 → Show alternative solutions from research, ask which to use
   - Option 3 → Proceed to Phase 0.5 (Investigation) despite research existing
   - Option 4 → Collect free-form text, reassess
</if_research_detected>

<if_no_research_detected>
When NO research findings detected in conversation history:

1. **Log detection completion:**
   ```
   No research handoff detected in conversation history.
   Proceeding to Phase 0.5 (Investigation).
   ```

2. **Continue to Phase 0.5:**
   - Run normal investigation logic (auto-tiered)
   - May invoke deep-research if Tier 3 detected (user informed)

3. **Detection was performed:**
   - Important: Even if nothing found, detection ran (MANDATORY)
   - This prevents false negatives from lazy evaluation
   - Phase 0.45 always executes, never skipped
</if_no_research_detected>

**State Tracking:**

Document whether research was detected for audit trail:

```bash
# Create/append to improvement context file
echo "research_detection_performed: true" >> .improve-context.yaml
echo "research_handoff_detected: ${FINDINGS_DETECTED}" >> .improve-context.yaml
echo "research_tier: ${TIER:-none}" >> .improve-context.yaml
echo "detection_timestamp: $(date -u +%Y-%m-%dT%H:%M:%SZ)" >> .improve-context.yaml
```

**Anti-pattern:**

<anti_pattern severity="CRITICAL">
❌ NEVER skip Phase 0.45 research detection
❌ NEVER assume no research exists without scanning conversation history
❌ NEVER re-run deep-research if findings already in conversation
❌ NEVER ignore handoff signals from deep-research skill
✓ ALWAYS scan conversation history in Phase 0.45
✓ ALWAYS extract findings if markers detected
✓ ALWAYS skip Phase 0.5 if research already complete (unless user overrides)
✓ ALWAYS document detection results in .improve-context.yaml
</anti_pattern>

**Integration with existing phase flow:**

```
Phase 0 (Specificity) → Phase 0.3 (Clarification) → Phase 0.4 (Decision Gate)
                                                            ↓
                                            Phase 0.45 (Research Detection) ← YOU ARE HERE
                                                            ↓
                                      [Research detected?] decision point
                                            ↙                    ↘
                        YES: Skip to Phase 0.9              NO: Continue to Phase 0.5
                        (Backup Verification)                (Investigation)
```

**Why this matters:**

- Avoids duplicate investigation (user already ran /research or deep-research auto-invoked)
- Preserves expensive research context (Opus + extended thinking costs real time and money)
- Maintains separation of concerns: research finds solutions, improve implements them
- Prevents "re-investigate what we just investigated" anti-pattern
- Clear handoff: research outputs findings → improve extracts and uses them

</research_detection>

## Phase 0.5: Investigation (Auto-Tiered)

**Purpose:** Find root causes, prevent band-aid fixes

**Automatic Tier Detection:**

Analyze the request and automatically select investigation tier based on complexity indicators. Never ask the user which tier to use.

**Detection algorithm:**

```
Analysis:
1. Check troubleshooting/ for known pattern → Tier 1
2. Keywords: "cosmetic", "typo", "rename", "color", "text" → Tier 1
3. Keywords: "crash", "performance", "architecture", "all plugins" → Tier 3
4. Scope: Single file + clear symptom → Tier 2
5. Scope: Multiple components OR unclear cause → Tier 3
6. Default: Start Tier 1, escalate if needed
```

Log selected tier for transparency: "Analyzing issue (quick investigation)..." or "Analyzing issue (deep investigation)..."

**Tier 1 (5-10 min):** File read + pattern match
- Cosmetic changes, simple fixes, obvious issues
- Known patterns from troubleshooting/
- **Indicators:** Simple wording, single-file scope, UI-only changes

**Tier 2 (15-30 min):** Logic trace + integration check
- Logic errors, parameter issues, integration bugs
- Single component, requires code analysis
- **Indicators:** Specific component mentioned, functional bug, parameter issue

**Tier 3 (30-60 min):** Invoke deep-research skill (uses Opus)
- Complex bugs, performance issues, architectural problems
- Multi-component, requires deep investigation
- **Indicators:** "crash", "all plugins", performance, architecture, unclear cause

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

<critical_sequence phase="backup-verification" enforcement="strict">
## Phase 0.9: Backup Verification

**CRITICAL INVARIANT:** Phase 1 MUST NOT execute until backup verified.
**ENFORCEMENT:** Block execution, halt workflow if backup fails.
**VIOLATION CONSEQUENCE:** Data loss, no rollback path.

**Goal:** Ensure rollback is possible if improvement fails

**Check if backup exists:**

```bash
BACKUP_PATH="backups/${PLUGIN_NAME}/v${CURRENT_VERSION}/"
if [[ ! -d "$BACKUP_PATH" ]]; then
  echo "⚠️ No backup found for v${CURRENT_VERSION}"
  CREATE_BACKUP=true
fi
```

**Create backup if missing using template:**

```bash
# See assets/backup-template.sh for complete script
mkdir -p "backups/${PLUGIN_NAME}/v${CURRENT_VERSION}/"
rsync -a --exclude='build/' --exclude='build.log' \
  "plugins/${PLUGIN_NAME}/" "backups/${PLUGIN_NAME}/v${CURRENT_VERSION}/"
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

</critical_sequence>

<phase id="1" name="pre-implementation" dependencies="backup-verification">
## Phase 1: Pre-Implementation Checks

**DEPENDENCY:** MUST NOT execute until Phase 0.9 (Backup Verification) completes.

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

</phase>

<critical_sequence phase="backup-creation" enforcement="strict">
## Phase 2: Verify Rollback Path

**Baseline backup verified in Phase 0.9. Confirm ready to proceed:**

```
✓ Backup verified: backups/[PluginName]/v[CurrentVersion]/
Ready to implement changes for v[NewVersion]
```

</critical_sequence>

<phase id="3" name="implementation" dependencies="backup-creation">
## Phase 3: Implementation

**DEPENDENCY:** MUST NOT execute until Phase 2 (Backup Creation) completes.
**SAFETY:** If implementation fails, rollback path guaranteed by Phase 2 backup.

**Execute the change:**

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

</phase>

## Phase 4: Enhanced CHANGELOG Update

**Update CHANGELOG.md with enhanced format:**

See `references/changelog-format.md` for complete template structure and section usage guide.

**Quick reference:**
- **PATCH (0.0.X):** Use "Fixed" section primarily
- **MINOR (0.X.0):** Use "Added"/"Changed" sections
- **MAJOR (X.0.0):** Include "Breaking Changes" and "Migration Notes" sections

**Always include:**
- Date in ISO format (YYYY-MM-DD)
- Technical details (root cause, solution, affected components)
- User impact (what changes in behavior)
- Testing notes (regression test results if available)

<delegation_rule target="build-automation" required="true">
## Phase 5: Build and Test

**DELEGATION:** MUST invoke build-automation skill for all build operations.
**REASON:** Centralized build logic, 7-phase pipeline with verification.

**Delegate to build-automation skill:**

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

</delegation_rule>

<validation_gate gate="regression-tests" required="conditional">
## Phase 5.5: Regression Testing

**GATE CONDITION:** If plugin-testing skill exists AND baseline backup available
**GATE FAILURE:** Present rollback options, require user decision

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

**If plugin-testing skill exists and baseline backup available:**

See `references/regression-testing.md` for complete RegressionReport interface and protocol.

**Quick summary:**
1. Invoke plugin-testing skill with baseline and new versions
2. Collect RegressionReport (build, load, parameter, audio tests)
3. Analyze failures: critical → rollback, warnings → review, pass → deploy
4. Present results with decision menu

**If regression tests fail, present rollback options before proceeding.**

</validation_gate>

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

<delegation_rule target="plugin-lifecycle" required="false">
## Phase 7: Installation (Optional)

**DELEGATION:** If user requested installation, invoke plugin-lifecycle skill.
**REASON:** Centralized installation logic with cache clearing and verification.

**If user requested installation:**

Invoke `plugin-lifecycle` skill:

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

</delegation_rule>

<checkpoint_protocol>
## Phase 8: Completion

**MUST present numbered decision menu using inline format (NOT AskUserQuestion tool)**

**Present numbered decision menu (inline format):**

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

</checkpoint_protocol>

## Breaking Change Detection

**Check for breaking changes:**

See `references/breaking-changes.md` for complete detection criteria.

**Quick check:**
- Parameter IDs changed? → Breaking
- Parameter ranges changed? → Breaking
- State format changed? → Breaking
- Features removed? → Breaking
- API signatures changed? → Breaking

If breaking changes detected, warn user and require MAJOR version bump.

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

**Determine version bump using decision tree:**

See `references/versioning.md` for complete versioning logic.

**Quick reference:**
- Breaking change detected? → MAJOR (X.0.0)
- New feature added? → MINOR (0.X.0)
- Bug fix only? → PATCH (0.0.X)

If unsure, prefer MINOR over MAJOR (conservative approach).

## Version History

**Phase 7 enhancements (2025-11):**
- Regression testing integration (Phase 5.5)
- Enhanced changelog format (Phase 4)
- Backup verification protocol (Phase 0.9)
- One-command rollback mechanism
- Breaking change detection

**See:** `architecture/17-testing-strategy.md`, `scripts/verify-backup.sh`, `references/regression-testing.md`

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
