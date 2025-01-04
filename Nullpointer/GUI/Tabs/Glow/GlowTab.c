#include "GlowTab.h"

void ShowGlowTab(struct nk_context* ctx, int* wallhack, PFeaturesStates Features, PGUIConfig conf) {

    static boolean showPopup = 1;

    nk_layout_row_dynamic(ctx, 30, 1);
    if (nk_check_label(ctx, "Enabled", *wallhack == 1)) {
        *wallhack = 1;

        if (showPopup)
            WarningPopup(ctx, wallhack, &Features->Glow, &showPopup);
    }

    else {
        showPopup = 1;
        *wallhack = 0;
        Features->Glow = 0;
    }

    nk_label(ctx, "Glow Color: ", NK_TEXT_ALIGN_LEFT);
    nk_layout_row_static(ctx, 250, 250, 1);
    conf->glowColor = nk_color_picker(ctx, conf->glowColor, NK_RGBA);
}
