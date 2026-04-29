// src/app/clock.c
#include "lvgl.h"
#include <time.h>
#include <stdio.h>

#include "app_list.h"

static lv_obj_t * time_label;

static void back_to_app_list(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_app_list(parent);
}

static void update_time(lv_timer_t * timer)
{
    time_t now;
    time(&now);
    struct tm * timeinfo = localtime(&now);

    char buffer[9];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    lv_label_set_text(time_label, buffer);
}

void create_clock_app(lv_obj_t * parent)
{
    lv_obj_clean(parent);

    time_label = lv_label_create(parent);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
    lv_obj_center(time_label);

    lv_timer_create(update_time, 1000, NULL);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");
}
