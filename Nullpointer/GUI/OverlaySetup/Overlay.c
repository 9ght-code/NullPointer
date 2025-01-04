#define NUKLEAR_THEMES
#define INTERNAL_GUI
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#define MEMORY
#define ENTITY_STRUCTURE
#define CONFIG
#include "Overlay.h"

//[<-----GLOBALS----->]
SDL_Renderer* renderer;

void _SetWindowTransparency(SDL_Window* window) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

    HWND hwnd = wmInfo.info.win.window;
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}

void _SetupSDL(PAppInfo info) {
    SDL_Window* win;

    int flags = 0;

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("NightFall",
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

    _SetWindowTransparency(win);
    SDL_ShowCursor(SDL_FALSE);
    SDL_SetWindowGrab(win, SDL_FALSE);

    info->render = renderer;
    info->win = win;

}

void _LoadIMG(PMemoryPool memoryRegion, const char* imagePath, const char* name) {

    if (AllocatePool(memoryRegion, sizeof(Image), "StartRegion") == NULL) {
        printf("Run out of memory!");
        return;
    }

    memoryRegion->offset -= sizeof(Image);

    Image* newIcon = (Image*)AllocatePool(memoryRegion, sizeof(Image), name);

    newIcon->surface = IMG_Load(imagePath);
    newIcon->texture = SDL_CreateTextureFromSurface(renderer, newIcon->surface);
    newIcon->img = nk_image_ptr(newIcon->texture);

}

void _LoadIcons(PMemoryPool pool) {
    const char* files[] = { "icons\\logo.png", "icons\\aimbot.png", "icons\\trigger.png", "icons\\glow.png", "icons\\esp.png", "icons\\misc.png" };
    const char* names[] = { "logo", "aim", "trig", "glow", "esp", "misc" };

    for (int i = 0; i < 6; i++) {
        _LoadIMG(pool, files[i], names[i]);
    }

}

PAppInfo InitGUI() {
    /* Platform */
    static AppInfo info = { 0 };

    float font_scale = 1;
    struct nk_context* ctx;

    _SetupSDL(&info);

    /* scale the renderer output for High-DPI displays */
    {
        int render_w, render_h;
        int window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(info.render, &render_w, &render_h);
        SDL_GetWindowSize(info.win, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(info.render, scale_x, scale_y);
        font_scale = scale_y;
    }

    ctx = nk_sdl_init(info.win, info.render);

    {
        struct nk_font_atlas* atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font* font;

        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_from_file(atlas, "font\\FiraCode.ttf", 16 * font_scale, &config);
        nk_sdl_font_stash_end();

        font->handle.height /= font_scale;

        nk_style_set_font(ctx, &font->handle);
    }

    apply_9ght_style(ctx);

    info.ctx = ctx;

    return &info;
}

void ShowLogo(PMemoryPool pool) {

    _LoadIcons(pool);
    SDL_Texture* texture = ((Image*)ReadFromPool(pool, 0, 0))->texture;


    UINT32 startTime = SDL_GetTicks();
    BOOLEAN show = TRUE;
    UINT8 alpha = 255;

    while (show) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                show = FALSE;
        }


        SDL_RenderClear(renderer);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        if (elapsedTime > 2000) {
            if (alpha > 0) {
                alpha -= 5;
                if (alpha < 0) alpha = 0;
            }
            else
                show = FALSE;
        }

        SDL_Rect dstRect = {
            .x = (WINDOW_WIDTH - 900) / 2,
            .y = (WINDOW_HEIGHT - 600) / 2,
            .w = 900,
            .h = 600
        };

        SDL_SetTextureAlphaMod(texture, alpha);
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_RenderPresent(renderer);

        if (SDL_GetTicks() - startTime > 3000)
            show = FALSE;
    }

    SDL_DestroyTexture(texture);
    ResetPool(pool);
    _LoadIcons(pool);

}