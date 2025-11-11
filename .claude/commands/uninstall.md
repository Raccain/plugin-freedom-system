---
name: uninstall
description: Remove plugin from system folders (uninstall)
---

# /uninstall

When user runs `/uninstall [PluginName]`, invoke the plugin-lifecycle skill with uninstallation mode.

## Preconditions

- Plugin must be installed (status: 📦 Installed in PLUGINS.md)
- Cannot uninstall if plugin is 🚧 in development (use /continue first)

## Behavior

Invoke plugin-lifecycle skill following the uninstallation workflow:
1. Locate plugin files (extract PRODUCT_NAME from CMakeLists.txt)
2. Confirm removal with user (show file sizes, preserve source code)
3. Remove from system folders (VST3 + AU)
4. Clear DAW caches (Ableton + Logic)
5. Update PLUGINS.md status: 📦 Installed → ✅ Working
6. Present decision menu for next steps

See `.claude/skills/plugin-lifecycle/references/uninstallation-process.md` for complete workflow.

## Success Output

```
✓ [PluginName] uninstalled

Removed from system folders:
- VST3: ~/Library/Audio/Plug-Ins/VST3/[ProductName].vst3 (deleted)
- AU: ~/Library/Audio/Plug-Ins/Components/[ProductName].component (deleted)

Cache status: Cleared (Ableton + Logic)

Source code remains: plugins/[PluginName]/

To reinstall: /install-plugin [PluginName]
```

## Routes To

`plugin-lifecycle` skill (uninstallation mode)
