#include "app/calendar.h"
#include "lvgl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_USER_EVENTS 10

// --- Forward Declarations ---
static void create_new_event_screen(lv_obj_t * parent);
static void new_event_button_handler(lv_event_t * e);
static void save_event_handler(lv_event_t * e);
static void cancel_event_handler(lv_event_t * e);
static void keyboard_event_handler(lv_event_t * e);


// --- Data ---
// Local, mock data for the calendar
static calendar_event_t hardcoded_events[] = {
    {"09:00 AM", "Daily Standup"},
    {"11:00 AM", "Project Phoenix Sync"},
    {"01:00 PM", "Lunch with a very long name to test wrapping"},
    {NULL, NULL} // Sentinel
};

// Array to store user-created events
static calendar_event_t user_events[MAX_USER_EVENTS];
static int user_event_count = 0;


// --- Event Creation Screen ---

static void create_new_event_screen(lv_obj_t * parent) {
    lv_obj_clean(parent);

    // Keyboard
    lv_obj_t * kb = lv_keyboard_create(parent);
    lv_obj_add_event_cb(kb, keyboard_event_handler, LV_EVENT_ALL, NULL);

    // Text Area for Title
    lv_obj_t * ta = lv_textarea_create(parent);
    lv_obj_set_width(ta, lv_pct(100)); // Set width to fill parent
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_placeholder_text(ta, "Event Title");
    lv_keyboard_set_textarea(kb, ta);

    // Time Rollers
    // Options for the rollers
    const char * hours = "01\\n02\\n03\\n04\\n05\\n06\\n07\\n08\\n09\\n10\\n11\\n12";
    const char * minutes = "00\\n05\\n10\\n15\\n20\\n25\\n30\\n35\\n40\\n45\\n50\\n55";
    const char * am_pm = "AM\\nPM";

    lv_obj_t * roller_cont = lv_obj_create(parent);
    lv_obj_set_width(roller_cont, lv_pct(100));
    lv_obj_align_to(roller_cont, ta, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_flex_flow(roller_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(roller_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(roller_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(roller_cont, 0, 0);
    
    lv_obj_t * hour_roller = lv_roller_create(roller_cont);
    lv_roller_set_options(hour_roller, hours, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(hour_roller, 2);

    lv_obj_t * min_roller = lv_roller_create(roller_cont);
    lv_roller_set_options(min_roller, minutes, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(min_roller, 2);
    
    lv_obj_t * ampm_roller = lv_roller_create(roller_cont);
    lv_roller_set_options(ampm_roller, am_pm, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(ampm_roller, 2);


    // Save and Cancel Buttons
    lv_obj_t * save_btn = lv_btn_create(parent);
    lv_obj_align(save_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(save_btn, save_event_handler, LV_EVENT_CLICKED, parent); // Pass parent
    lv_obj_t * save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save");

    lv_obj_t * cancel_btn = lv_btn_create(parent);
    lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(cancel_btn, cancel_event_handler, LV_EVENT_CLICKED, parent); // Pass parent
    lv_obj_t * cancel_label = lv_label_create(cancel_btn);
    lv_label_set_text(cancel_label, "Cancel");
}


// --- Event Handlers ---

static void save_event_handler(lv_event_t * e) {
    if (user_event_count >= MAX_USER_EVENTS) {
        // Optional: show an error that storage is full
        return;
    }
    
    lv_obj_t * parent = (lv_obj_t *)lv_event_get_user_data(e);
    lv_obj_t * ta = lv_obj_get_child(parent, 1); // Fragile way to get textarea
    const char * title = lv_textarea_get_text(ta);

    // Don't save empty events
    if (strlen(title) == 0) {
        return;
    }
    
    // Get time from rollers
    lv_obj_t* roller_cont = lv_obj_get_child(parent, 2);
    lv_obj_t* hour_roller = lv_obj_get_child(roller_cont, 0);
    lv_obj_t* min_roller = lv_obj_get_child(roller_cont, 1);
    lv_obj_t* ampm_roller = lv_obj_get_child(roller_cont, 2);
    
    char hour_buf[4], min_buf[4], ampm_buf[4];
    lv_roller_get_selected_str(hour_roller, hour_buf, sizeof(hour_buf));
    lv_roller_get_selected_str(min_roller, min_buf, sizeof(min_buf));
    lv_roller_get_selected_str(ampm_roller, ampm_buf, sizeof(ampm_buf));
    
    // Allocate and format time string
    char * time_str = malloc(16);
    snprintf(time_str, 16, "%s:%s %s", hour_buf, min_buf, ampm_buf);
    
    // Save the event
    user_events[user_event_count].time = time_str;
    user_events[user_event_count].title = strdup(title);
    user_event_count++;

    // Go back to the main calendar view
    create_calendar_app(parent);
}


static void cancel_event_handler(lv_event_t * e) {
    lv_obj_t * parent = (lv_obj_t *)lv_event_get_user_data(e);
    create_calendar_app(parent);
}

static void new_event_button_handler(lv_event_t * e) {
    lv_obj_t * parent = (lv_obj_t *)lv_event_get_user_data(e);
    create_new_event_screen(parent);
}

static void keyboard_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * kb = lv_event_get_target(e);
    if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}


// --- Main App UI ---

void create_calendar_app(lv_obj_t * parent) {
    lv_obj_clean(parent);

    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    // Add "New Event" button
    lv_obj_t * new_btn = lv_list_add_btn(list, LV_SYMBOL_PLUS, "New Event");
    lv_obj_add_event_cb(new_btn, new_event_button_handler, LV_EVENT_CLICKED, parent);

    lv_list_add_text(list, "Today's Agenda");

    // Display hardcoded events
    for (int i = 0; hardcoded_events[i].title != NULL; i++) {
        lv_obj_t * btn = lv_list_add_btn(list, NULL, NULL);
        lv_obj_t* time_label = lv_label_create(btn);
        lv_label_set_text(time_label, hardcoded_events[i].time);
        lv_obj_set_width(time_label, 80);

        lv_obj_t* title_label = lv_label_create(btn);
        lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
        lv_label_set_text(title_label, hardcoded_events[i].title);
        lv_obj_set_flex_grow(title_label, 1);
    }

    // Display user-created events
    if (user_event_count > 0) {
        lv_list_add_text(list, "My Events");
        for (int i = 0; i < user_event_count; i++) {
            lv_obj_t * btn = lv_list_add_btn(list, NULL, NULL);
            lv_obj_t* time_label = lv_label_create(btn);
            lv_label_set_text(time_label, user_events[i].time);
            lv_obj_set_width(time_label, 80);

            lv_obj_t* title_label = lv_label_create(btn);
            lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
            lv_label_set_text(title_label, user_events[i].title);
            lv_obj_set_flex_grow(title_label, 1);
        }
    }
}
