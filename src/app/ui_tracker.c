#include "app/ui_tracker.h"
#include "app/today_service.h"
#include "app/time_service.h" // Include for time service
#include <stdio.h> // For snprintf

static lv_obj_t* tracker_time_label = NULL;
static lv_timer_t* tracker_time_timer = NULL; // Variable to hold the timer

static void update_tracker_time_cb(lv_timer_t* timer) {
    if (tracker_time_label) {
        char time_buffer[6];
        snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", time_service_get_hours(), time_service_get_minutes());
        lv_label_set_text(tracker_time_label, time_buffer);
    }
}

// Cleanup function to be called before switching UI
void destroy_tracker_ui(void) {
    if (tracker_time_timer) {
        lv_timer_del(tracker_time_timer);
        tracker_time_timer = NULL;
    }
    tracker_time_label = NULL; // Invalidate the label pointer
}

// This is the main entry point for the simple UI
void create_tracker_ui(lv_obj_t* parent) {
    // A simple vertical layout
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 10, 0);
    lv_obj_set_style_pad_row(parent, 10, 0);

    // A simple clock at the top
    tracker_time_label = lv_label_create(parent); // Assign to static variable
    lv_label_set_text(tracker_time_label, "Loading.."); // Initial text
    lv_obj_set_style_text_font(tracker_time_label, &lv_font_montserrat_28, 0);
    lv_obj_align(tracker_time_label, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_pad_bottom(tracker_time_label, 15, 0);

    // Create a timer to update the time every second
    if(tracker_time_timer == NULL) { // Create timer only if it doesn't exist
        tracker_time_timer = lv_timer_create(update_tracker_time_cb, 1000, NULL);
    }
    update_tracker_time_cb(NULL); // Call once immediately to set current time


    // Then, just render all the health widgets in a list
    today_service_render_widgets(parent);
}
