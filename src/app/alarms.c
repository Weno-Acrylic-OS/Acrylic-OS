// src/app/alarms.c
#include "app/alarms.h"
#include <stdio.h>

static void add_alarm_event_handler(lv_event_t * e)
{
    lv_obj_t * mbox = lv_msgbox_create(NULL, "Add Alarm", "This feature is coming soon!", NULL, true);
    lv_obj_center(mbox);
}

void create_alarms_app(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 10, 0);

    lv_obj_t * title = lv_label_create(panel);
    lv_label_set_text(title, "Alarms");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t * list = lv_list_create(panel);
    lv_obj_set_size(list, lv_pct(100), lv_pct(80));
    lv_obj_align(list, LV_ALIGN_CENTER, 0, 20);

    lv_list_add_text(list, "No alarms yet");

    lv_obj_t * add_btn = lv_btn_create(panel);
    lv_obj_align(add_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(add_btn, add_alarm_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * add_label = lv_label_create(add_btn);
    lv_label_set_text(add_label, LV_SYMBOL_PLUS " Add Alarm");
}
