#pragma once

#include "../dependencies.h"
#include "../Libraries/include/SDL2/SDL.h"
#include "../Libraries/include/SDL2/SDL_syswm.h"
#include "../Libraries/include/SDL2/SDL_image.h"
#include "../Libraries/include/nuklear.h"
#include "../Libraries/include/nuklear_sdl_renderer.h"

typedef struct _GUIConfig {
	struct nk_colorf boxColor;
	struct nk_colorf teamBoxColor;
	struct nk_colorf lineColor;
	struct nk_colorf hpColor;
	struct nk_colorf zoomColor;
	struct nk_colorf defuseColor;
	struct nk_colorf glowColor;

}GUIConfig, * PGUIConfig;

typedef struct _AppInfo {
	struct nk_context* ctx;
	SDL_Window* win;
	SDL_Renderer* render;
}AppInfo, * PAppInfo;

typedef struct _Image {
	SDL_Surface* surface;
	SDL_Texture* texture;
	struct nk_image img;
}Image, * PImage;