#define UTILS
#include "AimTab.h"


void _DrawMode(struct nk_context* ctx, PConfig config) {
    static enum Mode currentMode = HOLD;

    nk_layout_row_dynamic(ctx, 30, 2);
    if (nk_option_label(ctx, "Hold mode", currentMode == HOLD)) {
        config->aimbotMode = HOLD;
        currentMode = HOLD;

    }
    if (nk_option_label(ctx, "Press mode", currentMode == PRESS)) {
        config->aimbotMode = PRESS;
        currentMode = PRESS;
    }

}

void _DrawKeyBind(struct nk_context* ctx, PConfig config) {

    if (nk_combo_begin_label(ctx, config->keyEntry.keys[config->keyEntry.selectedKeyAim].name, nk_vec2(200, 300))) {
        nk_layout_row_dynamic(ctx, 25, 1);

        for (int i = 0; i <= 6; i++) {
            if (nk_combo_item_label(ctx, config->keyEntry.keys[i].name, NK_TEXT_LEFT)) {
                config->keyEntry.selectedKeyAim = i;
                nk_combo_close(ctx);
            }
        }


        nk_combo_end(ctx);
    }
}

void _showFOV(struct nk_context* ctx, PConfig conf) {
    static char buffer[8];

    nk_layout_row_dynamic(ctx, 30, 3);

    nk_label(ctx, "FOV Value", NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &conf->fovSize, 180, 1);

    snprintf(buffer, sizeof(buffer), "%i", conf->fovSize);
    nk_label(ctx, buffer, NK_TEXT_ALIGN_RIGHT);
}

void _FOVCheck(struct nk_context* ctx, int* fov, PConfig conf, PFeaturesStates Features) {
    if (nk_check_label(ctx, "FOV", *fov == 1))
    {
        *fov = 1;
        Features->FOV = TRUE;
        _showFOV(ctx, conf);

    }

    else
    {
        *fov = 0;
        Features->FOV = FALSE;
    }
}

void _EnabledCheck(struct nk_context* ctx, int* aimbot, int* fov, PFeaturesStates Features, PConfig config) {

    if (nk_check_label(ctx, "Enabled", *aimbot == 1))
    {
        *aimbot = 1;
        Features->Aimbot = TRUE;
        _DrawKeyBind(ctx, config);
        _DrawMode(ctx, config);
        _FOVCheck(ctx, fov, config, Features);

    }

    else
    {
        *aimbot = 0;
        Features->Aimbot = FALSE;
    }

}

void ShowAimTab(struct nk_context* ctx, int* aimbot, int* fov, PFeaturesStates Features, PConfig config, PGUIConfig gconf) {
    nk_layout_row_dynamic(ctx, 30, 1);

    _EnabledCheck(ctx, aimbot, fov, Features, config);
}