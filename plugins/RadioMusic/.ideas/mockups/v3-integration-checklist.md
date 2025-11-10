# RadioMusic v3 UI Integration Checklist

## Overview

This checklist guides you through integrating the WebView UI into your JUCE plugin during **Stage 5 (GUI Implementation)**.

## Prerequisites

- [ ] Stage 4 (DSP Implementation) complete
- [ ] All parameters defined in PluginProcessor
- [ ] AudioProcessorValueTreeState configured with parameters:
  - `station` (0-100%)
  - `start` (0-100%)
  - `speed_pitch` (-1.0 to +1.0)
  - `volume` (-60dB to +12dB)

---

## Step 1: Create UI Directory Structure

```bash
cd plugins/RadioMusic/Source/
mkdir -p ui
```

---

## Step 2: Copy HTML to Source

```bash
cp .ideas/mockups/v3-ui.html Source/ui/ui.html
```

**Verify:**
- [ ] `Source/ui/ui.html` exists
- [ ] File is exactly 700x500px in dimensions

---

## Step 3: Update CMakeLists.txt

Add WebView configuration from `v3-CMakeLists.txt`:

```cmake
# In your existing juce_add_plugin() call:
juce_add_plugin(RadioMusic
    # ... existing settings ...
    NEEDS_WEB_BROWSER TRUE  # Add this line
)

# Add binary data section:
juce_add_binary_data(RadioMusicAssets
    SOURCES
        Source/ui/ui.html
)

# Link assets:
target_link_libraries(RadioMusic
    PRIVATE
        RadioMusicAssets
        juce::juce_gui_extra  # Required for WebBrowserComponent
    # ... other links ...
)

# Add preprocessor definitions:
target_compile_definitions(RadioMusic
    PUBLIC
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURL=0
)
```

**Verify:**
- [ ] `NEEDS_WEB_BROWSER TRUE` set
- [ ] `juce_add_binary_data` section added
- [ ] `juce::juce_gui_extra` linked
- [ ] `JUCE_WEB_BROWSER=1` defined

---

## Step 4: Update PluginEditor.h

Replace your existing `PluginEditor.h` with content from `v3-PluginEditor.h`.

**Key additions:**
- [ ] `#include <JuceHeader.h>`
- [ ] `std::unique_ptr<juce::WebBrowserComponent> webView;`
- [ ] `std::unique_ptr<juce::FileChooser> fileChooser;`
- [ ] `void sendMessageToUI (const juce::String& message);`
- [ ] `void updateWaveformData();`
- [ ] `void updateFileInfo();`
- [ ] Timer callback for playback position updates

---

## Step 5: Update PluginEditor.cpp

Replace your existing `PluginEditor.cpp` with content from `v3-PluginEditor.cpp`.

**Implementation checklist:**
- [ ] WebView initialized in constructor
- [ ] HTML loaded from `BinaryData::ui_html`
- [ ] Parameter listeners registered
- [ ] Timer started at 60fps
- [ ] `handleMessage()` implemented for JavaScript → C++
- [ ] `sendMessageToUI()` implemented for C++ → JavaScript
- [ ] `updateWaveformData()` sends waveform to UI
- [ ] `updateFileInfo()` sends file metadata to UI
- [ ] Browse button triggers folder chooser

---

## Step 6: Add Required PluginProcessor Methods

Your `PluginProcessor` needs these methods for the UI:

```cpp
// In PluginProcessor.h:
class RadioMusicAudioProcessor : public juce::AudioProcessor
{
public:
    // ... existing methods ...

    // UI integration methods
    float getPlaybackPosition() const;
    std::vector<float> getCurrentWaveformData() const;
    int getCurrentFileIndex() const;
    juce::String getCurrentFileName() const;
    int getTotalFiles() const;
    void loadFolder (const juce::File& folder);

private:
    // ... your DSP implementation ...
};
```

**Verify:**
- [ ] All methods declared in header
- [ ] All methods implemented in .cpp
- [ ] Waveform data downsampled to ~800 points
- [ ] File list cached for quick access

---

## Step 7: Build and Test Standalone

```bash
cd build
cmake --build . --target RadioMusic_Standalone
```

**Test checklist:**
- [ ] Build succeeds without errors
- [ ] Standalone app launches
- [ ] UI displays correctly (700x500)
- [ ] Folder button opens file chooser
- [ ] Waveform displays after loading folder
- [ ] Knobs rotate when dragged
- [ ] Parameter values update in UI
- [ ] Speed/Volume knobs snap to center
- [ ] File info displays correctly
- [ ] Playback cursor animates

---

## Step 8: Test in DAW

**AU/VST3 Testing:**

1. Build plugin:
   ```bash
   cmake --build . --target RadioMusic_AU
   cmake --build . --target RadioMusic_VST3
   ```

2. Test in DAW:
   - [ ] Plugin loads without crash
   - [ ] UI renders correctly
   - [ ] Knobs control audio parameters
   - [ ] Waveform updates when changing files
   - [ ] Automation works on all parameters
   - [ ] State saves/restores correctly
   - [ ] No crashes on project reload

---

## Step 9: Troubleshooting Common Issues

### WebView doesn't load

**Check:**
- `JUCE_WEB_BROWSER=1` defined in CMake
- `juce::juce_gui_extra` linked
- `BinaryData::ui_html` accessible
- macOS: Developer tools enabled for WebView

**Fix:**
```cpp
// Add error logging in constructor:
webView->pageLoadHadNetworkError = [](const juce::String& error) {
    DBG("WebView error: " + error);
};
```

### JavaScript → C++ communication broken

**Check:**
- `handleMessage()` implemented and called
- JSON parsing works correctly
- Parameter IDs match exactly

**Test:**
```javascript
// In browser console (v3-ui-test.html):
juce.postMessage({type: 'test', data: 'hello'});
```

### Waveform doesn't update

**Check:**
- `getCurrentWaveformData()` returns valid data
- `updateWaveformData()` called when file changes
- JavaScript `updateFromBackend()` receives data
- Canvas drawing logic runs

**Debug:**
```cpp
// In updateWaveformData():
DBG("Sending waveform with " + juce::String(waveformData.size()) + " points");
```

### Parameters don't respond to knobs

**Check:**
- Parameter IDs match (`data-param` in HTML)
- `parameterChanged()` listener registered
- Value conversion correct (0-1 normalized vs actual range)

**Debug:**
```javascript
// In updateParameter():
console.log(`Parameter ${param} changed to ${value}`);
```

### Knobs don't snap to center

**Check:**
- `.bipolar` class applied to Speed/Volume knobs
- `snapThreshold` values correct (0.05 for speed, 0.5 for volume)
- Center snap logic in `mousemove` handler

---

## Step 10: Optimization (Optional)

### Reduce waveform update frequency

```cpp
// In timerCallback(), only update if position changed significantly:
if (std::abs(newPosition - lastPosition) > 0.01f)
{
    lastPosition = newPosition;
    // send update
}
```

### Throttle parameter updates to UI

```cpp
// Use atomic flag to prevent flooding:
std::atomic<bool> parameterUpdatePending {false};

void parameterChanged(...)
{
    if (!parameterUpdatePending.exchange(true))
    {
        juce::MessageManager::callAsync([this, parameterID, newValue]()
        {
            sendMessageToUI(...);
            parameterUpdatePending = false;
        });
    }
}
```

---

## Success Criteria

UI integration is complete when:

- ✅ All 4 knobs control their parameters
- ✅ Waveform displays loaded files
- ✅ Start position masks waveform correctly
- ✅ Playback cursor animates smoothly
- ✅ Folder browse button works
- ✅ File info displays correctly
- ✅ Speed/Volume snap to center
- ✅ No crashes in DAW
- ✅ State persistence works
- ✅ Automation records/plays back

---

## Next Steps

After UI integration:

1. **Stage 6 (Validation & Testing):**
   - Run automated tests
   - Test edge cases (empty folders, corrupt files)
   - Performance profiling

2. **Stage 7 (Packaging):**
   - Create installer
   - Code signing
   - Deploy to system folders

3. **User testing:**
   - Get feedback on UI/UX
   - Iterate if needed (create v4 mockup)
