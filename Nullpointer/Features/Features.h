#pragma once
#include "Aimbot/Aimbot.h"
#include "Entity/Entity.h"
#include "AntiFlash/AntiFlash.h"
#include "Glow/Glow.h"
#include "TriggerBot/TriggerBot.h"

void MultiHack(PHANDLE driver, uintptr_t client);
void InitPointersFeatures(PFeaturesStates statesPointer, PEntity array, PConfig config);
