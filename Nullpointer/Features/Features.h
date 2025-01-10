#pragma once
#include "../dependencies.h"

void MultiHack(PHANDLE driver, uintptr_t client);
void InitPointersFeatures(PFeaturesStates statesPointer, PEntity array, PConfig config);
void TriggerBot(uintptr_t client, PFeaturesStates featuresPointer);
