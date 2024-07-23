#pragma once
#include <Windows.h>

//offsets
ptrdiff_t dwEntityList = 0x19BDD58;
ptrdiff_t dwLocalPlayerPawn = 0x1823A08;

//client.dll
ptrdiff_t m_hPlayerPawn = 0x7DC;
ptrdiff_t m_flFlashBangTime = 0x1348; // float32
ptrdiff_t m_iIDEntIndex = 0x13A8;
ptrdiff_t m_iTeamNum = 0x3C3;
ptrdiff_t m_iHealth = 0x324; // int32
ptrdiff_t m_Glow = 0xBA0;
ptrdiff_t m_glowColorOverride = 0x40;
ptrdiff_t m_bGlowing = 0x51;
ptrdiff_t m_fFlags = 0x3CC; 
ptrdiff_t jump = 0x181C670;
ptrdiff_t m_entitySpottedState = 0x2288;
ptrdiff_t m_bSpotted = 0x8;