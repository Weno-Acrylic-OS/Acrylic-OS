#include "lvgl.h"
#include "app/phone_service.h"
#include "app/datalock.h"
#include "app/navigation_service.h"
#include "app/settings.h" // Required for the settings app entry point
#include "app/dnd_service.h"

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


// --- UI Creation ---

// Helper function to create a single icon button with a label
static lv_obj_t* create_icon_button(lv_obj_t* parent, const char* symbol, const char* text) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 70, 70); // Make buttons larger
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0); // Make them circular
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x444444), 0);
    
    // Allow the button to be toggled for DND
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x888888), LV_STATE_CHECKED);

    lv_obj_t * cont = lv_obj_create(btn);
    lv_obj_remove_style_all(cont); // Make container transparent
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cont, 5, 0);
    lv_obj_center(cont);

    lv_obj_t * icon = lv_label_create(cont);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_22, 0);
    lv_label_set_text(icon, symbol);

    lv_obj_t * label = lv_label_create(cont);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_label_set_text(label, text);

    return btn;
}

void create_quick_settings(lv_obj_t * parent) {
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x111111), 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(parent, 10, 0);

    // --- Top Row ---
    lv_obj_t* row1 = lv_obj_create(parent);
    lv_obj_remove_style_all(row1);
    lv_obj_set_width(row1, lv_pct(100));
    lv_obj_set_height(row1, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row1, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    lv_obj_t* dnd_btn = create_icon_button(row1, LV_SYMBOL_MUTE, "DND");
    lv_obj_add_event_cb(dnd_btn, dnd_event_handler, LV_EVENT_CLICKED, NULL);
    // Set initial state from service
    if (dnd_service_is_active()) {
        lv_obj_add_state(dnd_btn, LV_STATE_CHECKED);
    }

    lv_obj_t* find_phone_btn = create_icon_button(row1, LV_SYMBOL_BELL, "Find Phone");
    lv_obj_add_event_cb(find_phone_btn, find_my_phone_event_handler, LV_EVENT_CLICKED, NULL);

    // --- Bottom Row ---
    lv_obj_t* row2 = lv_obj_create(parent);
    lv_obj_remove_style_all(row2);
    lv_obj_set_width(row2, lv_pct(100));
    lv_obj_set_height(row2, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row2, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row2, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* datalock_btn = create_icon_button(row2, LV_SYMBOL_EYE_CLOSE, "Data Lock");
    lv_obj_add_event_cb(datalock_btn, datalock_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t* settings_btn = create_icon_button(row2, LV_SYMBOL_SETTINGS, "Settings");
    lv_obj_add_event_cb(settings_btn, settings_event_handler, LV_EVENT_CLICKED, NULL);
}
