// src/app/quick_settings.c
#include "lvgl.h"
#include "app/status_bar.h"

void create_quick_settings(lv_obj_t * parent)
{
    create_status_bar(parent);

    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x333333), 0);

    lv_obj_t * label = lv_label_create(panel);
    lv_label_set_text(label, "Quick Settings");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t * btn1 = lv_btn_create(panel);
    lv_obj_align(btn1, LV_ALIGN_CENTER, -60, 0);
    lv_obj_t * btn_label1 = lv_label_create(btn1);
    lv_label_set_text(btn_label1, "WiFi");

    lv_obj_t * btn2 = lv_btn_create(panel);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 60, 0);
    lv_obj_t * btn_label2 = lv_label_create(btn2);
    lv_label_set_text(btn_label2, "DND");
}
