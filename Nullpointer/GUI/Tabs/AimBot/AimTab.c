#include "AimTab.h"

void _WaitKey(struct nk_context* ctx) {
	static int waitinForKey = 0;

	nk_layout_row_dynamic(ctx, 30, 1);
	//nk_label()

}