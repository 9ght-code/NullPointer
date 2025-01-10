#pragma once
typedef __int64 ptrdiff_t;

// [<-----OFFSETS----->]
ptrdiff_t dwEntityList = 0x1A157C8;
ptrdiff_t dwLocalPlayerPawn = 0x1869D88;
ptrdiff_t dwViewMatrix = 0x1A80870;
ptrdiff_t dwBoneMatrx = 0x80;

// [<-----CLIENT.DLL----->]
ptrdiff_t m_hPlayerPawn = 0x80C;
ptrdiff_t m_flFlashBangTime = 0x13F8;
ptrdiff_t m_iIDEntIndex = 0x1458;
ptrdiff_t m_iTeamNum = 0x3E3;
ptrdiff_t m_iHealth = 0x344;
ptrdiff_t m_Glow = 0xC00;
ptrdiff_t m_glowColorOverride = 0x40;
ptrdiff_t m_bGlowing = 0x51;
ptrdiff_t m_fFlags = 0x3EC;
ptrdiff_t jump = 0x181C670;
ptrdiff_t m_entitySpottedState = 0x1B58;
ptrdiff_t m_bSpotted = 0x8;
ptrdiff_t m_vOldOrigin = 0x1324;
ptrdiff_t m_vecViewOffset = 0xCB0;
ptrdiff_t m_bIsScoped = 0x23E8; // bool
ptrdiff_t m_bResumeZoom = 0x23E9; // bool
ptrdiff_t m_bIsDefusing = 0x23EA; // bool
ptrdiff_t m_modelState = 0x170; // int*
ptrdiff_t m_pGameSceneNode = 0x328; //int*
ptrdiff_t m_lifeState = 0x348; //uint8
ptrdiff_t m_bPawnIsAlive = 0x814;

enum BoneID{

	HEAD = 6,
	NECK = 5,
	CHEST = 4,
	STOMACH = 2,
	PENIS = 0,
	LEFT_SHOULDER = 8,
	LEFT_ARM = 9,
	LEFT_HAND = 10,
	RIGHT_SHOULDER = 13,
	RIGHT_ARM = 14,
	RIGHT_HAND = 15,
	LEFT_THIGH = 22,
	LEFT_CALF = 23,
	LEFT_FOOT = 24,
	RIGHT_THIGH = 25,
	R_CALF = 26,
	RIGHT_FOOT = 27

};