#include "MiscTab.h"

void _AntiFlashCheck(struct nk_context* ctx, int* antiFlash, PFeaturesStates Features) {
    static boolean showPopupAnti = 1;

    if (nk_check_label(ctx, "AntiFlash", *antiFlash == 1)) {
        *antiFlash = 1;

        if (showPopupAnti)
            WarningPopup(ctx, antiFlash, &Features->AntiFlash, &showPopupAnti);
    }

    else {
        showPopupAnti = 1;
        *antiFlash = 0;
        Features->AntiFlash = 0;
    }
}

void _DelaySleep(struct nk_context* ctx, PConfig conf) {
    static char buffer[8];

    nk_layout_row_dynamic(ctx, 30, 3);

    nk_label(ctx, "Sleep Value", NK_TEXT_ALIGN_LEFT);
    nk_slider_int(ctx, 1, &conf->sleepTime, 180, 1);

    snprintf(buffer, sizeof(buffer), "%i", conf->sleepTime);
    nk_label(ctx, buffer, NK_TEXT_ALIGN_RIGHT);
}

void _RadarHackCheck(struct nk_context* ctx, int* radar, PFeaturesStates Features) {
    static boolean showPopup = 1;

    nk_layout_row_dynamic(ctx, 30, 1);
    if (nk_check_label(ctx, "RadarHack", *radar == 1)) {
        *radar = 1;

        if (showPopup)
            WarningPopup(ctx, radar, &Features->RadarHack, &showPopup);
    }

    else {
        showPopup = 1;
        *radar = 0;
        Features->RadarHack = 0;
    }
}

void _SleepTime(struct nk_context* ctx, PConfig config) {
    static int sleep = 0;

    if (nk_check_label(ctx, "Sleep Time", sleep == 1)) {
        sleep = 1;
        _DelaySleep(ctx, config);
    }

    else {
        config->sleepTime = 0;
        sleep = 0;
    }
}

void ShowMiscTab(struct nk_context* ctx, int* antiFlash, int* radar, PFeaturesStates Features, PConfig config) {

    _SleepTime(ctx, config);
    _RadarHackCheck(ctx, radar, Features);
    _AntiFlashCheck(ctx, antiFlash, Features);
}