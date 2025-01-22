//[< -----NUKLEAR----->]

//[<-----INTERNAL DECLARATIONS----->]
#define ENTITY_STRUCTURE
#define INTERNAL_GUI
#define UTILS
#include "GUI.h"

//[<-----GLOBALS----->]
PAppInfo appInfo;
PEntity entities;
PConfig dataGUI;
GUIConfig conf = { .boxColor = {255, 255, 255, 255} };

static boolean showMenu = FALSE;
static enum nk_panel_flags flags;

//[<-----PRIVATE GUI METHODS----->]

unsigned int _RgbaToHex(int r, int g, int b, int a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

void _DrawESP(PFeaturesStates Features) {
    for (int i = 0; i < 64; i++) {

        if (entities[i].draw == 1 || entities[i].health <= 0 || entities[i].pawn == NULL)
            continue;

        SDL_Rect model = { .x = entities[i].screenHead.x, .y = entities[i].screenHead.y, .h = entities[i].height, .w = entities[i].width };

        float healthPrecantage = entities[i].health / 100.0f;
        float healthBarHeight = model.h * healthPrecantage;

        SDL_Rect health = {
            .x = model.x + model.w + 5,
            .y = model.y + (model.h - healthBarHeight),
            .w = 5,
            .h = healthBarHeight
        };


        if (Features->Box || Features->ESPTeamCheck) {

            struct nk_color pickedColorBox = nk_rgba_fv(&conf.boxColor.r);
            struct nk_color pickedTeamColorBox = nk_rgba_fv(&conf.teamBoxColor.r);
            struct nk_color pickedColorZoom = nk_rgba_fv(&conf.zoomColor.r);
            struct nk_color pickedColorDefuse = nk_rgba_fv(&conf.defuseColor.r);

            if (!entities[i].isScoped && !entities[i].isDefusing) {
                if (!entities[i].isAlly && Features->Box)
                    SDL_SetRenderDrawColor(appInfo->render, pickedColorBox.r, pickedColorBox.g, pickedColorBox.b, pickedColorBox.a);

                else if (Features->ESPTeamCheck && entities[i].isAlly)
                    SDL_SetRenderDrawColor(appInfo->render, pickedTeamColorBox.r, pickedTeamColorBox.g, pickedTeamColorBox.b, pickedTeamColorBox.a);
            }

            else if (entities[i].isScoped)
                SDL_SetRenderDrawColor(appInfo->render, pickedColorZoom.r, pickedColorZoom.g, pickedColorZoom.b, pickedColorZoom.a);

            else if (entities[i].isDefusing)
                SDL_SetRenderDrawColor(appInfo->render, pickedColorDefuse.r, pickedColorZoom.g, pickedColorBox.b, pickedColorBox.a);

            SDL_RenderDrawRect(appInfo->render, &model);
        }

        if (Features->Health) {
            struct nk_color pickedColor = nk_rgba_fv(&conf.hpColor.r);
            SDL_SetRenderDrawColor(appInfo->render, pickedColor.r, pickedColor.g, pickedColor.b, pickedColor.a);
            SDL_RenderFillRect(appInfo->render, &health);
        }

        if (Features->Line) {
            struct nk_color pickedColor = nk_rgba_fv(&conf.lineColor.r);
            SDL_SetRenderDrawColor(appInfo->render, pickedColor.r, pickedColor.g, pickedColor.b, pickedColor.a);
            SDL_RenderDrawLine(appInfo->render, WINDOW_WIDTH / 2, WINDOW_HEIGHT, entities[i].position.x, entities[i].position.y);
        }



    }
}

//[<-----PUBLIC GUI METHODS----->]

void InitPointersGUI(PEntity array, PConfig config) {
    entities = array;
    dataGUI = config;
}

unsigned int GetGlowColor() {

    struct nk_color pickedColor = nk_rgba_fv(&conf.glowColor.r);
    return _RgbaToHex(pickedColor.r, pickedColor.g, pickedColor.b, pickedColor.a);
}

void HandleF9Key() {

    if (GetKeyState(VK_F9)) {
        if (showMenu == FALSE) {
            showMenu = TRUE;
            SetWindowTransparency(appInfo->win, FALSE);
        }
    }
    else {
        showMenu = FALSE;
        SetWindowTransparency(appInfo->win, TRUE);
    }
}

//[<-----MAIN LOGIC----->]

void showWindow(PFeaturesStates Features) {
    int running = 1;
    static boolean f9 = FALSE;
    static int activeTab = TAB_TRIGGERBOT;

    appInfo = InitGUI();
    MemoryPool icons = { .offset = 0 };

    ShowLogo(&icons);

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
        
        SDL_SetRenderDrawColor(appInfo->render, 0, 0, 0, 0);
        SDL_RenderClear(appInfo->render);

        if (Features->FOV == TRUE) {
            circleRGBA(appInfo->render, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, dataGUI->fovSize, 255, 255, 255, 255);
        }

        if (Features->poolLoaded) {

            if (Features->Box || Features->Health || Features->Line) 
                _DrawESP(Features);

            Features->poolLoaded = FALSE;
            
        }

        HandleF9Key();

        if (showMenu == TRUE)
            flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_SCALABLE | NK_WINDOW_BACKGROUND;

        else
            flags = NK_WINDOW_HIDDEN;

        if (nk_begin(appInfo->ctx, "NullPointer - 9GHT", nk_rect(50, 50, 975, 500), flags))
            {

                static enum State triggerBot, wallhack, bhop, antiFlash, radar, box, 
                    line, health, espTeamCheck, triggerTeamCheck, aimbot, fov = DISABLED;

                nk_menubar_begin(appInfo->ctx);

                nk_layout_row_static(appInfo->ctx, 30, 150, TAB_COUNT);

                if (nk_button_image_label(appInfo->ctx, trig_img, "AIMBOT", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_AIMBOT;
                if (nk_button_image_label(appInfo->ctx, aim_img, "TRIGGERBOT", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_TRIGGERBOT;
                if (nk_button_image_label(appInfo->ctx, glow_img, "GLOW", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_GLOW;
                if (nk_button_image_label(appInfo->ctx, esp_img, "ESP", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_ESP;
                if (nk_button_image_label(appInfo->ctx, misc_img, "MISC", NK_TEXT_ALIGN_CENTERED)) activeTab = TAB_MISC;
                if (nk_button_label(appInfo->ctx, "EXIT")) exit(0);

                nk_menubar_end(appInfo->ctx);

                if (activeTab == TAB_GLOW)
                    ShowGlowTab(appInfo->ctx, &wallhack, Features, &conf);

                else if (activeTab == TAB_TRIGGERBOT)
                    ShowTriggerTab(appInfo->ctx, &triggerBot, &triggerTeamCheck, Features, dataGUI);

                else if (activeTab == TAB_ESP)
                    ShowESPTab(appInfo->ctx, &espTeamCheck, &line, &box, &health, Features, &conf);

                else if (activeTab == TAB_MISC)
                    ShowMiscTab(appInfo->ctx, &antiFlash, &radar, Features, dataGUI);

                else if (activeTab == TAB_AIMBOT)
                    ShowAimTab(appInfo->ctx, &aimbot, &fov, Features, dataGUI, &conf);
                
            }

            nk_end(appInfo->ctx);

        nk_sdl_render(NK_ANTI_ALIASING_ON);
        SDL_RenderPresent(appInfo->render);
    }

cleanup:
    nk_sdl_shutdown();
    SDL_DestroyRenderer(appInfo->render);
    SDL_DestroyWindow(appInfo->win);
    SDL_Quit();
}