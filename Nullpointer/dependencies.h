#pragma once

// Core system headers
#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <windows.graphics.h>

// Local dependencies
#include "../Driver/IO_Operations.h"

// Feature flags structure for cheat functionality
typedef struct _Feature {
    int TriggerBot;    // Auto-shooting when crosshair on enemy
    int Wallhack;      // See enemies through walls
    int AntiFlash;     // Prevent flashbang effects
    int Bhop;          // Bunny hop assistance
    int RadarHack;     // Enhanced radar visibility
    boolean UNLOAD;    // Flag to unload cheat

} FeaturesStates, *PFeaturesStates;

// Conditional includes based on build configuration
#ifdef MEMORY
#include "Memory/memory.h"
#endif // MEMORY

#ifdef GUI
#include "GUI/GUI.h"
#endif // GUI

#ifdef OFFSETS
#include "Offsets.h"
#endif // OFFSETS

#ifdef FEATURES
#include "Features/Features.h"
#endif // FEATURES

#ifdef MATH
#include "Math/Math.h"
#endif // MATH

// Full stack configuration includes all modules
#ifdef FULL_STACK
#include "Offsets.h"
#include "Memory/memory.h"
#include "GUI/GUI.h"
#include "Features/Features.h"
#include "Math/Math.h"
#endif // FULL_STACK
