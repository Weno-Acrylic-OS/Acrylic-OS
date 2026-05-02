// src/app/phone.c
#include "lvgl.h"
#include "app_list.h"

static void back_to_app_list(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_app_list(parent);
}

void create_phone_app(lv_obj_t * parent)
{
    lv_obj_clean(parent);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * dial_btn = lv_btn_create(parent);
    lv_obj_align(dial_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t * dial_label = lv_label_create(dial_btn);
    lv_label_set_text(dial_label, LV_SYMBOL_CALL " Dial");
}