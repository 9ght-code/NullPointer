#define OFFSETS
#define ENTITY_STRUCTURE
#define UTILS
#include "TriggerBot.h"

void Shoot(PConfig data) {
	if (data->triggerMode == HOLD && GetKeyState(data->keyEntry.keys[data->keyEntry.selectedKeyTrigger].keyID) & 0x8000)
		ClickMouse(data->sleepTriggerTime);

	else if (data->triggerMode == PRESS && GetKeyState(data->keyEntry.keys[data->keyEntry.selectedKeyTrigger].keyID))
		ClickMouse(data->sleepTriggerTime);
}

void _TriggerBot(PEntity localPlayer, UINT8* triggerEntityTeam, int* flags, PFeaturesStates featuresPointer, PConfig data) {
	boolean notInAir = *flags & (1 << 0);

	if (notInAir) {

		Sleep(data->preShotDelay);

		if (featuresPointer->TriggerTeamCheck == TRUE && *triggerEntityTeam != localPlayer->team)
			Shoot(data);

		else if (featuresPointer->TriggerTeamCheck == FALSE)
			Shoot(data);

		Sleep(data->postShotDelay);

	}
}