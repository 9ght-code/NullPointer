#define ENTITY_STRUCTURE
#define OFFSETS
#define UTILS
#define MEMORY
#define MATH
#define GUI
#include "Features.h"

PFeaturesStates featuresPointer;
PEntity entitiesList = { 0 };
PConfig data = { 0 };

void InitPointersFeatures(PFeaturesStates statesPointer, PEntity array, PConfig config) {
	featuresPointer = statesPointer;
	entitiesList = array;
	data = config;
}

boolean NeedToLoop() {
	return featuresPointer->Aimbot || featuresPointer->RadarHack || featuresPointer->Glow || featuresPointer->Health ||
		featuresPointer->Box || featuresPointer->Line || featuresPointer->ESPTeamCheck || featuresPointer->TriggerBot;
}


void MultiHack(PHANDLE driver, uintptr_t client) {

	Entity localPlayer = InitializePlayer(driver, client);

	static uintptr_t entityList;
	ReadMemory(*driver, (uintptr_t)((client + dwEntityList)), &entityList, sizeof(int*));

	static uintptr_t listEntry;
	ReadMemory(*driver, (uintptr_t)(entityList + 0x10), &listEntry, sizeof(int*));

	static ViewMatrix4x4 matrix;
	ReadMemory(*driver, (uintptr_t)(client + dwViewMatrix), &matrix.matrix, sizeof(int[4][4]));

	Entity nearestPlayer = { 0 };

	if (NeedToLoop() == TRUE) {

		float minDist = 100000000;

		for (int i = 0; i < 32; i++) {

			if (listEntry == 0)
				continue;

			uintptr_t pawn;
			ReadMemory(*driver, (uintptr_t)(listEntry + (i * 0x78)), &pawn, sizeof(int*));

			int entityIndex;
			ReadMemory(*driver, (uintptr_t)(localPlayer.pawn + m_iIDEntIndex), &entityIndex, sizeof(int));

			uintptr_t triggerBotPawn;
			ReadMemory(*driver, (uintptr_t)(listEntry + (0x78 * (entityIndex & 0x1FF))), &triggerBotPawn, sizeof(int*));

			UINT8 triggerEntityHealth;
			ReadMemory(*driver, (uintptr_t)(triggerBotPawn + m_iHealth), &triggerEntityHealth, sizeof(uint8_t));

			UINT8 triggerEntityTeam;
			ReadMemory(*driver, (uintptr_t)(triggerBotPawn + m_iTeamNum), &triggerEntityTeam, sizeof(uint8_t));

			int flags;
			ReadMemory(*driver, (uintptr_t)(localPlayer.pawn + m_fFlags), &flags, sizeof(int));

			if (pawn == 0)
				continue;

			int pawnHandle;
			ReadMemory(*driver, (uintptr_t)(pawn + m_hPlayerPawn), &pawnHandle, sizeof(int));

			if (pawnHandle == 0)
				continue;

			uintptr_t listEntry2;
			ReadMemory(*driver, (uintptr_t)(entityList + (0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10)), &listEntry2, sizeof(int*));

			Entity currenEnemy = InitializeEnemy(driver, listEntry2, pawnHandle, &matrix);

			if (currenEnemy.health <= 0) {
				currenEnemy.draw = 1;
				entitiesList[i] = currenEnemy;

				continue;
			}

			if (!featuresPointer->ESPTeamCheck && currenEnemy.team == localPlayer.team) {
				currenEnemy.draw = 1;
				entitiesList[i] = currenEnemy;

				continue;
			}

			else if (featuresPointer->ESPTeamCheck && localPlayer.team == currenEnemy.team)
				currenEnemy.isAlly = 1;

			float currentDist = CalculateDistance2D(&((Vector2) { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 }), &currenEnemy.screenHead);

			if (currentDist < minDist) {
				minDist = currentDist;
				nearestPlayer = currenEnemy;
			}
			entitiesList[i] = currenEnemy;

			// [<-----FUNCS----->]

			// don't even ask fam why i check for flash 3 times, this shit ain't working without that

			if (featuresPointer->Glow == TRUE)
				_Glow(driver, currenEnemy.pawn, GetGlowColor());

			if (featuresPointer->TriggerBot == 1 && entityIndex > 0 && triggerEntityHealth > 0) {
				_TriggerBot(&localPlayer, &triggerEntityTeam, &flags, featuresPointer, data);
			}

			if (featuresPointer->AntiFlash == 1 && localPlayer.health > 0)
				_AntiFlash(driver, localPlayer.pawn);

			if (featuresPointer->RadarHack == 1) // RadarHack
				WriteMemory(*driver, (uintptr_t)(currenEnemy.pawn + m_entitySpottedState + m_bSpotted), 1, sizeof(boolean));
		}

		featuresPointer->poolLoaded = TRUE;

		if (featuresPointer->AntiFlash == 1 && localPlayer.health > 0)
			_AntiFlash(driver, localPlayer.pawn);

		if (featuresPointer->Aimbot == TRUE) {

			if (data->aimbotMode == HOLD && GetKeyState(data->keyEntry.keys[data->keyEntry.selectedKeyAim].keyID) & 0x8000)
				AimAtTarget(&nearestPlayer, minDist, featuresPointer, data);

			else if (data->aimbotMode == PRESS && GetKeyState(data->keyEntry.keys[data->keyEntry.selectedKeyAim].keyID))
				AimAtTarget(&nearestPlayer, minDist, featuresPointer, data);
		}
	}

	if (featuresPointer->AntiFlash == 1 && localPlayer.health > 0)
		_AntiFlash(driver, localPlayer.pawn);

	nearestPlayer.pawn = NULL;

	Sleep(data->sleepTime);
}