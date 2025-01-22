#pragma once
#include "../dependencies.h"

void MoveMouse(int dx, int dy);
void ClickMouse(int delay);

void FillKeys(PKeyEntry entry);
const char* GetKeyName(int key);