#pragma once

#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include "../Driver/IO_Operations.h"

#define WINDOW_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define WINDOW_HEIGHT GetSystemMetrics(SM_CYSCREEN)

typedef struct _Feature {
    boolean TriggerBot;
    boolean Glow;
	boolean Line;
	boolean Box;
	boolean Health;
	boolean AntiFlash;
	boolean Bhop;
	boolean RadarHack;
	boolean poolLoaded;

    boolean UNLOAD;

} FeaturesStates, * PFeaturesStates;

typedef struct _Config {
	int sleepTime;
	int sleepTriggerTime;

}Config, * PConfig;

#ifdef ENTITY_STRUCTURE

#include "Math/Math.h"
typedef struct _Entity {
	boolean draw;
	boolean isScoped;
	boolean isDefusing;
	uintptr_t pawn;
	UINT8 team;
	UINT8 health;
	Vector3 absOrigin;
	Vector3 eyePos;
	Vector2 position;
	Vector2 screenHead;
	float width;
	float height;

}Entity, * PEntity;

#endif

#ifdef CONFIG

#endif // CONFIG

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



