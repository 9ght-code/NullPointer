#include "TriggerTab.h"


void _TeamCheckBox(struct nk_context* ctx, int* teamCheck, PFeaturesStates Features) {

    if (nk_check_label(ctx, "Team Check", *teamCheck == 1)) {
        *teamCheck = 1;
        Features->TriggerTeamCheck = TRUE;
    }

    else {
        *teamCheck = 0;
        Features->TriggerTeamCheck = FALSE;
    }
}

void _KeyComboBox(struct nk_context* ctx, PConfig config) {

    if (nk_combo_begin_label(ctx, config->keyEntry.keys[config->keyEntry.selectedKeyTrigger].name, nk_vec2(200, 300))) {
        nk_layout_row_dynamic(ctx, 25, 1);

        for (int i = 0; i <= 6; i++) {
            if (nk_combo_item_label(ctx, config->keyEntry.keys[i].name, NK_TEXT_LEFT)) {
                config->keyEntry.selectedKeyTrigger = i;
                nk_combo_close(ctx);
            }
        }


        nk_combo_end(ctx);
    }
}

void _DrawTriggerMode(struct nk_context* ctx, PConfig config) {
    static enum Mode currentMode = HOLD;

    nk_layout_row_dynamic(ctx, 30, 2);
    if (nk_option_label(ctx, "Hold mode", currentMode == HOLD)) {
        config->triggerMode = HOLD;
        currentMode = HOLD;

    }
    if (nk_option_label(ctx, "Press mode", currentMode == PRESS)) {
        config->triggerMode = PRESS;
        currentMode = PRESS;
    }

}

void _PreDelaySlider(struct nk_context* ctx, PConfig conf) {
    static char buffer[8];

    nk_layout_row_dynamic(ctx, 30, 3);

    nk_label(ctx, "Pre Shot Value (ms)", NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &conf->preShotDelay, 500, 1);

    snprintf(buffer, sizeof(buffer), "%i", conf->preShotDelay);
    nk_label(ctx, buffer, NK_TEXT_ALIGN_RIGHT);
}

void _PostDelaySlider(struct nk_context* ctx, PConfig conf) {
    static char buffer[8];

    nk_layout_row_dynamic(ctx, 30, 3);

    nk_label(ctx, "Post Shot Value (ms)", NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &conf->postShotDelay, 500, 1);

    snprintf(buffer, sizeof(buffer), "%i", conf->postShotDelay);
    nk_label(ctx, buffer, NK_TEXT_ALIGN_RIGHT);
}


void _DelaySlider(struct nk_context*ctx, PConfig conf) {
    static char buffer[8];

    nk_layout_row_dynamic(ctx, 30, 3);

    nk_label(ctx, "Delay Value between clicking (ms)", NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &conf->sleepTriggerTime, 500, 1);

    snprintf(buffer, sizeof(buffer), "%i", conf->sleepTriggerTime);
    nk_label(ctx, buffer, NK_TEXT_ALIGN_RIGHT);
}

void _ShootInAirCheck(struct nk_context* ctx, PConfig config) {

    static int shoot = 0;

    if (nk_check_label(ctx, "Shoot In Air", shoot == 1)) {
        shoot = 1;
        config->shootInAir = TRUE;
    }

    else {
        shoot = 0;
        config->shootInAir = FALSE;
    }
}

void _EnabledCheckBox(struct nk_context* ctx, int* triggerBot, PFeaturesStates Features, PConfig config) {

    if (nk_check_label(ctx, "Enabled", *triggerBot == 1)) {
        *triggerBot = 1;
        Features->TriggerBot = 1;

        _DrawTriggerMode(ctx, config);
        _DelaySlider(ctx, config);
        _PreDelaySlider(ctx, config);
        _PostDelaySlider(ctx, config);
        _KeyComboBox(ctx, config);
        //_ShootInAirCheck(ctx, config);
    }

    else {
        *triggerBot = 0;
        Features->TriggerBot = 0;
    }
}

void ShowTriggerTab(struct nk_context* ctx, int* triggerBot, int* teamCheck, PFeaturesStates Features, PConfig config) {

    nk_layout_row_dynamic(ctx, 50, 1);

    _TeamCheckBox(ctx, teamCheck, Features);
    _EnabledCheckBox(ctx, triggerBot, Features, config);
}