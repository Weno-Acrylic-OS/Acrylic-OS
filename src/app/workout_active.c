#include "app/workout_active.h"
#include "app/fitness.h"
#include "app/navigation_service.h"
#include "drivers/heart_rate.h"
#include <stdio.h>

// --- Module-level State ---
static lv_timer_t * workout_timer;
static lv_timer_t * hr_update_timer;
static uint32_t time_seconds;
static bool is_paused;

static lv_obj_t * time_label;
static lv_obj_t * hr_label;
static lv_obj_t * pause_btn_label;

// --- Timer Callbacks ---

static void workout_timer_cb(lv_timer_t * timer) {
    (void)timer;
    time_seconds++;
    
    uint32_t hours = time_seconds / 3600;
    uint32_t mins = (time_seconds % 3600) / 60;
    uint32_t secs = time_seconds % 60;

    if (hours > 0) {
        lv_label_set_text_fmt(time_label, "%02d:%02d:%02d", hours, mins, secs);
    } else {
        lv_label_set_text_fmt(time_label, "%02d:%02d", mins, secs);
    }
}

static void hr_updater_cb(lv_timer_t * timer) {
    (void)timer;
    lv_label_set_text_fmt(hr_label, "%d bpm", heart_rate_get_bpm());
}

// --- Event Handlers ---

static void finish_event_handler(lv_event_t * e) {
    (void)e;
    // Stop the timers
    lv_timer_del(workout_timer);
    lv_timer_del(hr_update_timer);

    // Update global state
    fitness_app_stop_workout();

    // Go back to the workout list
    navigation_service_go_back();
}

static void pause_event_handler(lv_event_t * e) {
    (void)e;
    is_paused = !is_paused;
    if (is_paused) {
        lv_timer_pause(workout_timer);
        lv_label_set_text(pause_btn_label, "Resume");
    } else {
        lv_timer_resume(workout_timer);
        lv_label_set_text(pause_btn_label, "Pause");
    }
}


// --- UI Creation ---

void create_workout_active_screen(lv_obj_t * parent, const char* workout_type) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_black(), 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(parent, 10, 0);

    // --- Title ---
    lv_obj_t* title_label = lv_label_create(parent);
    lv_label_set_text(title_label, workout_type);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_22, 0);

    // --- Timer ---
    time_label = lv_label_create(parent);
    lv_label_set_text(time_label, "00:00");
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);

    // --- Heart Rate ---
    hr_label = lv_label_create(parent);
    lv_label_set_text(hr_label, "-- bpm");
    lv_obj_set_style_text_font(hr_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(hr_label, lv_color_hex(0xff0000), 0);

    // --- Controls ---
    lv_obj_t* btn_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(btn_cont);
    lv_obj_set_width(btn_cont, lv_pct(100));
    lv_obj_set_height(btn_cont, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btn_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* pause_btn = lv_btn_create(btn_cont);
    lv_obj_add_event_cb(pause_btn, pause_event_handler, LV_EVENT_CLICKED, NULL);
    pause_btn_label = lv_label_create(pause_btn);
    lv_label_set_text(pause_btn_label, "Pause");

    lv_obj_t* finish_btn = lv_btn_create(btn_cont);
    lv_obj_add_event_cb(finish_btn, finish_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t* finish_label = lv_label_create(finish_btn);
    lv_label_set_text(finish_label, "Finish");

    // --- Initialize State & Timers ---
    time_seconds = 0;
    is_paused = false;
    fitness_app_start_workout();
    workout_timer = lv_timer_create(workout_timer_cb, 1000, NULL);
    hr_update_timer = lv_timer_create(hr_updater_cb, 2000, NULL);
}
