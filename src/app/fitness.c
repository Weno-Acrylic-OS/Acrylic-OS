#include "app/fitness.h"
#include "app/navigation_service.h"
#include "app/workout_active.h"
#include "lvgl.h"
#include <stdio.h> // For sprintf

// --- State Management ---
static bool workout_is_active = false; 
static const char* selected_workout_type = NULL;

bool fitness_app_is_active() {
    return workout_is_active;
}
void fitness_app_start_workout() {
    workout_is_active = true;
}
void fitness_app_stop_workout() {
    workout_is_active = false;
}
// --- End State Management ---


// --- Navigation Wrapper ---
// This wrapper allows us to navigate to a screen that requires a parameter.
void launch_active_workout_screen(lv_obj_t* parent) {
    if (selected_workout_type != NULL) {
        create_workout_active_screen(parent, selected_workout_type);
    } else {
        // Fallback in case something went wrong
        lv_obj_t* label = lv_label_create(parent);
        lv_label_set_text(label, "Error: No workout type selected.");
        lv_obj_center(label);
    }
}


// --- Event Handlers ---

static void workout_select_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    // Store the selected workout type in our static variable
    selected_workout_type = lv_list_get_btn_text(lv_obj_get_parent(btn), btn);

    // Navigate to the screen via our wrapper function
    navigation_service_navigate_to(launch_active_workout_screen);
}

static void go_back_event_handler(lv_event_t * e) {
    (void)e; // Unused
    navigation_service_go_back();
}

// --- UI Creation ---

void create_fitness_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_black(), 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 10, 0);

    // --- Title Bar ---
    lv_obj_t * top_bar = lv_obj_create(parent);
    lv_obj_remove_style_all(top_bar);
    lv_obj_set_width(top_bar, lv_pct(100));
    lv_obj_set_height(top_bar, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(top_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top_bar, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Back button
    lv_obj_t * back_btn = lv_btn_create(top_bar);
    lv_obj_set_style_bg_opa(back_btn, LV_OPA_TRANSP, 0);
    lv_obj_add_event_cb(back_btn, go_back_event_handler, LV_EVENT_CLICKED, NULL); 
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");
    lv_obj_center(back_label);

    lv_obj_t * title = lv_label_create(top_bar);
    lv_label_set_text(title, "Start Workout");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_pad_left(title, 20, 0);


    // --- Workout List ---
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(list, 1);
    
    // Add workout types to the list
    lv_obj_t* btn;
    btn = lv_list_add_btn(list, LV_SYMBOL_CHARGE, "Outdoor Run");
    lv_obj_add_event_cb(btn, workout_select_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(list, LV_SYMBOL_HOME, "Treadmill");
    lv_obj_add_event_cb(btn, workout_select_handler, LV_EVENT_CLICKED, NULL);
    
    btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "Strength");
    lv_obj_add_event_cb(btn, workout_select_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(list, LV_SYMBOL_REFRESH, "Yoga");
    lv_obj_add_event_cb(btn, workout_select_handler, LV_EVENT_CLICKED, NULL);
}
