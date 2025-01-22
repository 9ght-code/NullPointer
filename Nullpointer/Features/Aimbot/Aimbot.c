#define OFFSETS
#define ENTITY_STRUCTURE
#define UTILS
#include "Aimbot.h"

void AimAtTarget(PEntity player, float distance, PFeaturesStates featuresPointer, PConfig data) {
	if (player->health < 0 || !player->pawn)
		return;

	if (featuresPointer->FOV == TRUE && distance > data->fovSize)
		return;

	else if (featuresPointer->FOV == FALSE && distance > WINDOW_WIDTH / 4)
		return;

	POINT mouse;
	GetCursorPos(&mouse);

	float angleX = player->screenHead.x - mouse.x;
	float angleY = player->screenHead.y - mouse.y;

	MoveMouse(angleX, angleY);
}