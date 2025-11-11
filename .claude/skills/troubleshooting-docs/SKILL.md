---
name: troubleshooting-docs
description: Capture problem solutions in searchable knowledge base
allowed-tools:
  - Read  # Parse conversation context
  - Write  # Create resolution docs
  - Bash  # Create directories
  - Grep  # Search existing docs
preconditions:
  - Problem has been solved (not in-progress)
  - Solution has been verified working
model: sonnet
---

# troubleshooting-docs Skill

**Purpose:** Automatically document solved problems to build searchable institutional knowledge organized by symptom category.

## Overview

This skill captures problem solutions immediately after confirmation, creating structured documentation that serves as a searchable knowledge base for future sessions. Documentation is organized by symptom category (build-failures, parameter-issues, etc.) with plugin name in the filename and YAML frontmatter.

---

## 7-Step Process

### Step 1: Detect Confirmation

**Auto-invoke after phrases:**
- "that worked"
- "it's fixed"
- "working now"
- "problem solved"
- "that did it"

**OR manual:** `/doc-fix` command

**Non-trivial problems only:**
- Multiple investigation attempts needed
- Tricky debugging that took time
- Non-obvious solution
- Future sessions would benefit

**Skip documentation for:**
- Simple typos
- Obvious syntax errors
- Trivial fixes immediately corrected

### Step 2: Gather Context

Extract from conversation history:

**Required information:**
- **Plugin name**: Which plugin had the problem
- **Symptom**: Observable error/behavior (exact error messages)
- **Investigation attempts**: What didn't work and why
- **Root cause**: Technical explanation of actual problem
- **Solution**: What fixed it (code/config changes)
- **Prevention**: How to avoid in future

**Environment details:**
- JUCE version
- Stage (0-6 or post-implementation)
- OS version
- File/line references

**Ask user if missing critical context:**
```
I need a few details to document this properly:

1. Which plugin had this issue? [PluginName]
2. What was the exact error message or symptom?
3. What stage were you in? (0-6 or post-implementation)

[Continue after user provides details]
```

### Step 3: Check Existing Docs

Search troubleshooting/ for similar issues:

```bash
# Search by error message keywords
grep -r "exact error phrase" troubleshooting/

# Search by symptom category
ls troubleshooting/[category]/
```

**If similar issue found:**

Present options:
```
Found similar issue: troubleshooting/build-failures/similar-issue-OtherPlugin-20251109.md

What's next?
1. Create new doc with cross-reference (recommended)
2. Update existing doc - Add this case as variant
3. Link as duplicate - Don't create new doc
4. Other
```

### Step 4: Generate Filename

Format: `[sanitized-symptom]-[plugin]-[YYYYMMDD].md`

**Sanitization rules:**
- Lowercase
- Replace spaces with hyphens
- Remove special characters except hyphens
- Truncate to reasonable length (< 80 chars)

**Examples:**
- `missing-juce-dsp-module-DelayPlugin-20251110.md`
- `parameter-not-saving-state-ReverbPlugin-20251110.md`
- `webview-crash-on-resize-TapeAgePlugin-20251110.md`

### Step 5: Validate YAML Schema

**CRITICAL:** All docs require validated YAML frontmatter.

**Load schema:**
Read `.claude/skills/troubleshooting-docs/references/yaml-schema.md` for validation rules.

**Required fields:**
- `plugin`: String (must exist in PLUGINS.md - warning if not)
- `date`: String (YYYY-MM-DD format)
- `symptom`: String (brief description)
- `severity`: Enum [low, medium, high, critical]
- `tags`: Array (one or more of: build, runtime, validation, webview, dsp, gui, parameters, cmake, juce-api)

**Validation process:**
```bash
# Check date format
if [[ ! "$DATE" =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ ]]; then
  echo "ERROR: date must be YYYY-MM-DD format"
  exit 1
fi

# Check severity
if [[ ! "$SEVERITY" =~ ^(low|medium|high|critical)$ ]]; then
  echo "ERROR: severity must be low, medium, high, or critical"
  exit 1
fi

# Check tags non-empty
if [[ ${#TAGS[@]} -eq 0 ]]; then
  echo "ERROR: tags must have at least one value"
  exit 1
fi
```

**BLOCK if validation fails:**
```
❌ YAML validation failed

Errors:
- severity: must be one of [low, medium, high, critical], got "moderate"
- tags: must be non-empty array

Please provide corrected values.
```

Present retry with corrected values, don't proceed until valid.

### Step 6: Create Documentation

**Determine category from tags:**

Auto-detect primary category:
- `build` → build-failures/
- `runtime` → runtime-issues/
- `validation` → validation-problems/
- `webview` → webview-issues/
- `dsp` → dsp-issues/
- `gui` → gui-issues/
- `parameters` → parameter-issues/
- `cmake` → build-failures/
- `juce-api` → api-usage/

**Create file in category directory:**
```bash
CATEGORY="[detected-category]"
FILENAME="[generated-filename].md"
FILE_PATH="troubleshooting/${CATEGORY}/${FILENAME}"

# Create directory if needed
mkdir -p "troubleshooting/${CATEGORY}"

# Write documentation
cat > "$FILE_PATH" << 'EOF'
[YAML frontmatter]
[Documentation content from template]
EOF
```

**Documentation template:**

Use template from `assets/resolution-template.md` with populated values:
- YAML frontmatter with validated fields (plugin name included)
- Problem title (descriptive)
- Symptom section (exact errors, observable behavior)
- Context section (plugin, stage, JUCE version, OS)
- Investigation steps tried (what didn't work and why)
- Root cause (technical explanation)
- Solution (step-by-step fix with code examples)
- Prevention (how to avoid)
- Related issues (cross-references if found in Step 3)
- References (JUCE docs, forum threads, external resources)

**Finding plugin-specific docs:**
```bash
# All docs for a specific plugin
grep -l "plugin: DelayPlugin" troubleshooting/**/*.md

# Or search by filename pattern
find troubleshooting/ -name "*-DelayPlugin-*.md"
```

### Step 7: Cross-Reference

If similar issues found in Step 3:

**Update existing doc:**
```bash
# Add Related Issues link to similar doc
echo "- See also: [$FILENAME]($FILE_PATH)" >> [similar-doc.md]
```

**Update new doc:**
Already includes cross-reference from Step 6.

**Update patterns if applicable:**

If this represents a common pattern (3+ similar issues):
```bash
# Add to troubleshooting/patterns/common-solutions.md
cat >> troubleshooting/patterns/common-solutions.md << 'EOF'

## [Pattern Name]

**Common symptom:** [Description]
**Root cause:** [Technical explanation]
**Solution pattern:** [General approach]

**Examples:**
- [Link to doc 1]
- [Link to doc 2]
- [Link to doc 3]
EOF
```

---

## Decision Menu After Capture

After successful documentation:

```
✓ Solution documented

File created: troubleshooting/[category]/[filename].md
Plugin: [PluginName]
Category: [category]

What's next?
1. Continue workflow (recommended)
2. Link related issues - Connect to similar problems
3. Update common patterns - Add to pattern library
4. View documentation - See what was captured
5. Other
```

**Handle responses:**

**Option 1: Continue workflow**
- Return to calling skill/workflow
- Documentation is complete

**Option 2: Link related issues**
- Prompt: "Which doc to link? (provide filename or describe)"
- Search troubleshooting/ for the doc
- Add cross-reference to both docs
- Confirm: "✓ Cross-reference added"

**Option 3: Update common patterns**
- Check if 3+ similar issues exist
- If yes: Add pattern to troubleshooting/patterns/common-solutions.md
- If no: "Need 3+ similar issues to establish pattern (currently N)"

**Option 4: View documentation**
- Display the created documentation
- Present decision menu again

**Option 5: Other**
- Ask what they'd like to do

---

## Integration Points

**Invoked by:**
- Auto-detection after success phrases
- `/doc-fix` command
- Any skill after solution confirmation
- Manual: "document this solution"

**Invokes:**
- None (terminal skill)

**Reads:**
- Conversation history (for context extraction)
- `PLUGINS.md` (validate plugin name)
- `troubleshooting/` (search existing docs)
- `assets/resolution-template.md` (documentation template)
- `references/yaml-schema.md` (validation rules)

**Creates:**
- `troubleshooting/[category]/[filename].md` (documentation file)
- Updates to `troubleshooting/patterns/common-solutions.md` (if pattern detected)

**Updates:**
- Existing docs (cross-references)
- Pattern library (if applicable)

---

## Success Criteria

Documentation is successful when:

- ✅ YAML frontmatter validated (all required fields, correct formats)
- ✅ File created in troubleshooting/[category]/[filename].md
- ✅ Documentation follows template structure
- ✅ All sections populated with relevant content
- ✅ Code examples included (if applicable)
- ✅ Cross-references added (if similar issues exist)
- ✅ File is searchable (descriptive filename, plugin in filename and YAML, tags)

---

## Error Handling

**Missing context:**
- Ask user for missing details
- Don't proceed until critical info provided

**YAML validation failure:**
- Show specific errors
- Present retry with corrected values
- BLOCK until valid

**Similar issue ambiguity:**
- Present multiple matches
- Let user choose: new doc, update existing, or link as duplicate

**Plugin not in PLUGINS.md:**
- Warn but don't block
- Proceed with documentation
- Suggest: "Add [Plugin] to PLUGINS.md if not there"

---

## Notes for Claude

**When executing this skill:**

1. Always validate YAML frontmatter - BLOCK if invalid
2. Use relative symlinks (../../by-plugin/...) not absolute paths
3. Extract exact error messages from conversation
4. Include code examples in solution section
5. Cross-reference similar issues automatically
6. Category detection is automatic from tags
7. Ask user if critical context missing
8. Be specific in documentation (exact file:line, versions)

**Common pitfalls:**

- Forgetting to create directories before writing files
- Missing YAML validation (creates invalid docs)
- Vague descriptions (not searchable)
- No code examples (harder to understand solution)
- No cross-references (knowledge stays siloed)
- Plugin name not in filename (harder to find plugin-specific issues)

---

## Quality Guidelines

**Good documentation has:**
- ✅ Exact error messages (copy-paste from output)
- ✅ Specific file:line references
- ✅ Observable symptoms (what you saw, not interpretations)
- ✅ Failed attempts documented (helps avoid wrong paths)
- ✅ Technical explanation (not just "what" but "why")
- ✅ Code examples (before/after if applicable)
- ✅ Prevention guidance (how to catch early)
- ✅ Cross-references (related issues)

**Avoid:**
- ❌ Vague descriptions ("something was wrong")
- ❌ Missing technical details ("fixed the code")
- ❌ No context (which version? which file?)
- ❌ Just code dumps (explain why it works)
- ❌ No prevention guidance
- ❌ No cross-references

---

## Example Scenario

**User:** "That worked! The parameter is saving correctly now."

**Skill activates:**

1. **Detect confirmation:** "That worked!" triggers auto-invoke
2. **Gather context:**
   - Plugin: ReverbPlugin
   - Symptom: DECAY parameter not persisting after DAW reload
   - Failed attempts: Checked APVTS creation, verified parameter ID
   - Solution: Added parameterValueChanged() to update processor state
   - Root cause: State only saved from APVTS, not reflecting UI changes
3. **Check existing:** No similar issue found
4. **Generate filename:** `parameter-not-saving-decay-ReverbPlugin-20251110.md`
5. **Validate YAML:**
   ```yaml
   plugin: ReverbPlugin
   date: 2025-11-10
   symptom: DECAY parameter not persisting after DAW reload
   severity: medium
   tags: [parameters, runtime]
   ```
   ✅ Valid
6. **Create documentation:**
   - File: `troubleshooting/parameter-issues/parameter-not-saving-decay-ReverbPlugin-20251110.md`
7. **Cross-reference:** None needed (no similar issues)

**Output:**
```
✓ Solution documented

File created: troubleshooting/parameter-issues/parameter-not-saving-decay-ReverbPlugin-20251110.md
Plugin: ReverbPlugin
Category: parameter-issues

What's next?
1. Continue workflow (recommended)
2. Link related issues
3. Update common patterns
4. View documentation
5. Other
```

---

## Future Enhancements

**Not in Phase 7 scope, but potential:**

- Search by date range
- Filter by severity
- Tag-based search interface
- Metrics (most common issues, resolution time)
- Export to shareable format (community knowledge sharing)
- Import community solutions
