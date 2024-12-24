//[< -----NUKLEAR----->]
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

//[<-----INTERNAL DECLARATIONS----->]

#define MEMORY
#define ENTITY_STRUCTURE
#define CONFIG
#include "GUI.h"

//[<-----GLOBALS----->]

SDL_Renderer* renderer;
PEntity entities;
PConfig data;
GUIConfig conf = { .boxColor = {255, 255, 255, 255} };

//[<-----PRIVATE GUI METHODS----->]

unsigned int _RgbaToHex(int r, int g, int b, int a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

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

void _LoadIMG(PMemoryPool memoryRegion, const char* imagePath, SDL_Renderer* renderer, const char* name) {

    if (AllocatePool(memoryRegion, sizeof(Image), "StartRegion") == NULL) {
        printf("Run out of memory!");
        return;
    }

    memoryRegion->offset -= sizeof(Image);

    Image* newIcon = (Image*) AllocatePool(memoryRegion, sizeof(Image), name);

    newIcon->surface = IMG_Load(imagePath);
    newIcon->texture = SDL_CreateTextureFromSurface(renderer, newIcon->surface);
    newIcon->img = nk_image_ptr(newIcon->texture);

}

void _LoadIcons(PMemoryPool pool, SDL_Renderer* renderer) {
    const char* files[] = { "icons\\logo.png", "icons\\aimbot.png", "icons\\trigger.png", "icons\\glow.png", "icons\\esp.png", "icons\\misc.png" };
    const char* names[] = {"logo", "aim", "trig", "glow", "esp", "misc"};

    for (int i = 0; i < 6; i++) {
        _LoadIMG(pool, files[i], renderer, names[i]);
    }

}

PAppInfo _InitGUI() {
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

void _DrawESP(PFeaturesStates Features) {
    for (int i = 0; i < 64; i++) {

        if (entities[i].draw == 1 || entities[i].health <= 0 || entities[i].pawn == NULL)
            continue;

        SDL_Rect model = { .x = entities[i].position.x - 10, .y = entities[i].screenHead.y, .h = entities[i].height, .w = entities[i].width };

        float healthPrecantage = entities[i].health / 100.0f;
        float healthBarHeight = model.h * healthPrecantage;

        SDL_Rect health = {
            .x = model.x + model.w + 5,
            .y = model.y + (model.h - healthBarHeight),
            .w = 5,
            .h = healthBarHeight
        };


        if (Features->Box == 1) {

            struct nk_color pickedColorBox = nk_rgba_fv(&conf.boxColor.r);
            struct nk_color pickedColorZoom = nk_rgba_fv(&conf.zoomColor.r);
            struct nk_color pickedColorDefuse = nk_rgba_fv(&conf.defuseColor.r);

            if (!entities[i].isScoped && !entities[i].isDefusing) {

                SDL_SetRenderDrawColor(renderer, pickedColorBox.r, pickedColorBox.g, pickedColorBox.b, pickedColorBox.a);
            }

            else if (entities[i].isScoped)
                SDL_SetRenderDrawColor(renderer, pickedColorZoom.r, pickedColorZoom.g, pickedColorZoom.b, pickedColorZoom.a);

            else if (entities[i].isDefusing)
                SDL_SetRenderDrawColor(renderer, pickedColorDefuse.r, pickedColorZoom.g, pickedColorBox.b, pickedColorBox.a);

            SDL_RenderDrawRect(renderer, &model);
        }

        if (Features->Health) {
            struct nk_color pickedColor = nk_rgba_fv(&conf.hpColor.r);
            SDL_SetRenderDrawColor(renderer, pickedColor.r, pickedColor.g, pickedColor.b, pickedColor.a);
            SDL_RenderFillRect(renderer, &health);
        }

        if (Features->Line) {
            struct nk_color pickedColor = nk_rgba_fv(&conf.lineColor.r);
            SDL_SetRenderDrawColor(renderer, pickedColor.r, pickedColor.g, pickedColor.b, pickedColor.a);
            SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT, entities[i].position.x, entities[i].position.y);
        }


    }
}

void _ShowLogo(PMemoryPool pool, SDL_Renderer* renderer) {

    _LoadIcons(pool, renderer);
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
    _LoadIcons(pool, renderer);

}


//[<-----PUBLIC GUI METHODS----->]

void InitPointersGUI(PEntity array, PConfig config) {
    entities = array;
    data = config;
}

unsigned int GetGlowColor() {

    struct nk_color pickedColor = nk_rgba_fv(&conf.glowColor.r);

    return _RgbaToHex(pickedColor.r, pickedColor.g, pickedColor.b, pickedColor.a);
}

//[<-----GUI ELEMENTS METHODS----->]

void _showHPTree(struct nk_context* ctx, int* health, PFeaturesStates Features) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "HP", NK_MINIMIZED)) {

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_check_label(ctx, "Show HP", *health == ENABLED)) {
            *health = ENABLED;
            Features->Health = TRUE;

        }
        else {
            *health = DISABLED;
            Features->Health = FALSE;
        }

        nk_label(ctx, "Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf.hpColor = nk_color_picker(ctx, conf.hpColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showBoxTree(struct nk_context* ctx, int* box, PFeaturesStates Features) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Boxes", NK_MINIMIZED)) {

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_check_label(ctx, "Enable Boxes", *box == ENABLED)) {
            *box = ENABLED;
            Features->Box = TRUE;

        }
        else {
            *box = DISABLED;
            Features->Box = FALSE;
        }

        nk_label(ctx, "Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf.boxColor = nk_color_picker(ctx, conf.boxColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showLinesTree(struct nk_context* ctx, int* line, PFeaturesStates Features) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Lines", NK_MINIMIZED)) {

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_check_label(ctx, "Enable Lines", *line == ENABLED)) {
            *line = ENABLED;
            Features->Line = TRUE;

        }
        else {
            *line = DISABLED;
            Features->Line = FALSE;
        }

        nk_label(ctx, "Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf.lineColor = nk_color_picker(ctx, conf.lineColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showZoomColorTree(struct nk_context* ctx) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Change zoom color", NK_MINIMIZED)) {

        nk_label(ctx, "Zoom Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf.zoomColor = nk_color_picker(ctx, conf.zoomColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showDefuseColorTree(struct nk_context* ctx) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Change defuse color", NK_MINIMIZED)) {

        nk_label(ctx, "Defuse Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf.defuseColor = nk_color_picker(ctx, conf.defuseColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _warningPopup(struct nk_context* ctx, int* func, boolean* featureState, boolean* popupState) {
    //popup warning window
    if (nk_popup_begin(ctx, NK_POPUP_STATIC, "WARNING", NK_WINDOW_CLOSABLE, (struct nk_rect) { 150, 150, 500, 150 })) {
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_label(ctx, "This function WRITES into the memory, are you sure?", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_button_label(ctx, "Yes")) {
            *popupState = 0;
            *featureState = 1;
            nk_popup_close(ctx);
        }
        if (nk_button_label(ctx, "No")) {
            *func = DISABLED;
            nk_popup_close(ctx);
        }
    }
    nk_popup_end(ctx);
}

void _showGlowTab(struct nk_context* ctx, int* wallhack, PFeaturesStates Features) {

    static boolean showPopup = 1;

    nk_layout_row_dynamic(ctx, 30, 1);
    if (nk_check_label(ctx, "Enabled", *wallhack == ENABLED)) {
        *wallhack = ENABLED;

        if (showPopup) 
            _warningPopup(ctx, wallhack, &Features->Glow, &showPopup);    
    }

    else {
        showPopup = 1;
        *wallhack = DISABLED;
        Features->Glow = 0;
    }

    nk_label(ctx, "Glow Color: ", NK_TEXT_ALIGN_LEFT);
    nk_layout_row_static(ctx, 250, 250, 1);
    conf.glowColor = nk_color_picker(ctx, conf.glowColor, NK_RGBA);
}

void _AntiFlashCheck(struct nk_context* ctx, int* antiFlash, PFeaturesStates Features) {
    static boolean showPopupAnti = 1;

    if (nk_check_label(ctx, "AntiFlash", *antiFlash == ENABLED)) {
        *antiFlash = ENABLED;

        if (showPopupAnti)
            _warningPopup(ctx, antiFlash, &Features->AntiFlash, &showPopupAnti);
    }

    else {
        showPopupAnti = 1;
        *antiFlash = DISABLED;
        Features->AntiFlash = 0;
    }
}

void _showMiscTab(struct nk_context* ctx, int* antiFlash, int* radar, PFeaturesStates Features) {
    static boolean showPopup = 1;
    static boolean showPopupAnti = 1;

    nk_layout_row_dynamic(ctx, 30, 1);
    if (nk_check_label(ctx, "RadarHack", *radar == ENABLED)) {
        *radar = ENABLED;

        if (showPopup)
            _warningPopup(ctx, radar, &Features->RadarHack, &showPopup);
    }

    else {
        showPopup = 1;
        *radar = DISABLED;
        Features->RadarHack = 0;
    }

    _AntiFlashCheck(ctx, antiFlash, Features);
}

void _showTriggerTab(struct nk_context* ctx, int* triggerBot, PFeaturesStates Features) {

    nk_layout_row_dynamic(ctx, 50, 1);

    if (nk_check_label(ctx, "Enabled", *triggerBot == ENABLED)) {
        *triggerBot = ENABLED;
        Features->TriggerBot = 1;
    }

    else {
        *triggerBot = DISABLED;
        Features->TriggerBot = 0;
    }
}



//[<-----MAIN LOGIC----->]

void showWindow(PFeaturesStates Features) {
    int running = 1;
    static int activeTab = TAB_TRIGGERBOT;

    PAppInfo appInfo = _InitGUI();
    MemoryPool icons = { .offset = 0 };

    _ShowLogo(&icons, renderer);

    struct nk_image logo_img = ((Image*)(FindByPageName(&icons, "logo", sizeof(Image))))->img;
    struct nk_image aim_img = ((Image*)(FindByPageName(&icons, "aim", sizeof(Image))))->img;
    struct nk_image trig_img = ((Image*)(FindByPageName(&icons, "trig", sizeof(Image))))->img;
    struct nk_image glow_img = ((Image*)(FindByPageName(&icons, "glow", sizeof(Image))))->img;
    struct nk_image esp_img = ((Image*)(FindByPageName(&icons, "esp", sizeof(Image))))->img;
    struct nk_image misc_img = ((Image*)(FindByPageName(&icons, "misc", sizeof(Image))))->img;


    while (running)
    {
        /* Input */
        SDL_Event evt;

        nk_input_begin(appInfo->ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(appInfo->ctx);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        if (Features->poolLoaded) {

            if (Features->Box || Features->Health || Features->Line) 
                _DrawESP(Features);

            Features->poolLoaded = FALSE;
            
        }


        if (GetKeyState(VK_F9)) {

            SDL_ShowCursor(SDL_TRUE);
            SDL_SetRelativeMouseMode(SDL_FALSE);

            if (nk_begin(appInfo->ctx, "NullPointer - 9GHT", nk_rect(50, 50, 975, 500),
                NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_SCALABLE))
            {

                static enum State triggerBot, wallhack, bhop, antiFlash, radar, box, line, health = DISABLED;
                nk_menubar_begin(appInfo->ctx);

                nk_layout_row_static(appInfo->ctx, 30, 150, TAB_COUNT);
                //if (nk_button_image_label(appInfo->ctx, ((Image*)ReadFromPool(&icons, sizeof(Image), 0))->img,"VISUALS", NK_LEFT)) activeTab = TAB_VISUALS;
                if (nk_button_image_label(appInfo->ctx, trig_img, "AIMBOT", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_AIMBOT;
                if (nk_button_image_label(appInfo->ctx, aim_img, "TRIGGERBOT", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_TRIGGERBOT;
                if (nk_button_image_label(appInfo->ctx, glow_img, "GLOW", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_GLOW;
                if (nk_button_image_label(appInfo->ctx, esp_img, "ESP", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_ESP;
                if (nk_button_image_label(appInfo->ctx, misc_img, "MISC", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_MISC;
                if (nk_button_label(appInfo->ctx, "EXIT")) exit(0);

                nk_menubar_end(appInfo->ctx);

                if (activeTab == TAB_GLOW)
                    _showGlowTab(appInfo->ctx, &wallhack, Features);


                else if (activeTab == TAB_TRIGGERBOT)
                    _showTriggerTab(appInfo->ctx, &triggerBot, Features);

                else if (activeTab == TAB_ESP) {

                    nk_layout_row_dynamic(appInfo->ctx, 30, 1);
                    nk_label(appInfo->ctx, "Visual Settings", NK_TEXT_CENTERED);

                    _showLinesTree(appInfo->ctx, &line, Features);
                    _showBoxTree(appInfo->ctx, &box, Features);
                    _showHPTree(appInfo->ctx, &health, Features);
                    _showDefuseColorTree(appInfo->ctx);
                    _showZoomColorTree(appInfo->ctx);

                }

                else if (activeTab == TAB_MISC)
                    _showMiscTab(appInfo->ctx, &antiFlash, &radar, Features);
                
            }

            nk_end(appInfo->ctx);

        }

        else {
            SDL_ShowCursor(SDL_FALSE);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }

        nk_sdl_render(NK_ANTI_ALIASING_ON);
        SDL_RenderPresent(renderer);
    }

cleanup:
    nk_sdl_shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(appInfo->win);
    SDL_Quit();
}