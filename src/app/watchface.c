// src/app/watchface.c
#include "lvgl.h"
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "app/watchface.h"
#include "app/status_bar.h"

#define PI 3.14159265

static lv_obj_t * time_label;
static lv_obj_t * date_label;
static lv_obj_t * steps_label;
static lv_obj_t * hr_label;
static lv_obj_t * watchface_parent;
static lv_obj_t * hour_hand;
static lv_obj_t * minute_hand;
static lv_obj_t * second_hand;
static lv_timer_t * digital_timer;
static lv_timer_t * analog_timer;

static int current_style = 0;
static bool show_steps = true;
static bool show_hr = true;
static bool show_date = true;

static void update_watchface_time(lv_timer_t * timer)
{
    time_t now;
    time(&now);
    struct tm * timeinfo = localtime(&now);

    if (current_style < 2) {
        char time_buffer[9];
        strftime(time_buffer, sizeof(time_buffer), "%H:%M", timeinfo);
        if(time_label) lv_label_set_text(time_label, time_buffer);

        if (show_date) {
            char date_buffer[16];
            strftime(date_buffer, sizeof(date_buffer), "%a %b %d", timeinfo);
            if(date_label) lv_label_set_text(date_label, date_buffer);
        }
    } else {
        int hour = timeinfo->tm_hour;
        int minute = timeinfo->tm_min;
        int second = timeinfo->tm_sec;

        lv_obj_set_style_transform_angle(hour_hand, (hour % 12) * 300 + minute / 2, 0);
        lv_obj_set_style_transform_angle(minute_hand, minute * 60, 0);
        lv_obj_set_style_transform_angle(second_hand, second * 60, 0);
    }
}

void update_watchface_style(int style) {
    current_style = style;
    redraw_watchface();
}

void update_watchface_widgets(bool steps, bool hr, bool date) {
    show_steps = steps;
    show_hr = hr;
    show_date = date;
    redraw_watchface();
}

void redraw_watchface(void) {
    if (watchface_parent) {
        create_watchface(watchface_parent);
    }
}

lv_obj_t * get_watchface_parent(void) {
    return watchface_parent;
}

void create_watchface(lv_obj_t * parent)
{
    watchface_parent = parent;
    lv_obj_clean(parent);
    create_status_bar(parent);

    if (digital_timer) {
        lv_timer_del(digital_timer);
        digital_timer = NULL;
    }
    if (analog_timer) {
        lv_timer_del(analog_timer);
        analog_timer = NULL;
    }

    switch (current_style) {
        case 0: // Default
            time_label = lv_label_create(parent);
            lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
            lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -40);

            date_label = lv_label_create(parent);
            lv_obj_set_style_text_font(date_label, &lv_font_montserrat_24, 0);
            lv_obj_align(date_label, LV_ALIGN_CENTER, 0, 20);

            steps_label = lv_label_create(parent);
            lv_label_set_text(steps_label, "Steps: 8,500");
            lv_obj_align(steps_label, LV_ALIGN_CENTER, 0, 60);

            hr_label = lv_label_create(parent);
            lv_label_set_text(hr_label, "HR: 72 bpm");
            lv_obj_align(hr_label, LV_ALIGN_CENTER, 0, 90);
            
            if (!show_steps) lv_obj_add_flag(steps_label, LV_OBJ_FLAG_HIDDEN);
            if (!show_hr) lv_obj_add_flag(hr_label, LV_OBJ_FLAG_HIDDEN);
            if (!show_date) lv_obj_add_flag(date_label, LV_OBJ_FLAG_HIDDEN);

            digital_timer = lv_timer_create(update_watchface_time, 1000, NULL);
            break;
        case 1: // Minimal
            time_label = lv_label_create(parent);
            lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
            lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);
            digital_timer = lv_timer_create(update_watchface_time, 1000, NULL);
            break;
        case 2: // Analog
            hour_hand = lv_line_create(parent);
            minute_hand = lv_line_create(parent);
            second_hand = lv_line_create(parent);

            static lv_point_t hour_points[] = {{0, 0}, {0, -50}};
            lv_line_set_points(hour_hand, hour_points, 2);
            lv_obj_set_style_line_width(hour_hand, 5, 0);
            lv_obj_set_style_line_color(hour_hand, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_transform_pivot_x(hour_hand, 0, 0);
            lv_obj_set_style_transform_pivot_y(hour_hand, 0, 0);


            static lv_point_t minute_points[] = {{0, 0}, {0, -70}};
            lv_line_set_points(minute_hand, minute_points, 2);
            lv_obj_set_style_line_width(minute_hand, 3, 0);
            lv_obj_set_style_line_color(minute_hand, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_transform_pivot_x(minute_hand, 0, 0);
            lv_obj_set_style_transform_pivot_y(minute_hand, 0, 0);

            static lv_point_t second_points[] = {{0, 0}, {0, -90}};
            lv_line_set_points(second_hand, second_points, 2);
            lv_obj_set_style_line_color(second_hand, lv_color_hex(0xFF0000), 0);
            lv_obj_set_style_transform_pivot_x(second_hand, 0, 0);
            lv_obj_set_style_transform_pivot_y(second_hand, 0, 0);

            lv_obj_align(hour_hand, LV_ALIGN_CENTER, 0, 0);
            lv_obj_align(minute_hand, LV_ALIGN_CENTER, 0, 0);
            lv_obj_align(second_hand, LV_ALIGN_CENTER, 0, 0);
            
            analog_timer = lv_timer_create(update_watchface_time, 1000, NULL);
            break;
    }
}
