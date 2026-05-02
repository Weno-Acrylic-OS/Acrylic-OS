// src/app/fitness.c
#include "app/fitness.h"
#include "app_list.h"

static bool workout_is_active = false;

// Public accessor for the workout state
bool fitness_app_is_active() {
    return workout_is_active;
}

static void back_to_app_list(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_app_list(parent);
}

static void fitness_button_handler(lv_event_t * e) {
    const char* action = lv_event_get_user_data(e);
    lv_obj_t * container = lv_obj_get_parent(lv_event_get_target(e));
    lv_obj_t* status_label = lv_obj_get_child(container, 2); // Fragile, but works for this demo

    if (strcmp(action, "start") == 0) {
        workout_is_active = true;
        lv_label_set_text(status_label, "Workout Active");
    } else if (strcmp(action, "stop") == 0) {
        workout_is_active = false;
        lv_label_set_text(status_label, "Workout Stopped");
    }
}

void create_fitness_app(lv_obj_t * parent) {
    lv_obj_clean(parent);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    // Container for all fitness elements
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_center(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * title = lv_label_create(container);
    lv_label_set_text(title, "Fitness Tracker");

    lv_obj_t * status_label = lv_label_create(container);
    lv_label_set_text(status_label, workout_is_active ? "Workout Active" : "Workout Stopped");
    lv_obj_set_style_pad_bottom(status_label, 20, 0);

    // Button container
    lv_obj_t* btn_container = lv_obj_create(container);
    lv_obj_set_size(btn_container, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(btn_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(btn_container, 0, 0);

    lv_obj_t * start_btn = lv_btn_create(btn_container);
    lv_obj_add_event_cb(start_btn, fitness_button_handler, LV_EVENT_CLICKED, "start");
    lv_obj_t * start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "Start");

    lv_obj_t * stop_btn = lv_btn_create(btn_container);
    lv_obj_add_event_cb(stop_btn, fitness_button_handler, LV_EVENT_CLICKED, "stop");
    lv_obj_t * stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, "Stop");
}