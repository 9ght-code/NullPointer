#define OFFSETS
#define ENTITY_STRUCTURE
#define MATH
#define MEMORY
#include "Entity.h"

Entity InitializeEnemy(PHANDLE driver, uintptr_t listEntry2, int pawnHandle, PViewMatrix4x4 matrix) {
	Entity currentEnemy = { 0 };

	ReadMemory(*driver, (uintptr_t)(listEntry2 + (0x78 * (pawnHandle & 0x1FF))), &currentEnemy.pawn, sizeof(int*));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_iHealth), &currentEnemy.health, sizeof(UINT8));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_iTeamNum), &currentEnemy.team, sizeof(UINT8));

	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_vOldOrigin), &currentEnemy.absOrigin, sizeof(Vector3));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_pGameSceneNode), &currentEnemy.sceneNode, sizeof(int*));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.sceneNode + m_modelState + dwBoneMatrx), &currentEnemy.model, sizeof(int*));

	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_bIsScoped), &currentEnemy.isScoped, sizeof(boolean));
	ReadMemory(*driver, (uintptr_t)(currentEnemy.pawn + m_bIsDefusing), &currentEnemy.isDefusing, sizeof(boolean));

	if (WorldToScreen(&currentEnemy.absOrigin, &currentEnemy.position, matrix, WINDOW_WIDTH, WINDOW_HEIGHT) == 1) {
		Vector3 head;
		ReadMemory(*driver, (uintptr_t)(currentEnemy.model + (HEAD * 32)), &head, sizeof(Vector3));

		if (WorldToScreen(&head, &currentEnemy.screenHead, matrix, WINDOW_WIDTH, WINDOW_HEIGHT) == 1) {

			currentEnemy.height = (currentEnemy.position.y - currentEnemy.screenHead.y);
			currentEnemy.width = currentEnemy.height / 2.4f;
		}
	}

	return currentEnemy;
}

Entity InitializePlayer(PHANDLE driver, uintptr_t client) {
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

