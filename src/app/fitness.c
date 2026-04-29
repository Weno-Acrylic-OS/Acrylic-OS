// src/app/fitness.c
#include "lvgl.h"
#include "app_list.h"

static void back_to_app_list(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_app_list(parent);
}

void create_fitness_app(lv_obj_t * parent)
{
    lv_obj_clean(parent);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * hr_label = lv_label_create(parent);
    lv_label_set_text(hr_label, "Heart Rate");
    lv_obj_align(hr_label, LV_ALIGN_TOP_MID, 0, 50);

    lv_obj_t * hr_value = lv_label_create(parent);
    lv_label_set_text(hr_value, "85 BPM");
    lv_obj_set_style_text_font(hr_value, &lv_font_montserrat_48, 0);
    lv_obj_align(hr_value, LV_ALIGN_TOP_MID, 0, 80);

    lv_obj_t * steps_label = lv_label_create(parent);
    lv_label_set_text(steps_label, "Steps");
    lv_obj_align(steps_label, LV_ALIGN_TOP_MID, 0, 180);

    lv_obj_t * steps_value = lv_label_create(parent);
    lv_label_set_text(steps_value, "10,523");
    lv_obj_set_style_text_font(steps_value, &lv_font_montserrat_48, 0);
    lv_obj_align(steps_value, LV_ALIGN_TOP_MID, 0, 210);
}
