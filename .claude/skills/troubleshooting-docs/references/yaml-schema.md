# YAML Frontmatter Schema

## Required Fields

- **plugin** (string): Plugin name exactly as in PLUGINS.md
- **date** (string): ISO 8601 date (YYYY-MM-DD)
- **symptom** (string): Brief symptom description
- **severity** (enum): One of [low, medium, high, critical]
- **tags** (array): One or more of [build, runtime, validation, webview, dsp, gui, parameters, cmake, juce-api]

## Validation Rules

1. All required fields must be present
2. date must match YYYY-MM-DD format
3. severity must be one of the allowed values
4. tags must be non-empty array
5. plugin must exist in PLUGINS.md (warning if not, not blocking)

## Example

```yaml
---
plugin: DelayPlugin
date: 2025-11-10
symptom: Missing juce_dsp module causing link errors
severity: medium
tags: [build, cmake]
---
```
