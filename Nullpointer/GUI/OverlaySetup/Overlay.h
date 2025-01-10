#pragma once
#include "../NuklearDefinition.h"
#include "../../Memory/memory.h"
#include "../Themes/NuklearThemes.h"

PAppInfo InitGUI();

void ShowLogo(PMemoryPool pool);
void SetWindowTransparency(SDL_Window* window, boolean state);