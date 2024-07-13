#pragma once
#include "Libraries/include/SDL2/SDL.h"

typedef struct _Feature {
    int TriggerBot;
    int Wallhack;
    int AntiFlash;
    int Bhop;
    boolean UNLOAD;

} FeaturesStates, * PFeaturesStates;


void SetWindowTransparency(SDL_Window* window);

void showWindow(PFeaturesStates Features);