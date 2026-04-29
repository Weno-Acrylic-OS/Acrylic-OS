// src/app/watchface.c
#include "lvgl.h"
#include <time.h>
#include <stdio.h>

static lv_obj_t * time_label;
static lv_obj_t * date_label;

static void update_watchface_time(lv_timer_t * timer)
{
    time_t now;
    time(&now);
    struct tm * timeinfo = localtime(&now);

    char time_buffer[9];
    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", timeinfo);
    lv_label_set_text(time_label, time_buffer);

    char date_buffer[16];
    strftime(date_buffer, sizeof(date_buffer), "%a %b %d", timeinfo);
    lv_label_set_text(date_label, date_buffer);
}

void create_watchface(lv_obj_t * parent)
{
    lv_obj_clean(parent);

    time_label = lv_label_create(parent);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -20);

    date_label = lv_label_create(parent);
    lv_obj_set_style_text_font(date_label, &lv_font_montserrat_24, 0);
    lv_obj_align(date_label, LV_ALIGN_CENTER, 0, 30);

    lv_timer_create(update_watchface_time, 1000, NULL);
}
