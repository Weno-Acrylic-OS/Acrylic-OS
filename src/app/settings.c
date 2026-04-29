// src/app/settings.c
#include "lvgl.h"
#include "app_list.h"

static void back_to_app_list(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_app_list(parent);
}

void create_settings_app(lv_obj_t * parent)
{
    lv_obj_clean(parent);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(80));
    lv_obj_center(list);

    lv_list_add_text(list, "Settings");
    lv_list_add_btn(list, LV_SYMBOL_WIFI, "Bluetooth");
    lv_list_add_btn(list, LV_SYMBOL_IMAGE, "Display");
}
