#pragma once

#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <windows.graphics.h>
#include "../Driver/IO_Operations.h"

typedef struct _Feature {
    int TriggerBot;
    int Wallhack;
    int AntiFlash;
    int Bhop;
    int RadarHack;
    boolean UNLOAD;

} FeaturesStates, * PFeaturesStates;

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
#endif

#ifdef FULL_STACK
#include "Offsets.h"
#include "Memory/memory.h"
#include "GUI/GUI.h"
#include "Features/Features.h"
#include "Math/Math.h"
#endif // FULL_STACK



