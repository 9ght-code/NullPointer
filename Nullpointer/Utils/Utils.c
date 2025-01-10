#include "Utils.h"

void MoveMouse(int dx, int dy) {
	INPUT input = { 0 };

	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;

	SendInput(1, &input, sizeof(INPUT));
}