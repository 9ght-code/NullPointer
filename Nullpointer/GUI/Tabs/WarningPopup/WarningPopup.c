#include "WarningPopup.h"

void WarningPopup(struct nk_context* ctx, int* func, boolean* featureState, boolean* popupState) {
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
            *func = 0;
            nk_popup_close(ctx);
        }
    }
    nk_popup_end(ctx);
}