// src/app/stopwatch.c
#include "app/stopwatch.h"
#include <stdio.h>

static lv_obj_t * time_label;
static lv_timer_t * stopwatch_timer;
static int32_t time_ms;
static bool running;

static void stopwatch_timer_cb(lv_timer_t * timer)
{
    time_ms += (int32_t)(uintptr_t)timer->user_data;

    int32_t seconds = time_ms / 1000;
    int32_t minutes = seconds / 60;
    int32_t hours = minutes / 60;

    char time_buffer[12];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d:%02d.%02d",
             hours, minutes % 60, seconds % 60, (time_ms % 1000) / 10);
    lv_label_set_text(time_label, time_buffer);
}

static void start_stop_event_handler(lv_event_t * e)
{
    if (running) {
        lv_timer_pause(stopwatch_timer);
        running = false;
        lv_obj_t * label = lv_obj_get_child(lv_event_get_target(e), 0);
        lv_label_set_text(label, "Start");
    } else {
        lv_timer_resume(stopwatch_timer);
        running = true;
        lv_obj_t * label = lv_obj_get_child(lv_event_get_target(e), 0);
        lv_label_set_text(label, "Stop");
    }
}

static void reset_event_handler(lv_event_t * e)
{
    time_ms = 0;
    lv_label_set_text(time_label, "00:00:00.00");
    if (running) {
        lv_timer_pause(stopwatch_timer);
        running = false;
        // Also reset the start/stop button label
        lv_obj_t * btn = (lv_obj_t *)lv_event_get_user_data(e);
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text(label, "Start");
    }
}


void create_stopwatch_app(lv_obj_t * parent)
{
    time_ms = 0;
    running = false;

    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 20, 0);

    time_label = lv_label_create(panel);
    lv_label_set_text(time_label, "00:00:00.00");
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_22, 0);
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * btn_container = lv_obj_create(panel);
    lv_obj_set_size(btn_container, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(btn_container, 0, 0);
    lv_obj_set_style_bg_opa(btn_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(btn_container, 0, 0);
    lv_obj_align(btn_container, LV_ALIGN_CENTER, 0, 40);

    lv_obj_t * start_stop_btn = lv_btn_create(btn_container);
    lv_obj_add_event_cb(start_stop_btn, start_stop_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * start_stop_label = lv_label_create(start_stop_btn);
    lv_label_set_text(start_stop_label, "Start");

    lv_obj_t * reset_btn = lv_btn_create(btn_container);
    lv_obj_add_event_cb(reset_btn, reset_event_handler, LV_EVENT_CLICKED, start_stop_btn);
    lv_obj_t * reset_label = lv_label_create(reset_btn);
    lv_label_set_text(reset_label, "Reset");

    stopwatch_timer = lv_timer_create(stopwatch_timer_cb, 10, (void*)10);
    lv_timer_pause(stopwatch_timer);
}
