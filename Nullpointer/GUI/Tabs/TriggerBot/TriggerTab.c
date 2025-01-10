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

        nk_property_int(ctx, "Delay between shots (MilliSeconds)", 1, &config->sleepTriggerTime, 10000, 5, 5);
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