#define MEMORY
#define OFFSETS
#define MATH
#define GUI
#define ENTITY_STRUCTURE

#include "Features.h"

PFeaturesStates featuresPointer = { 0 };
PEntity entitiesList = { 0 };
PConfig data = { 0 };

//[<-----PRIVATE METHODS----->]

void static _AntiFlash(PHANDLE driver, uintptr_t localPlayer) {
	float flashDuration;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_flFlashBangTime), &flashDuration, sizeof(float));

	if (flashDuration > 0)
		WriteMemory(*driver, (uintptr_t)(localPlayer + m_flFlashBangTime), 0, sizeof(int));
}

Entity _InitializeEnemy(PHANDLE driver, uintptr_t listEntry2, int pawnHandle, PViewMatrix4x4 matrix) {
	Entity currentEnemy = { 0 };

	ReadMemory(*driver, (uintptr_t)(listEntry2 + (0x78 * (pawnHandle & 0x1FF))), &currentEnemy.pawn, sizeof(int*));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_iHealth), &currentEnemy.health, sizeof(UINT8));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_iTeamNum), &currentEnemy.team, sizeof(UINT8));

	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_vOldOrigin), &currentEnemy.absOrigin, sizeof(Vector3));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_pGameSceneNode), &currentEnemy.sceneNode, sizeof(int*));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.sceneNode + m_modelState + 0x80), &currentEnemy.model, sizeof(int*));

	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_bIsScoped), &currentEnemy.isScoped, sizeof(boolean));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_bIsDefusing), &currentEnemy.isDefusing, sizeof(boolean));

	if (WorldToScreen(&currentEnemy.absOrigin, &currentEnemy.position, matrix, WINDOW_WIDTH, WINDOW_HEIGHT) == 1) {
		Vector3 head = { .x = currentEnemy.absOrigin.x, .y = currentEnemy.absOrigin.y, .z = currentEnemy.absOrigin.z + 75.f };
		ReadMemory(*driver, (uintptr_t)(currentEnemy.model + (6 * 32)), &head, sizeof(Vector3));

		if (WorldToScreen(&head, &currentEnemy.screenHead, matrix, WINDOW_WIDTH, WINDOW_HEIGHT) == 1) {

			currentEnemy.height = (currentEnemy.position.y - currentEnemy.screenHead.y);
			currentEnemy.width = currentEnemy.height / 2.4f;
		}
	}

	return currentEnemy;
}

Entity _InitializePlayer(PHANDLE driver, uintptr_t client) {
	Entity player = { 0 };

	uintptr_t localPlayer;
	ReadMemory(*driver, (uintptr_t)(client + dwLocalPlayerPawn), &localPlayer, sizeof(int*));

	UINT8 iHealth;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_iHealth), &iHealth, sizeof(int));

	UINT8 team;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_iTeamNum), &team, sizeof(int));

	player.pawn = localPlayer;
	player.health = iHealth;
	player.team = team;

	return player;
}

void static _Glow(PHANDLE driver, uintptr_t currentPawn) {
	WriteMemory(*driver, (uintptr_t)(currentPawn + m_Glow + m_glowColorOverride), GetGlowColor(), sizeof(int));
	WriteMemory(*driver, (uintptr_t)(currentPawn + m_Glow + m_bGlowing), 1, 1);
}

void _TriggerBot(PEntity localPlayer, UINT8* triggerEntityTeam, int* flags) {
	boolean notInAir = *flags & (1 << 0);

	if (notInAir) {


		if (featuresPointer->TriggerTeamCheck && *triggerEntityTeam != localPlayer->team)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(data->sleepTriggerTime);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			Sleep(data->sleepTriggerTime);
		}

		else if (!featuresPointer->TriggerTeamCheck)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(data->sleepTriggerTime);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			Sleep(data->sleepTriggerTime);
		}

	}
}


//[<-----PUBLIC METHODS----->]

void InitPointersFeatures(PFeaturesStates statesPointer, PEntity array, PConfig config) {
	featuresPointer = statesPointer;
	entitiesList = array;
	data = config;
}

void MultiHack(PHANDLE driver, uintptr_t client) {

	int WallHack = featuresPointer->Glow;
	int TriggerBot = featuresPointer->TriggerBot;
	int AntiFlash = featuresPointer->AntiFlash;
	int Bhop = featuresPointer->Bhop;
	int Unload = featuresPointer->UNLOAD;
	int ESPBox = featuresPointer->Box;
	int ESPLine = featuresPointer->Line;
	int ESPHealth = featuresPointer->Health;
	int RadarHack = featuresPointer->RadarHack;
	int EspTeamCheck = featuresPointer->ESPTeamCheck;

	Entity localPlayer = _InitializePlayer(driver, client);

	uintptr_t entityList;
	ReadMemory(*driver, (uintptr_t)((client + dwEntityList)), &entityList, sizeof(int*));

	uintptr_t listEntry;
	ReadMemory(*driver, (uintptr_t)(entityList + 0x10), &listEntry, sizeof(int*));

	ViewMatrix4x4 matrix;
	ReadMemory(*driver, (uintptr_t)(client + dwViewMatrix), &matrix.matrix, sizeof(int[4][4]));

	Entity nearestPlayer = { 0 };
	float minDist = 100000000;

	if ((WallHack == 1 || RadarHack == 1 || TriggerBot == 1 || ESPLine || ESPBox || ESPHealth || EspTeamCheck == 1) && localPlayer.health > 0) {

		for (int i = 0; i < 64; i++) {

			if (listEntry == 0)
				continue;

			uintptr_t pawn;
			ReadMemory(*driver, (uintptr_t)(listEntry + (i * 0x78)), &pawn, sizeof(int*));

			int entityIndex;
			ReadMemory(*driver, (uintptr_t)(localPlayer.pawn + m_iIDEntIndex), &entityIndex, sizeof(int));

			uintptr_t triggerBotPawn;
			ReadMemory(*driver, (uintptr_t)(listEntry + (0x78 * (entityIndex & 0x1FF))), &triggerBotPawn, sizeof(int*));

			UINT8 triggerEntityHealth;
			ReadMemory(*driver, (uintptr_t)(triggerBotPawn + m_iHealth), &triggerEntityHealth, sizeof(UINT8));

			UINT8 triggerEntityTeam;
			ReadMemory(*driver, (uintptr_t)(triggerBotPawn + m_iTeamNum), &triggerEntityTeam, sizeof(UINT8));

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

			Entity currenEnemy = _InitializeEnemy(driver, listEntry2, pawnHandle, &matrix);

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

			float currentDist = CalculateDistance2D(&((Vector2) {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}), &currenEnemy.screenHead);

			if (currentDist < minDist) {
				minDist = currentDist;
				nearestPlayer = currenEnemy;
			}
			entitiesList[i] = currenEnemy;

			// [<-----FUNCS----->]

			if (WallHack)
				_Glow(driver, currenEnemy.pawn);

			if (TriggerBot == 1 && entityIndex > 0 && triggerEntityHealth > 0) {
				_TriggerBot(&localPlayer, &triggerEntityTeam, &flags);
			}

			if (RadarHack == 1) // RadarHack
				WriteMemory(*driver, (uintptr_t)(currenEnemy.pawn + m_entitySpottedState + m_bSpotted), 1, sizeof(boolean));

		}

		minDist = 10000000;


		if (AntiFlash == 1 && localPlayer.health > 0)
			_AntiFlash(driver, localPlayer.pawn);

		featuresPointer->poolLoaded = TRUE;

	}


	if (AntiFlash == 1 && localPlayer.health > 0)
		_AntiFlash(driver, localPlayer.pawn);

	Sleep(data->sleepTime);
}



