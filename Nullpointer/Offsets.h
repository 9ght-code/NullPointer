#pragma once
typedef __int64 ptrdiff_t;

// [<-----OFFSETS----->]
ptrdiff_t dwEntityList = 0x19F2488;
ptrdiff_t dwLocalPlayerPawn = 0x1855CE8;
ptrdiff_t dwViewMatrix = 0x1A54550;

// [<-----CLIENT.DLL----->]
ptrdiff_t m_hPlayerPawn = 0x80C;
ptrdiff_t m_flFlashBangTime = 0x13F8;
ptrdiff_t m_iIDEntIndex = 0x1458;
ptrdiff_t m_iTeamNum = 0x3E3;
ptrdiff_t m_lifestate = 0x348;
ptrdiff_t m_iHealth = 0x344;
ptrdiff_t m_Glow = 0xC00;
ptrdiff_t m_glowColorOverride = 0x40;
ptrdiff_t m_bGlowing = 0x51;
ptrdiff_t m_fFlags = 0x3EC;
ptrdiff_t jump = 0x181C670;
ptrdiff_t m_entitySpottedState = 0x23B8;
ptrdiff_t m_bSpotted = 0x8;
ptrdiff_t m_vOldOrigin = 0x1324;
ptrdiff_t m_vecViewOffset = 0xCB0;
ptrdiff_t m_bIsScoped = 0x23E8; // bool
ptrdiff_t m_bResumeZoom = 0x23E9; // bool
ptrdiff_t m_bIsDefusing = 0x23EA; // bool