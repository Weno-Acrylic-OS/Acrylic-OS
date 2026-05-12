#include "app/handwashing.h"
#include <stdio.h>

#define HANDWASH_SECONDS 20

// --- Module-level State ---
static lv_obj_t* time_label;
static lv_obj_t* progress_bar;
static lv_obj_t* start_btn;
static lv_obj_t* start_label;
static lv_timer_t* handwash_timer;
static int countdown;

// --- Forward Declarations ---
static void start_handler(lv_event_t * e);

// --- Timer & Event Logic ---

static void timer_callback(lv_timer_t * timer) {
    (void)timer;

    countdown--;
    lv_bar_set_value(progress_bar, countdown, LV_ANIM_ON);
    lv_label_set_text_fmt(time_label, "%d", countdown);

    if (countdown <= 0) {
        lv_obj_clear_flag(start_btn, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(start_label, "Done! Wash Again?");
        lv_timer_del(handwash_timer);
        handwash_timer = NULL;
    }
}

static void start_handler(lv_event_t * e) {
    (void)e;

    // Reset and start the timer
    countdown = HANDWASH_SECONDS;
    lv_bar_set_value(progress_bar, countdown, LV_ANIM_OFF);
    lv_label_set_text_fmt(time_label, "%d", countdown);

    // Hide the start button during the countdown
    lv_obj_add_flag(start_btn, LV_OBJ_FLAG_HIDDEN);

    // Create a timer that fires every second
    if (handwash_timer == NULL) {
        handwash_timer = lv_timer_create(timer_callback, 1000, NULL);
    }
}


// --- UI Creation ---

void create_handwashing_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x0568A7), 0); // A clean, watery blue
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(parent, 20, 0);

    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "Handwashing Timer");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);

    // The countdown label, big and centered
    time_label = lv_label_create(parent);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
    lv_label_set_text_fmt(time_label, "%d", HANDWASH_SECONDS);

    // The circular progress bar that goes backwards
    progress_bar = lv_bar_create(parent);
    lv_obj_set_size(progress_bar, 150, 150);
    lv_obj_center(progress_bar);
    lv_bar_set_range(progress_bar, 0, HANDWASH_SECONDS);
    lv_bar_set_value(progress_bar, HANDWASH_SECONDS, LV_ANIM_OFF);
    lv_obj_set_style_anim_time(progress_bar, 1000, 0); // Animate over 1s

    // The start button
    start_btn = lv_btn_create(parent);
    lv_obj_set_size(start_btn, 120, 50);
    lv_obj_add_event_cb(start_btn, start_handler, LV_EVENT_CLICKED, NULL);
    start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "Start");
    lv_obj_center(start_label);

    // Clean up timer if the screen is deleted
    // lv_obj_add_event_cb(parent, screen_delete_handler, LV_EVENT_DELETE, NULL);
    // ^ This would require a handler to delete the timer if user navigates away
}
