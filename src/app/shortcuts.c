// src/app/shortcuts.c
#include "app/shortcuts.h"

void create_shortcuts_menu(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x02F7BE), 0);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 20, 0);

    lv_obj_t * title = lv_label_create(panel);
    lv_label_set_text(title, "Shortcuts");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_pad_bottom(title, 20, 0);
    lv_obj_set_width(title, lv_pct(100));
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t * btn_payment = lv_btn_create(panel);
    lv_obj_set_width(btn_payment, lv_pct(100));
    lv_obj_t * label_payment = lv_label_create(btn_payment);
    lv_label_set_text(label_payment, "Weno Pay");

    lv_obj_t * btn_music = lv_btn_create(panel);
    lv_obj_set_width(btn_music, lv_pct(100));
    lv_obj_t * label_music = lv_label_create(btn_music);
    lv_label_set_text(label_music, "Music");

    lv_obj_t * btn_messages = lv_btn_create(panel);
    lv_obj_set_width(btn_messages, lv_pct(100));
    lv_obj_t * label_messages = lv_label_create(btn_messages);
    lv_label_set_text(label_messages, "Messages");

    lv_obj_t * btn_settings = lv_btn_create(panel);
    lv_obj_set_width(btn_settings, lv_pct(100));
    lv_obj_t * label_settings = lv_label_create(btn_settings);
    lv_label_set_text(label_settings, "Settings");
}
