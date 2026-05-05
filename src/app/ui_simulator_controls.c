#include "lvgl.h"
#include "app/ui_simulator_controls.h"
#include "app/ui_tracker.h" // Needed for destroy_tracker_ui

// Forward-declare the main creation functions for both UIs
void create_main_ui(lv_obj_t* parent);
// create_tracker_ui is in ui_tracker.h

static bool is_full_os = true;

static void switch_ui_event_handler(lv_event_t * e) {
    (void)e; // Unused event parameter

    // If we are currently in the Lite UI, clean up its resources first
    if (!is_full_os) {
        destroy_tracker_ui();
    }
    
    lv_obj_clean(lv_scr_act()); // Clear the entire screen

    if (is_full_os) {
        create_tracker_ui(lv_scr_act());
        is_full_os = false;
    } else {
        create_main_ui(lv_scr_act());
        is_full_os = true;
    }

    // After redrawing the main UI, we need to re-create the switcher button itself
    create_simulator_controls();
}

void create_simulator_controls(void) {
    lv_obj_t * switcher_btn = lv_btn_create(lv_scr_act());
    lv_obj_align(switcher_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(switcher_btn, switch_ui_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(switcher_btn, lv_color_hex(0xcccccc), 0);
    lv_obj_set_style_bg_opa(switcher_btn, LV_OPA_70, 0);


    lv_obj_t* label = lv_label_create(switcher_btn);
    if (is_full_os) {
        lv_label_set_text(label, "Lite UI");
    } else {
        lv_label_set_text(label, "Full UI");
    }
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
}
