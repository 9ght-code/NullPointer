#pragma once

// [<-----DECLARATIONS----->]
#include "../dependencies.h"
#include "NuklearDefinition.h"


#ifdef NUKLEAR_THEMES
#include "Themes/NuklearThemes.h"
#endif

// [<-----FUNCS----->]
void showWindow(PFeaturesStates Features);
void InitPointersGUI(PEntity array, PConfig conf);
unsigned int GetGlowColor();

#ifdef INTERNAL_GUI

enum State { DISABLED, ENABLED };


typedef struct _GUIConfig {
	struct nk_colorf boxColor;
	struct nk_colorf lineColor;
	struct nk_colorf hpColor;
	struct nk_colorf zoomColor;
	struct nk_colorf defuseColor;
	struct nk_colorf glowColor;

}GUIConfig, *PGUIConfig;

enum Tabs {
	TAB_TRIGGERBOT,
	TAB_AIMBOT,
	TAB_GLOW,
	TAB_ESP,
	TAB_MISC,
	TAB_EXIT,
	TAB_COUNT
};

enum VisualSubTabs {
	VISUAL_GLOW,
	VISUAL_WALLHACK,
	VISUAL_COUNT
};

typedef struct _AppInfo {
	struct nk_context* ctx;
	SDL_Window* win;
	SDL_Renderer* render;
}AppInfo, *PAppInfo;

typedef struct _Image {
	SDL_Surface* surface;
	SDL_Texture* texture;
	struct nk_image img;
}Image, *PImage;

#endif