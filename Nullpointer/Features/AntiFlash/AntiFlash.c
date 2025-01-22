#define OFFSETS
#define ENTITY_STRUCTURE
#define MEMORY
#include "AntiFlash.h"

void _AntiFlash(PHANDLE driver, uintptr_t localPlayer) {
	float flashDuration;
	ReadMemory(*driver, (uintptr_t)(localPlayer + m_flFlashBangTime), &flashDuration, sizeof(float));

	if (flashDuration > 0)
		WriteMemory(*driver, (uintptr_t)(localPlayer + m_flFlashBangTime), 0, sizeof(int));
}