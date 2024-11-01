#include "GUI.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#include "../Libraries/include/nuklear.h"
#include "../Libraries/include/nuklear_sdl_renderer.h"

#define WINDOW_WIDTH 840
#define WINDOW_HEIGHT 600

HWND wnd;
HBRUSH brush;
HDC dc;
boolean* show = TRUE;

void SetWindowTransparency(SDL_Window* window) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

    HWND hwnd = wmInfo.info.win.window;
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}


void showWindow(PFeaturesStates Features) {
    /* Platform */
    SDL_Window* win;
    SDL_Renderer* renderer;

    int running = 1;
    int flags = 0;
    float font_scale = 1;


    /* GUI */
    struct nk_context* ctx;

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS);

    if (win == NULL) {
        SDL_Log("Error SDL_CreateWindow %s", SDL_GetError());
        exit(-1);
    }



    SDL_SetWindowAlwaysOnTop(win, SDL_TRUE);
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);

    flags |= SDL_RENDERER_ACCELERATED;
    flags |= SDL_RENDERER_PRESENTVSYNC;


    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_Log("Error SDL_CreateRenderer %s", SDL_GetError());
        exit(-1);
    }

    SetWindowTransparency(win);

    /* scale the renderer output for High-DPI displays */
    {
        int render_w, render_h;
        int window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(renderer, &render_w, &render_h);
        SDL_GetWindowSize(win, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(renderer, scale_x, scale_y);
        font_scale = scale_y;
    }

    enum State { DISABLED, ENABLED };

    ctx = nk_sdl_init(win, renderer);

    {
        struct nk_font_atlas* atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font* font;

        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        nk_sdl_font_stash_end();

        font->handle.height /= font_scale;

        nk_style_set_font(ctx, &font->handle);
    }

    while (running)
    {
        /* Input */
        SDL_Event evt;

        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        if (GetKeyState(VK_F9)) {
            SDL_ShowWindow(win);
        }

        else 
            SDL_HideWindow(win);

        /* GUI */
        

        if (nk_begin(ctx, "NullPointer", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE))
        {

            static enum State triggerBot = DISABLED;
            static enum State wallHack = DISABLED;
            static enum State BHOP = DISABLED;
            static enum State AntiFlash = DISABLED;
            static enum State Radar = DISABLED;
            static enum STATE ESP = DISABLED;

            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, "TriggerBot", NK_LEFT);

            if (nk_option_label(ctx, "Enabled", triggerBot == ENABLED))
            {
                triggerBot = ENABLED;
                Features->TriggerBot = 1;
            }
            else
            {
                triggerBot = DISABLED;
                Features->TriggerBot = 0;
            }

            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, "WallHack", NK_LEFT);

            if (nk_option_label(ctx, "Enabled", wallHack == ENABLED))
            {
                wallHack = ENABLED;
                Features->Wallhack = 1;

            }

            else
            {
                wallHack = DISABLED;
                Features->Wallhack = 0;
            }

            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, "BHOP", NK_LEFT);

            if (nk_option_label(ctx, "Enabled", BHOP == ENABLED))
            {
                BHOP = ENABLED;
                Features->Bhop = 1;
            }
            else
            {
                BHOP = DISABLED;
                Features->Bhop = 0;
            }

            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, "Anti-Flash", NK_LEFT);

            if (nk_option_label(ctx, "Enabled", AntiFlash == ENABLED))
            {
                AntiFlash = ENABLED;
                Features->AntiFlash = 1;
            }
            else
            {
                AntiFlash = DISABLED;
                Features->AntiFlash = 0;
            }

            nk_label(ctx, "RadarHack", NK_LEFT);
            if (nk_option_label(ctx, "Enabled", Radar == ENABLED))
            {
                Radar = ENABLED;
                Features->RadarHack = 1;
            }
            else
            {
                Radar = DISABLED;
                Features->RadarHack= 0;
            }

            nk_label(ctx, "ESP", NK_LEFT);
            if (nk_option_label(ctx, "Enabled", ESP == ENABLED))
            {
                ESP = ENABLED;

                SDL_Rect rect = {.x = 50, .y = 50, .h = 50, .w = 50};

                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
                SDL_RenderDrawRect(renderer, &rect);
                
            }
            else
            {
                ESP = DISABLED;
                
            }

            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_button_label(ctx, "Close Cheat"))
                exit(0);

            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_button_label(ctx, "Unload The Driver"))
                Features->UNLOAD = TRUE;


        }
        nk_end(ctx);


        SDL_RenderClear(renderer);

        nk_sdl_render(NK_ANTI_ALIASING_ON);

        SDL_RenderPresent(renderer);
    }

cleanup:
    nk_sdl_shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}