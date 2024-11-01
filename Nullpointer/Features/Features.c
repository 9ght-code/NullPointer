#define MEMORY
#define OFFSETS
#include "Features.h"

PFeaturesStates featuresPointer = { 0 };

void InitFeatures(PFeaturesStates features) {
	featuresPointer = features;
}

void static _AntiFlash(PHANDLE driver, uintptr_t localPlayer) {
	float flashDuration;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_flFlashBangTime), &flashDuration, sizeof(float));

	if (flashDuration > 0)
		WriteMemory(*driver, (uintptr_t)(localPlayer + m_flFlashBangTime), 0, sizeof(int));
}


void static _TriggerBot(PHANDLE driver, uintptr_t localPlayer) {
	int flags;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_fFlags), &flags, sizeof(int));

	boolean notInAir = flags & (1 << 0);

	if (notInAir)
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(10);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(10);
}


void static _Glow(PHANDLE driver, uintptr_t currentPawn) {
	WriteMemory(*driver, (uintptr_t)(currentPawn + m_Glow + m_glowColorOverride), 0xFF800080, sizeof(int));
	WriteMemory(*driver, (uintptr_t)(currentPawn + m_Glow + m_bGlowing), 1, 1);
}

void MultiHack(PHANDLE driver, uintptr_t client) {

	int WallHack = featuresPointer->Wallhack;
	int TriggerBot = featuresPointer->TriggerBot;
	int AntiFlash = featuresPointer->AntiFlash;
	int Bhop = featuresPointer->Bhop;
	int Unload = featuresPointer->UNLOAD;
	int RadarHack = featuresPointer->RadarHack;

	uintptr_t localPlayer;
	ReadMemory(*driver, (uintptr_t)(client + dwLocalPlayerPawn), &localPlayer, sizeof(int*));

	UINT8 iHealth;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_iHealth), &iHealth, sizeof(int));

	uintptr_t entityList;
	ReadMemory(*driver, (uintptr_t)((client + dwEntityList)), &entityList, sizeof(int*));

	UINT8 team;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_iTeamNum), &team, sizeof(int));

	uintptr_t listEntry;
	ReadMemory(*driver, (uintptr_t)(entityList + 0x10), &listEntry, sizeof(int*));

	uintptr_t playerViewMatrix;
	ReadMemory(*driver, (uintptr_t)(localPlayer + dwViewMatrix), &playerViewMatrix, sizeof(int*));

	if (WallHack == 1 || RadarHack == 1 || TriggerBot == 1 && iHealth > 0) {
		for (int i = 0; i < 64; i++) {

			// [<-----VARS----->]

			if (listEntry == 0)
				continue;

			int entityIndex;
			ReadMemory(*driver, (uintptr_t)(localPlayer + m_iIDEntIndex), &entityIndex, sizeof(int));

			uintptr_t pawn;
			ReadMemory(*driver, (uintptr_t)(listEntry + (i * 0x78)), &pawn, sizeof(int*));

			uintptr_t triggerBotPawn;
			ReadMemory(*driver, (uintptr_t)(listEntry + (0x78 * (entityIndex & 0x1FF))), &triggerBotPawn, sizeof(int*));

			UINT8 triggerEntityHealth;
			ReadMemory(*driver, (uintptr_t)(triggerBotPawn + m_iHealth), &triggerEntityHealth, sizeof(UINT8));

			if (pawn == 0)
				continue;

			int pawnHandle;
			ReadMemory(*driver, (uintptr_t)(pawn + m_hPlayerPawn), &pawnHandle, sizeof(int));

			if (pawnHandle == 0)
				continue;

			uintptr_t listEntry2;
			ReadMemory(*driver, (uintptr_t)(entityList + (0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10)), &listEntry2, sizeof(int*));

			uintptr_t currentPawn;
			ReadMemory(*driver, (uintptr_t)(listEntry2 + (0x78 * (pawnHandle & 0x1FF))), &currentPawn, sizeof(int*));

			UINT8 health;
			ReadMemory(*driver, (uintptr_t)(currentPawn + m_iHealth), &health, sizeof(UINT8));

			if (health <= 0)
				continue;

			uintptr_t entityViewMatrix;
			ReadMemory(*driver, (uintptr_t)(currentPawn + dwViewMatrix), &entityViewMatrix, sizeof(int*));

			UINT8 entityTeam;
			ReadMemory(*driver, (uintptr_t)(currentPawn + m_iTeamNum), &entityTeam, sizeof(UINT8));

			UINT8 triggerEntityTeam;
			ReadMemory(*driver, (uintptr_t)(triggerBotPawn + m_iTeamNum), &triggerEntityTeam, sizeof(UINT8));
			

			// [<-----FUNCS----->]


			if (team != entityTeam && WallHack == 1)
				_Glow(driver, currentPawn);

			if (RadarHack == 1) // RadarHack
				WriteMemory(*driver, (uintptr_t)(currentPawn + m_entitySpottedState + m_bSpotted), 1, sizeof(boolean));

			if (TriggerBot == 1 && triggerEntityTeam != team && triggerEntityHealth > 0 && entityIndex > 0)
				_TriggerBot(driver, localPlayer);
			
			if (AntiFlash == 1 && iHealth > 0)
				_AntiFlash(driver, localPlayer);

		}
	}

	if (AntiFlash == 1 && iHealth > 0)
		_AntiFlash(driver, localPlayer);
}



