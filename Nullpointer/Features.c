
#include <Windows.h>
#include <stdint.h>
#include "memory.h"
#include "Offsets.h"

void TriggerBot(HANDLE driver, uintptr_t client) {

	uintptr_t localPlayer;
	ReadMemory(driver, (uintptr_t)(client + dwLocalPlayerPawn), &localPlayer, sizeof(int*));

	int iHealth;
	ReadMemory(driver, (uintptr_t)(localPlayer + m_iHealth), &iHealth, sizeof(int));

	if (iHealth > 0) { // i'm really aware of CPU


		uintptr_t entityList;
		ReadMemory(driver, (uintptr_t)(client + dwEntityList), &entityList, sizeof(int*));

		int team;
		ReadMemory(driver, (uintptr_t)(localPlayer + m_iTeamNum), &team, sizeof(int));

		int entityIndex;
		ReadMemory(driver, (uintptr_t)(localPlayer + m_iIDEntIndex), &entityIndex, sizeof(int));

		if (entityIndex > 0) {

			uintptr_t listEntry;
			ReadMemory(driver, (uintptr_t)(entityList + (0x8 * ((entityIndex & 0x7FFF) >> 9) + 0x10)), &listEntry, sizeof(int*));

			uintptr_t currentPawn;
			ReadMemory(driver, (uintptr_t)(listEntry + (0x78 * (entityIndex & 0x1FF))), &currentPawn, sizeof(int*));

			int entityTeam;
			ReadMemory(driver, (uintptr_t)(currentPawn + m_iTeamNum), &entityTeam, sizeof(int));

			int health;
			ReadMemory(driver, (uintptr_t)(currentPawn + m_iHealth), &health, sizeof(int));

			int flags;
			ReadMemory(driver, (uintptr_t)(localPlayer + m_fFlags), &flags, sizeof(int));

			boolean notInAir = flags & (1 << 0);

			if (team != entityTeam && health > 0 && notInAir) {
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				Sleep(10);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(10);
			}

		}
	}
}


void AntiFlashBang(HANDLE driver, uintptr_t client) {

	uintptr_t localPlayer;
	ReadMemory(driver, (uintptr_t)(client + dwLocalPlayerPawn), &localPlayer, sizeof(int*));

	int iHealth;
	ReadMemory(driver, (uintptr_t)(localPlayer + m_iHealth), &iHealth, sizeof(int));

	if (iHealth > 0) {

		float flashDuration = 0;
		ReadMemory(driver, (uintptr_t)(localPlayer + m_flFlashBangTime), &flashDuration, sizeof(float));

		if (flashDuration > 0)
			WriteMemory(driver, (uintptr_t)(localPlayer + m_flFlashBangTime), 0, sizeof(int));
	}

}


void GlowWallHack(HANDLE driver, uintptr_t client) {
	uintptr_t localPlayer;
	ReadMemory(driver, (uintptr_t)(client + dwLocalPlayerPawn), &localPlayer, sizeof(int*));

	int iHealth;
	ReadMemory(driver, (uintptr_t)(localPlayer + m_iHealth), &iHealth, sizeof(int));

	uintptr_t entityList;
	ReadMemory(driver, (uintptr_t)((client + dwEntityList)), &entityList, sizeof(int*));

	int team;
	ReadMemory(driver, (uintptr_t)(localPlayer + m_iTeamNum), &team, sizeof(int));

	uintptr_t listEntry;
	ReadMemory(driver, (uintptr_t)(entityList + 0x10), &listEntry, sizeof(int*));

	if (iHealth > 0) {
		for (int i = 0; i < 32; i++) {

			if (listEntry == 0)
				continue;

			uintptr_t pawn;
			ReadMemory(driver, (uintptr_t)(listEntry + (i * 0x78)), &pawn, sizeof(int*));

			if (pawn == 0)
				continue;

			int pawnHandle;
			ReadMemory(driver, (uintptr_t)(pawn + m_hPlayerPawn), &pawnHandle, sizeof(int));

			if (pawnHandle == 0)
				continue;

			uintptr_t listEntry2 = 0;
			ReadMemory(driver, (uintptr_t)(entityList + (0x8 * ((listEntry2 & 0x7FFF) >> 9) + 0x10)), &listEntry2, sizeof(int*));

			uintptr_t currentPawn;
			ReadMemory(driver, (uintptr_t)(listEntry2 + (0x78 * (pawnHandle & 0x1FF))), &currentPawn, sizeof(int*));

			int entityTeam;
			ReadMemory(driver, (uintptr_t)(currentPawn + m_iTeamNum), &entityTeam, sizeof(int));

			int health;
			ReadMemory(driver, (uintptr_t)(currentPawn + m_iHealth), &health, sizeof(int));

			if (health > 0 && team != entityTeam) {
				WriteMemory(driver, (uintptr_t)(currentPawn + m_Glow + m_glowColorOverride), 0xFF800080, sizeof(int));
				WriteMemory(driver, (uintptr_t)(currentPawn + m_Glow + m_bGlowing), 1, 1);
			}

		}

	}
}

void Bhop(HANDLE driver, uintptr_t client) {
	const int forceJumpCode = 65537;
	const int undoJumpCode = 256;

	uintptr_t localPlayer;
	ReadMemory(driver, (uintptr_t)(client + dwLocalPlayerPawn), &localPlayer, sizeof(int*));

	int iHealth;
	ReadMemory(driver, (uintptr_t)(localPlayer + m_iHealth), &iHealth, sizeof(int));

	if (iHealth > 0 && localPlayer > 0) {

		int flags;
		ReadMemory(driver, (uintptr_t)(localPlayer + m_fFlags), &flags, sizeof(int));

		boolean notInAir = flags & (1 << 0);
		boolean space_pressed = GetAsyncKeyState(VK_SPACE);

		ptrdiff_t forceJumpValue;
		ReadMemory(driver, (uintptr_t)(localPlayer + jump), &forceJumpValue, sizeof(int*));

		if (space_pressed && !notInAir) {
			Sleep(10);
			WriteMemory(driver, (uintptr_t)(localPlayer + jump), forceJumpCode, sizeof(int));
		}

		else if (space_pressed && notInAir)
			WriteMemory(driver, (uintptr_t)(localPlayer + jump), undoJumpCode, sizeof(int));

		else if (!space_pressed && forceJumpValue == 65537)
			WriteMemory(driver, (uintptr_t)(localPlayer + jump), undoJumpCode, sizeof(int));

	}

}