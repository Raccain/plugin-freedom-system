# Claude Code Plugin Packaging Guide

This guide explains how to package the Plugin Freedom System as a Claude Code plugin for distribution.

## Terminology Clarification

**Two meanings of "plugin" in this project:**
- **Claude Code Plugin** = Extension package (skills/commands/agents) for Claude Code
- **JUCE Plugin** = VST3/AU/AAX audio device created by this system

This guide covers packaging the **Claude Code plugin** (the development tooling itself).

---

## Current Structure vs. Plugin Structure

### What You Have Now
```
plugin-freedom-system/
├── .claude/
│   ├── commands/          # Slash commands (/implement, /dream, etc.)
│   ├── skills/            # Workflows (plugin-workflow, ui-mockup, etc.)
│   ├── agents/            # Subagents (foundation-agent, dsp-agent, etc.)
│   └── hooks/             # Validation gates
├── plugins/               # Where JUCE plugins get built
├── scripts/               # Build automation
└── troubleshooting/       # Knowledge base
```

### What's Needed for Distribution
```
plugin-freedom-system/
├── .claude-plugin/
│   ├── plugin.json        # NEW: Required metadata
│   └── marketplace.json   # NEW: Optional, for marketplace distribution
├── commands/              # NEW: Copy from .claude/commands/
├── skills/                # NEW: Copy from .claude/skills/
├── agents/                # NEW: Copy from .claude/agents/
├── hooks/                 # NEW: Copy from .claude/hooks/
├── .claude/               # Keep for local development
├── plugins/               # Keep (JUCE plugin output)
├── scripts/               # Keep (build automation)
└── troubleshooting/       # Keep (knowledge base)
```

---

## Step-by-Step Packaging

### Step 1: Create Plugin Metadata

Create `.claude-plugin/plugin.json`:

```json
{
  "name": "plugin-freedom-system",
  "description": "Complete JUCE audio plugin development workflow with 7-stage pipeline, WebView UI system, and automated testing",
  "version": "1.0.0",
  "author": "Lex Christopherson",
  "homepage": "https://github.com/lexchristopherson/plugin-freedom-system",
  "license": "MIT"
}
```

**Fields explained:**
- `name`: Unique identifier (lowercase, hyphens only)
- `description`: What the plugin does (shown in marketplace)
- `version`: Semantic versioning (MAJOR.MINOR.PATCH)
- `author`: Your name or organization
- `homepage`: Documentation/repo URL (optional)
- `license`: License type (optional)

### Step 2: Organize Plugin Components

Create the top-level directories and copy your existing components:

```bash
# Create plugin directories
mkdir -p .claude-plugin commands skills agents hooks

# Copy components to plugin structure
cp -r .claude/commands/* commands/
cp -r .claude/skills/* skills/
cp -r .claude/agents/* agents/
cp -r .claude/hooks/* hooks/
```

**Why duplicate?**
- `.claude/` = Local development (your active workspace)
- Top-level `commands/`, `skills/`, etc. = Distribution package

### Step 3: Create Marketplace Manifest (Optional)

If you want to distribute via a marketplace, create `.claude-plugin/marketplace.json`:

```json
{
  "plugins": [
    {
      "name": "plugin-freedom-system",
      "path": ".",
      "description": "Complete JUCE audio plugin development workflow",
      "version": "1.0.0",
      "tags": ["audio", "juce", "plugin-development", "workflow", "testing"]
    }
  ]
}
```

**When to use:**
- Distributing multiple plugins from one repo
- Want users to discover via `/plugin marketplace add`
- Hosting on GitHub/GitLab for public access

### Step 4: Add Documentation

Create a `README.md` at the root explaining:

```markdown
# Plugin Freedom System

Complete JUCE audio plugin development workflow for Claude Code.

## Features
- 7-stage development pipeline (/implement)
- Interactive ideation (/dream)
- WebView UI mockup system
- Automated testing (/test)
- Build automation with regression testing
- Knowledge base for troubleshooting

## Installation

### From GitHub
```bash
/plugin marketplace add lexchristopherson/plugin-freedom-system
/plugin install plugin-freedom-system
```

### Local Development
```bash
/plugin marketplace add /path/to/plugin-freedom-system
/plugin install plugin-freedom-system
```

## Quick Start

1. Create a new plugin idea:
   ```
   /dream
   ```

2. Implement the plugin:
   ```
   /implement PluginName
   ```

3. Test and install:
   ```
   /test PluginName
   /install-plugin PluginName
   ```

## Available Commands

See `commands/` directory for all slash commands.

## License
MIT
```

### Step 5: Test Locally

Before distributing, test the plugin structure:

```bash
# Add your local directory as a marketplace
/plugin marketplace add /Users/lexchristopherson/Developer/plugin-freedom-system

# List available plugins (should show plugin-freedom-system)
/plugin list

# Install the plugin
/plugin install plugin-freedom-system

# Verify commands are available
/implement --help
```

### Step 6: Prepare for Git Distribution

If distributing via GitHub:

1. **Create .gitignore** (if not exists):
```
# Build artifacts
plugins/*/Builds/
*.o
*.d

# IDE files
.DS_Store
.idea/
*.swp

# But include plugin structure
!.claude-plugin/
!commands/
!skills/
!agents/
!hooks/
```

2. **Commit plugin structure:**
```bash
git add .claude-plugin/ commands/ skills/ agents/ hooks/
git commit -m "feat: add Claude Code plugin packaging"
git push
```

3. **Tag release:**
```bash
git tag -a v1.0.0 -m "Initial Claude Code plugin release"
git push --tags
```

### Step 7: Distribute

**Option A: GitHub Distribution**
Users install with:
```bash
/plugin marketplace add lexchristopherson/plugin-freedom-system
/plugin install plugin-freedom-system
```

**Option B: Team Configuration**
For team standardization, add to `.claude/settings.json`:
```json
{
  "plugins": {
    "marketplaces": [
      "lexchristopherson/plugin-freedom-system"
    ],
    "installed": [
      "plugin-freedom-system"
    ]
  }
}
```

**Option C: Direct Git URL**
```bash
/plugin marketplace add https://github.com/lexchristopherson/plugin-freedom-system.git
```

---

## Maintenance

### Updating the Plugin

1. Make changes to `.claude/` (local development)
2. Copy changes to plugin structure:
   ```bash
   rsync -av .claude/commands/ commands/
   rsync -av .claude/skills/ skills/
   rsync -av .claude/agents/ agents/
   ```
3. Bump version in `.claude-plugin/plugin.json`
4. Commit and tag:
   ```bash
   git commit -am "feat: add new feature"
   git tag v1.1.0
   git push --tags
   ```

### Version Numbering

Follow semantic versioning:
- **MAJOR** (1.0.0 → 2.0.0): Breaking changes (command renamed, API changed)
- **MINOR** (1.0.0 → 1.1.0): New features (new command added)
- **PATCH** (1.0.0 → 1.0.1): Bug fixes (fix existing command)

---

## Advanced: Multi-Plugin Marketplace

If you create multiple Claude Code plugins, organize like this:

```
my-claude-plugins/
├── .claude-plugin/
│   └── marketplace.json
├── plugin-freedom-system/
│   ├── .claude-plugin/plugin.json
│   ├── commands/
│   └── skills/
├── another-plugin/
│   ├── .claude-plugin/plugin.json
│   └── commands/
└── README.md
```

Update `marketplace.json`:
```json
{
  "plugins": [
    {
      "name": "plugin-freedom-system",
      "path": "plugin-freedom-system",
      "description": "JUCE plugin development workflow"
    },
    {
      "name": "another-plugin",
      "path": "another-plugin",
      "description": "Another tool"
    }
  ]
}
```

---

## Troubleshooting

### Plugin Not Showing in `/plugin list`
- Verify `.claude-plugin/plugin.json` exists
- Check JSON syntax with `cat .claude-plugin/plugin.json | jq`
- Ensure `name` field is lowercase with hyphens only

### Commands Not Available After Install
- Check `commands/` directory has `.md` files
- Verify command files have proper frontmatter (if required)
- Restart Claude Code or run `/reload`

### Marketplace Add Fails
- For GitHub: Ensure repo is public or you have access
- For local: Use absolute path or `~/` for home directory
- Check `.claude-plugin/marketplace.json` syntax

---

## Summary Checklist

- [ ] Create `.claude-plugin/plugin.json` with metadata
- [ ] Copy `.claude/commands/` → `commands/`
- [ ] Copy `.claude/skills/` → `skills/`
- [ ] Copy `.claude/agents/` → `agents/`
- [ ] Copy `.claude/hooks/` → `hooks/`
- [ ] Create `.claude-plugin/marketplace.json` (optional)
- [ ] Write README.md with installation instructions
- [ ] Test locally with `/plugin marketplace add <local-path>`
- [ ] Commit and push to GitHub
- [ ] Tag release with semantic version
- [ ] Share marketplace URL with users

---

## Next Steps

After packaging, you can:
1. **Submit to official Claude Code plugin registry** (if available)
2. **Share on GitHub Discussions/Reddit** for visibility
3. **Create documentation site** with detailed guides
4. **Add examples** showing complete plugin development workflows
5. **Integrate CI/CD** to auto-publish on tag push
