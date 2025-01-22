#pragma once

#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include "../Driver/IO_Operations.h"

#define WINDOW_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define WINDOW_HEIGHT GetSystemMetrics(SM_CYSCREEN)

typedef struct _Feature {
	boolean Aimbot;
    boolean TriggerBot;
    boolean Glow;

	boolean Line;
	boolean Box;
	boolean Health;

	boolean AntiFlash;
	boolean Bhop;
	boolean RadarHack;

	boolean ESPTeamCheck;
	boolean TriggerTeamCheck;

	boolean FOV;
	boolean poolLoaded;

	boolean Ghost;
    boolean UNLOAD;

} FeaturesStates, * PFeaturesStates;


enum Mode {

	HOLD = 0,
	PRESS = 1

};

typedef struct _KeyBind {
	int keyID;
	char* name;
}KeyBind, * PKeyBind;


typedef struct _KeyEntry {
	KeyBind keys[7];
	int selectedKeyAim;
	int selectedKeyTrigger;

}KeyEntry, * PKeyEntry;


typedef struct _Config {
	int sleepTime;
	const SYSTEM_INFO sysinf;

	int preShotDelay;
	int postShotDelay;

	DWORD sleepTriggerTime;
	boolean shootInAir;

	int fovSize;
	KeyEntry keyEntry;

	enum Mode aimbotMode;
	enum Mode triggerMode;


}Config, * PConfig;

#ifdef ENTITY_STRUCTURE

#include "Math/Math.h"
typedef struct _Entity {
	boolean draw;
	boolean isScoped;
	boolean isDefusing;
	boolean isAlly;

	UINT8 team;
	UINT8 health;

	Vector3 absOrigin;
	Vector2 position;
	Vector2 screenHead;
	Vector2 screenBody;

	float width;
	float height;

	uintptr_t pawn;
	uintptr_t sceneNode;
	uintptr_t model;

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

#ifdef UTILS
#include "Utils/Utils.h"
#endif // UTILS


#ifdef FULL_STACK
#include "Offsets.h"
#include "Memory/memory.h"
#include "GUI/GUI.h"
#include "Features/Features.h"
#include "Math/Matils.h"
#include "Utils/Utils.h"
#endif // FULL_STACK




