#define OFFSETS
#define ENTITY_STRUCTURE
#define MEMORY
#include "Glow.h"

void _Glow(PHANDLE driver, uintptr_t currentPawn, int color) {
	WriteMemory(*driver, (uintptr_t)(currentPawn + m_Glow + m_glowColorOverride), color, sizeof(int));
	WriteMemory(*driver, (uintptr_t)(currentPawn + m_Glow + m_bGlowing), 1, 1);
}