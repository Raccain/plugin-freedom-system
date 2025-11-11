---
name: doc-fix
description: Document a recently solved problem for the knowledge base
---

# /doc-fix

Invoke the troubleshooting-docs skill to document a recently solved problem.

## Purpose

Captures problem solutions while context is fresh, creating structured documentation organized by symptom category. Plugin name included in filename and YAML frontmatter for easy filtering.

## Usage

```bash
/doc-fix                    # Document the most recent fix
/doc-fix [brief context]    # Provide additional context hint
```

## What It Captures

- **Problem symptom**: Exact error messages, observable behavior
- **Investigation steps tried**: What didn't work and why
- **Root cause analysis**: Technical explanation
- **Working solution**: Step-by-step fix with code examples
- **Prevention strategies**: How to avoid in future
- **Cross-references**: Links to related issues

## Preconditions

- Problem has been solved (not in-progress)
- Solution has been verified working
- Non-trivial problem (not simple typo or obvious error)

## What It Creates

**Documentation file:** `troubleshooting/[category]/[symptom]-[Plugin]-[YYYYMMDD].md`

**Categories auto-detected from problem:**
- build-failures/
- runtime-issues/
- validation-problems/
- webview-issues/
- dsp-issues/
- gui-issues/
- parameter-issues/
- api-usage/

## Success Output

```
✓ Solution documented

File created: troubleshooting/parameter-issues/[symptom]-[Plugin]-20251110.md
Plugin: [PluginName]
Category: parameter-issues

This documentation will be searched by deep-research skill as Level 1 (Fast Path)
when similar issues occur.

What's next?
1. Continue workflow (recommended)
2. Link related issues
3. Update common patterns
4. View documentation
5. Other
```

## Why This Matters

**Builds knowledge base:**
- Future sessions find solutions faster
- deep-research searches local docs first (Level 1 Fast Path)
- No solving same problem repeatedly
- Institutional knowledge compounds over time

**The feedback loop:**
1. Hit problem → deep-research searches local troubleshooting/
2. Fix problem → troubleshooting-docs creates documentation
3. Hit similar problem → Found instantly in Level 1
4. Knowledge grows organically

## Auto-Invoke

Skill automatically activates after phrases:
- "that worked"
- "it's fixed"
- "working now"
- "problem solved"

Manual invocation via `/doc-fix` when you want to document without waiting for auto-detection.

## Routes To

`troubleshooting-docs` skill

## Related Commands

- `/research [topic]` - Deep investigation (searches troubleshooting/ docs at Level 1)
- `/improve [Plugin]` - Enhancement workflow (may trigger documentation after fixes)
