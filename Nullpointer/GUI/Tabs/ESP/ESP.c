#include "ESP.h"

void _showTeamBoxTree(struct nk_context* ctx, PGUIConfig conf) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Team Box Color", NK_MINIMIZED)) {

        nk_label(ctx, "Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf->teamBoxColor = nk_color_picker(ctx, conf->teamBoxColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _teamCheckESP(struct nk_context* ctx, int* espTeamCheckState, PFeaturesStates Features, PConfig conf) {

    if (nk_check_label(ctx, "Team Check", *espTeamCheckState == 1))
    {
        *espTeamCheckState = 1;
        Features->ESPTeamCheck = TRUE;
        _showTeamBoxTree(ctx, conf);
    }

    else
    {
        *espTeamCheckState = 0;
        Features->ESPTeamCheck = FALSE;
    }
}

void _showLinesTree(struct nk_context* ctx, int* line, PFeaturesStates Features, PGUIConfig conf) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Lines", NK_MINIMIZED)) {

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_check_label(ctx, "Enable Lines", *line == 1)) {
            *line = 1;
            Features->Line = TRUE;

        }
        else {
            *line = 0;
            Features->Line = FALSE;
        }

        nk_label(ctx, "Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf->lineColor = nk_color_picker(ctx, conf->lineColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showBoxTree(struct nk_context* ctx, int* box, PFeaturesStates Features, PGUIConfig conf) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Boxes", NK_MINIMIZED)) {

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_check_label(ctx, "Enable Boxes", *box == 1)) {
            *box = 1;
            Features->Box = TRUE;

        }
        else {
            *box = 0;
            Features->Box = FALSE;
        }

        nk_label(ctx, "Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf->boxColor = nk_color_picker(ctx, conf->boxColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showHPTree(struct nk_context* ctx, int* health, PFeaturesStates Features, PGUIConfig conf) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "HP", NK_MINIMIZED)) {

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_check_label(ctx, "Show HP", *health == 1)) {
            *health = 1;
            Features->Health = TRUE;

        }
        else {
            *health = 0;
            Features->Health = FALSE;
        }

        nk_label(ctx, "Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf->hpColor = nk_color_picker(ctx, conf->hpColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showZoomColorTree(struct nk_context* ctx, PGUIConfig conf) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Change zoom color", NK_MINIMIZED)) {

        nk_label(ctx, "Zoom Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf->zoomColor = nk_color_picker(ctx, conf->zoomColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void _showDefuseColorTree(struct nk_context* ctx, PGUIConfig conf) {
    if (nk_tree_push(ctx, NK_TREE_TAB, "Change defuse color", NK_MINIMIZED)) {

        nk_label(ctx, "Defuse Color: ", NK_TEXT_ALIGN_LEFT);
        nk_layout_row_static(ctx, 250, 250, 1);
        conf->defuseColor = nk_color_picker(ctx, conf->defuseColor, NK_RGBA);

        nk_tree_pop(ctx);
    }
}

void ShowESPTab(struct nk_context* ctx, int* espTeamCheckState, int* line, int* box, int* health, PFeaturesStates Features, PGUIConfig conf) {

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Visual Settings", NK_TEXT_CENTERED);

    _teamCheckESP(ctx, espTeamCheckState, Features, conf);
    _showLinesTree(ctx, line, Features, conf);
    _showBoxTree(ctx, box, Features, conf);
    _showHPTree(ctx, health, Features, conf);
    _showZoomColorTree(ctx, conf);
    _showDefuseColorTree(ctx, conf);
}