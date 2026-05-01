// src/app/timer.c
#include "app/timer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static lv_obj_t * time_label;
static lv_timer_t * timer;
static int32_t time_remaining_ms;
static bool running;

static lv_obj_t * roller_hours;
static lv_obj_t * roller_minutes;
static lv_obj_t * roller_seconds;

static void timer_cb(lv_timer_t * t)
{
    if (time_remaining_ms > 0) {
        time_remaining_ms -= (int32_t)(uintptr_t)t->user_data;
        if (time_remaining_ms < 0) {
            time_remaining_ms = 0;
        }

        int32_t seconds = time_remaining_ms / 1000;
        int32_t minutes = seconds / 60;
        int32_t hours = minutes / 60;

        char time_buffer[12];
        snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d:%02d",
                 hours, minutes % 60, seconds % 60);
        lv_label_set_text(time_label, time_buffer);
    } else {
        // Timer finished
        lv_timer_pause(timer);
        running = false;
        lv_label_set_text(time_label, "Time's up!");
    }
}

static void start_stop_event_handler(lv_event_t * e)
{
    if (running) {
        lv_timer_pause(timer);
        running = false;
        lv_obj_t * label = lv_obj_get_child(lv_event_get_target(e), 0);
        lv_label_set_text(label, "Start");
    } else {
        if (time_remaining_ms == 0) {
            // Get time from rollers
            int hours = lv_roller_get_selected(roller_hours);
            int minutes = lv_roller_get_selected(roller_minutes);
            int seconds = lv_roller_get_selected(roller_seconds);
            time_remaining_ms = (hours * 3600 + minutes * 60 + seconds) * 1000;
        }
        lv_timer_resume(timer);
        running = true;
        lv_obj_t * label = lv_obj_get_child(lv_event_get_target(e), 0);
        lv_label_set_text(label, "Stop");
    }
}

static void reset_event_handler(lv_event_t * e)
{
    time_remaining_ms = 0;
    lv_label_set_text(time_label, "00:00:00");
    if (running) {
        lv_timer_pause(timer);
        running = false;
        lv_obj_t * btn = (lv_obj_t *)lv_event_get_user_data(e);
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text(label, "Start");
    }
}

void create_timer_app(lv_obj_t * parent)
{
    time_remaining_ms = 0;
    running = false;

    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 10, 0);

    // Rollers for setting time
    lv_obj_t * roller_container = lv_obj_create(panel);
    lv_obj_set_size(roller_container, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(roller_container, LV_FLEX_FLOW_ROW);
     lv_obj_set_style_pad_all(roller_container, 0, 0);
    lv_obj_set_style_bg_opa(roller_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(roller_container, 0, 0);
    lv_obj_align(roller_container, LV_ALIGN_CENTER, 0, -40);

    char * opts_24 = (char *)malloc(24 * 4);
    opts_24[0] = '\0';
    for(int i=0; i<24; i++) {
        char buf[5];
        snprintf(buf, sizeof(buf), "%d\n", i);
        strcat(opts_24, buf);
    }
    roller_hours = lv_roller_create(roller_container);
    lv_roller_set_options(roller_hours, opts_24, LV_ROLLER_MODE_INFINITE);

    char * opts_60 = (char *)malloc(60 * 4);
    opts_60[0] = '\0';
    for(int i=0; i<60; i++) {
        char buf[5];
        snprintf(buf, sizeof(buf), "%d\n", i);
        strcat(opts_60, buf);
    }
    roller_minutes = lv_roller_create(roller_container);
    lv_roller_set_options(roller_minutes, opts_60, LV_ROLLER_MODE_INFINITE);
    roller_seconds = lv_roller_create(roller_container);
    lv_roller_set_options(roller_seconds, opts_60, LV_ROLLER_MODE_INFINITE);


    time_label = lv_label_create(panel);
    lv_label_set_text(time_label, "00:00:00");
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_22, 0);
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 20);

    lv_obj_t * btn_container = lv_obj_create(panel);
    lv_obj_set_size(btn_container, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
     lv_obj_set_style_pad_all(btn_container, 0, 0);
    lv_obj_set_style_bg_opa(btn_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(btn_container, 0, 0);
    lv_obj_align(btn_container, LV_ALIGN_CENTER, 0, 60);


    lv_obj_t * start_stop_btn = lv_btn_create(btn_container);
    lv_obj_add_event_cb(start_stop_btn, start_stop_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * start_stop_label = lv_label_create(start_stop_btn);
    lv_label_set_text(start_stop_label, "Start");

    lv_obj_t * reset_btn = lv_btn_create(btn_container);
    lv_obj_add_event_cb(reset_btn, reset_event_handler, LV_EVENT_CLICKED, start_stop_btn);
    lv_obj_t * reset_label = lv_label_create(reset_btn);
    lv_label_set_text(reset_label, "Reset");

    timer = lv_timer_create(timer_cb, 1000, (void*)1000);
    lv_timer_pause(timer);
    
    free(opts_24);
    free(opts_60);
}
