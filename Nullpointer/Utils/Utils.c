#include "Utils.h"

void MoveMouse(int dx, int dy) {
	INPUT input = { 0 };

	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;

	SendInput(1, &input, sizeof(INPUT));
}

const char* GetKeyName(int key) {
	static char keyName[32];

    if (key == 0) return "None";
    GetKeyNameTextA(MapVirtualKeyA(key, MAPVK_VK_TO_VSC) << 16, keyName, sizeof(keyName));
    return keyName;
}

void ClickMouse(int delay) {
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(delay);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(delay);
}