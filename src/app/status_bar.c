// src/app/status_bar.c
#include "app/status_bar.h"

static lv_obj_t * time_label;

void create_status_bar(lv_obj_t * parent)
{
    lv_obj_t * status_bar = lv_obj_create(parent);
    lv_obj_set_size(status_bar, lv_pct(100), 30);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(status_bar, 0, 0);
    lv_obj_set_style_radius(status_bar, 0, 0);

    time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "12:00");
    lv_obj_set_style_text_color(time_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t * battery_label = lv_label_create(status_bar);
    lv_label_set_text(battery_label, "100%");
    lv_obj_set_style_text_color(battery_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(battery_label, LV_ALIGN_RIGHT_MID, -10, 0);
}

void update_time_label(const char * time_text)
{
    if (time_label) {
        lv_label_set_text(time_label, time_text);
    }
}
