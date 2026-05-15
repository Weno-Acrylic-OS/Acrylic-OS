#include "lvgl.h"
#include "app/phone_service.h"
#include "app/datalock.h"
#include "app/navigation_service.h"
#include "app/settings.h" // Required for the settings app entry point
#include "app/dnd_service.h"
#include "app/quick_settings.h"
#include "app/quick_settings_model.h"
#include "app/flashlight_service.h"
#include "app/bluetooth.h"
#include "app/persistence_service.h"

// --- Event Handlers for Quick Settings Buttons ---

static void find_my_phone_event_handler(lv_event_t * e) {
    (void)e;
    phone_service_find_my_phone();
}

static void datalock_event_handler(lv_event_t * e) {
    (void)e;
    // Hide the quick settings panel before showing the datalock screen
    lv_obj_add_flag(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))), LV_OBJ_FLAG_HIDDEN);
    datalock_lock_device();
}

static void settings_event_handler(lv_event_t * e) {
    (void)e;
    // The main settings app is a full-screen app, so we navigate to it
    navigation_service_navigate_to(create_settings_app);
}

static void dnd_event_handler(lv_event_t * e) {
    (void)e;
    dnd_service_toggle();
    
    // The button's visual state is toggled automatically by LVGL's CHECKABLE flag.
    // We just need to make sure the service state is in sync.
}

static void flashlight_event_handler(lv_event_t * e) {
    (void)e;
    flashlight_service_toggle();
}

static void bluetooth_event_handler(lv_event_t * e) {
    (void)e;
    bluetooth_toggle();
}

// --- Data Definition ---

// Array of all *possible* quick settings items
static const quick_setting_item_t available_settings[] = {
    {QUICK_SETTING_DND, LV_SYMBOL_MUTE, "DND", dnd_event_handler},
    {QUICK_SETTING_FIND_PHONE, LV_SYMBOL_BELL, "Find Phone", find_my_phone_event_handler},
    {QUICK_SETTING_DATALOCK, LV_SYMBOL_EYE_CLOSE, "Data Lock", datalock_event_handler},
    {QUICK_SETTING_SETTINGS, LV_SYMBOL_SETTINGS, "Settings", settings_event_handler},
    {QUICK_SETTING_FLASHLIGHT, LV_SYMBOL_POWER, "Flashlight", flashlight_event_handler},
    {QUICK_SETTING_BLUETOOTH, LV_SYMBOL_BLUETOOTH, "Bluetooth", bluetooth_event_handler},
};
static const int num_available_settings = sizeof(available_settings) / sizeof(quick_setting_item_t);

// Array of the *currently configured* quick setting IDs.
static quick_setting_id_t current_quick_settings[MAX_QUICK_SETTINGS];
static int num_current_quick_settings;

void quick_settings_init() {
    persistence_service_get_quick_settings(current_quick_settings, &num_current_quick_settings);

    // If no settings were loaded, apply a default set
    if (num_current_quick_settings == 0) {
        num_current_quick_settings = 4;
        current_quick_settings[0] = QUICK_SETTING_DND;
        current_quick_settings[1] = QUICK_SETTING_FIND_PHONE;
        current_quick_settings[2] = QUICK_SETTING_BLUETOOTH;
        current_quick_settings[3] = QUICK_SETTING_FLASHLIGHT;
        // Also save this default set so it exists for the next boot
        persistence_service_set_quick_settings(current_quick_settings, num_current_quick_settings);
    }
}

// Helper to find a setting's definition by its ID
static const quick_setting_item_t* find_setting_by_id(quick_setting_id_t id) {
    for (int i = 0; i < num_available_settings; i++) {
        if (available_settings[i].id == id) {
            return &available_settings[i];
        }
    }
    return NULL;
}


// --- UI Creation ---

// Helper function to create a single icon button with a label
static lv_obj_t* create_icon_button(lv_obj_t* parent, const quick_setting_item_t* item) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 70, 70);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x444444), 0);
    
    if (item->id == QUICK_SETTING_DND || item->id == QUICK_SETTING_FLASHLIGHT || item->id == QUICK_SETTING_BLUETOOTH) {
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x888888), LV_STATE_CHECKED);
    }

    lv_obj_t * cont = lv_obj_create(btn);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cont, 5, 0);
    lv_obj_center(cont);

    lv_obj_t * icon = lv_label_create(cont);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_22, 0);
    lv_label_set_text(icon, item->symbol);

    lv_obj_t * label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_label_set_text(label, item->text);

    lv_obj_add_event_cb(btn, item->event_handler, LV_EVENT_CLICKED, NULL);

    return btn;
}

void create_quick_settings(lv_obj_t * parent) {
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x111111), 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(parent, 10, 0);

    lv_obj_t* current_row = NULL;

    for (int i = 0; i < num_current_quick_settings; i++) {
        // Create a new row for every 2 buttons
        if (i % 2 == 0) {
            current_row = lv_obj_create(parent);
            lv_obj_remove_style_all(current_row);
            lv_obj_set_width(current_row, lv_pct(100));
            lv_obj_set_height(current_row, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(current_row, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(current_row, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        }

        const quick_setting_item_t* item = find_setting_by_id(current_quick_settings[i]);
        if (item) {
            lv_obj_t* btn = create_icon_button(current_row, item);

            // Special handling for initial state of toggle buttons
            if (item->id == QUICK_SETTING_DND && dnd_service_is_active()) {
                lv_obj_add_state(btn, LV_STATE_CHECKED);
            }
            if (item->id == QUICK_SETTING_FLASHLIGHT && flashlight_service_is_active()) {
                lv_obj_add_state(btn, LV_STATE_CHECKED);
            }
            if (item->id == QUICK_SETTING_BLUETOOTH && bluetooth_is_enabled()) {
                lv_obj_add_state(btn, LV_STATE_CHECKED);
            }
        }
    }
}

// --- Configuration API ---

const quick_setting_item_t* get_available_quick_settings(int* count) {
    *count = num_available_settings;
    return available_settings;
}

void set_current_quick_settings(const quick_setting_id_t* settings, int count) {
    if (count > MAX_QUICK_SETTINGS) {
        // Should probably log an error here
        count = MAX_QUICK_SETTINGS;
    }
    
    num_current_quick_settings = count;
    for (int i = 0; i < count; i++) {
        current_quick_settings[i] = settings[i];
    }
    
    // Save the new settings to persistent storage
    persistence_service_set_quick_settings(current_quick_settings, num_current_quick_settings);
}
