cmake_minimum_required(VERSION 3.22)

# Plugin formats: VST3, Standalone (Windows — no AU)
juce_add_plugin(GrooveScout
    COMPANY_NAME "PFS"
    PLUGIN_MANUFACTURER_CODE PFSx
    PLUGIN_CODE Gscv
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "GrooveScout"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    NEEDS_WEB_BROWSER TRUE
)

# Source files
target_sources(GrooveScout
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/GrooveScoutAnalyzer.cpp
)

# Include paths
target_include_directories(GrooveScout
    PRIVATE
        Source
)

# Required JUCE modules
target_link_libraries(GrooveScout
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# CRITICAL: Generate JuceHeader.h — MUST come after target_link_libraries()
# and BEFORE target_compile_definitions() (JUCE 8 requirement, Pattern 1)
juce_generate_juce_header(GrooveScout)

# WebView UI Resources — embedded into plugin binary via juce_add_binary_data
# Pattern #8: Explicit URL mapping in getResource() bridges BinaryData names to URL paths
# Pattern #13: check_native_interop.js REQUIRED for native bridge initialization
juce_add_binary_data(GrooveScout_UIResources
    SOURCES
        Source/ui/public/index.html
        Source/ui/public/js/juce/index.js
        Source/ui/public/js/juce/check_native_interop.js
)

target_link_libraries(GrooveScout
    PRIVATE
        GrooveScout_UIResources
)

# Compile definitions
target_compile_definitions(GrooveScout
    PUBLIC
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURL=0
        $<$<PLATFORM_ID:Windows>:JUCE_USE_WIN_WEBVIEW2=1>
)

# Windows: WebView2 SDK headers (required for JUCE_USE_WIN_WEBVIEW2)
if(WIN32)
    set(WEBVIEW2_PATH "C:/Data/Code/utils/WebView2" CACHE PATH "Path to WebView2 NuGet package")
    if(EXISTS "${WEBVIEW2_PATH}/build/native/include/WebView2.h")
        target_include_directories(GrooveScout PRIVATE "${WEBVIEW2_PATH}/build/native/include")
    else()
        message(WARNING "WebView2 SDK not found at ${WEBVIEW2_PATH}. Download from NuGet.")
    endif()
endif()

# C++17 standard
target_compile_features(GrooveScout
    PRIVATE
        cxx_std_17
)
