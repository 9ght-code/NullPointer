#pragma once

// [<-----DECLARATIONS----->]
#include "NuklearDefinition.h"
#include "Tabs/ESP/ESP.h"
#include "Tabs/TriggerBot/TriggerTab.h"
#include "Tabs/Misc/MiscTab.h"
#include "Tabs/Glow/GlowTab.h"
#include "Tabs/AimBot/AimTab.h"

#include "OverlaySetup/Overlay.h"

// [<-----FUNCS----->]
void showWindow(PFeaturesStates Features);
void InitPointersGUI(PEntity array, PConfig config);
unsigned int GetGlowColor();

#ifdef INTERNAL_GUI

enum State { DISABLED, ENABLED };

enum Tabs {
	TAB_TRIGGERBOT,
	TAB_AIMBOT,
	TAB_GLOW,
	TAB_ESP,
	TAB_MISC,
	TAB_EXIT,
	TAB_COUNT
};

#endif