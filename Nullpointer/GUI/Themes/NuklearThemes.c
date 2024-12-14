#include "NuklearThemes.h"

void apply_9ght_style(struct nk_context* ctx) {
    struct nk_color black = nk_rgb(10, 10, 10);
    struct nk_color dark_gray = nk_rgb(20, 20, 20);
    struct nk_color purple_glow = nk_rgb(120, 40, 150);
    struct nk_color cyan_glow = nk_rgb(50, 200, 220);
    struct nk_color text_color = nk_rgb(230, 230, 230);

    struct nk_style_window* win = &ctx->style.window;
    win->background = black;
    win->fixed_background = nk_style_item_color(dark_gray);
    win->border_color = purple_glow;
    win->padding = nk_vec2(15, 15);
    win->border = 2.0f;
    win->rounding = 12.0f; // Закругленные углы
    win->header.align = NK_HEADER_RIGHT;


    struct nk_style_button* btn = &ctx->style.button;
    btn->normal = nk_style_item_color(dark_gray);
    btn->hover = nk_style_item_color(purple_glow);
    btn->active = nk_style_item_color(cyan_glow);
    btn->border_color = purple_glow;
    btn->text_background = black;
    btn->text_normal = text_color;
    btn->text_hover = text_color;
    btn->text_active = black;
    btn->rounding = 8.0f;
    btn->border = 1.5f;
    btn->image_padding = nk_vec2(2, 2);

    struct nk_style_toggle* checkbox = &ctx->style.checkbox;
    checkbox->normal = nk_style_item_color(black);
    checkbox->hover = nk_style_item_color(purple_glow);
    checkbox->active = nk_style_item_color(cyan_glow);
    checkbox->cursor_normal = nk_style_item_color(purple_glow);
    checkbox->cursor_hover = nk_style_item_color(cyan_glow);
    checkbox->text_normal = text_color;
    checkbox->text_hover = cyan_glow;


    struct nk_style_edit* edit = &ctx->style.edit;
    edit->normal = nk_style_item_color(black);
    edit->hover = nk_style_item_color(dark_gray);
    edit->active = nk_style_item_color(purple_glow);
    edit->border_color = cyan_glow;
    edit->text_normal = text_color;
    edit->text_hover = cyan_glow;
    edit->text_active = purple_glow;
    edit->rounding = 6.0f;


    struct nk_style_slider* slider = &ctx->style.slider;
    slider->bar_normal = black;
    slider->bar_hover = purple_glow;
    slider->bar_active = cyan_glow;
    slider->cursor_normal = nk_style_item_color(purple_glow);
    slider->cursor_hover = nk_style_item_color(cyan_glow);
    slider->cursor_active = nk_style_item_color(cyan_glow);
    slider->rounding = 8.0f;

    struct nk_style_window_header* header = &ctx->style.window.header;
    header->normal = nk_style_item_color(black);
    header->hover = nk_style_item_color(purple_glow);
    header->active = nk_style_item_color(cyan_glow);
    header->label_normal = text_color;
    header->label_hover = cyan_glow;
    header->label_active = black;
}