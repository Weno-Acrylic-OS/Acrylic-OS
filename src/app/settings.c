#include "lvgl.h"
#include "app_list.h"
#include "app/datalock.h" // Needed for datalock functions
#include "app/watchface_aod.h"
#include "app/watchface.h"
#include "app/ota_service.h"
#include "app/profile_service.h" // For getting/setting age
#include "app/navigation_service.h"
#include "app/settings_quick_settings.h"
#include <string.h>
#include <stdio.h> // For sprintf

// --- Forward Declarations ---
void create_settings_app(lv_obj_t * parent);
static void create_datalock_settings_screen(lv_obj_t * parent);
static void create_health_settings_screen(lv_obj_t * parent);
static void datalock_settings_keypad_handler(lv_event_t * e);
static void navigate_to_settings_timer_cb(lv_timer_t * timer);
static void back_to_app_list(lv_event_t * e);
static void back_to_settings_app(lv_event_t * e);


// --- State for PIN setup ---
typedef enum {
    STATE_ENTERING_NEW,
    STATE_CONFIRMING_NEW
} pin_setup_state_t;

static pin_setup_state_t current_pin_state;
static char new_pin_buffer[DATALOCK_PIN_LENGTH + 1];

// --- State for Health Settings ---
static lv_obj_t * age_value_label;
static uint8_t current_age_in_ui;


// --- Event Handlers & Timers ---

static void back_to_app_list(lv_event_t * e) {
    (void)e;
    navigation_service_go_back();
}

static void back_to_settings_app(lv_event_t * e) {
    (void)e;
    create_settings_app(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))));
}

static void navigate_to_settings_timer_cb(lv_timer_t * timer) {
    lv_obj_t * parent = timer->user_data;
    if (parent) {
        create_settings_app(parent);
    }
}

static bool aod_enabled = false;

static void toggle_aod(lv_event_t * e) {
    aod_enabled = !aod_enabled;
    lv_obj_t * parent = lv_obj_get_parent(lv_obj_get_parent(lv_event_get_current_target(e)));
    if (aod_enabled) {
        create_watchface_aod(parent);
    } else {
        create_watchface(parent);
    }
}

static lv_obj_t* ota_status_label;

static void ota_ui_update_callback(const char* status) {
    if (ota_status_label) {
        lv_label_set_text(ota_status_label, status);
    }
}

static void create_ota_update_screen(lv_obj_t* parent) {
    lv_obj_clean(parent);

    lv_obj_t* back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_settings_app, LV_EVENT_CLICKED, NULL);
    lv_obj_t* back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    ota_status_label = lv_label_create(parent);
    lv_obj_center(ota_status_label);
    lv_label_set_text(ota_status_label, "Starting OTA update...");

    ota_service_start(ota_ui_update_callback);
}

static void ota_update_event_handler(lv_event_t * e) {
    lv_obj_t* btn = lv_event_get_current_target(e);
    lv_obj_t* parent = lv_obj_get_parent(lv_obj_get_parent(btn));
    create_ota_update_screen(parent);
}

static void settings_list_event_handler(lv_event_t * e) {
    const char * selection = lv_event_get_user_data(e);
    lv_obj_t * parent = lv_obj_get_parent(lv_obj_get_parent(lv_event_get_current_target(e)));

    if (strcmp(selection, "Security") == 0) {
        create_datalock_settings_screen(parent);
    } else if (strcmp(selection, "Health") == 0) {
        create_health_settings_screen(parent);
    } else if (strcmp(selection, "QuickSettings") == 0) {
        create_quick_settings_settings_screen(parent);
    }
}

static void age_change_event_handler(lv_event_t * e) {
    int adjustment = (int)lv_event_get_user_data(e);
    current_age_in_ui += adjustment;

    if (current_age_in_ui < 10) current_age_in_ui = 10;
    if (current_age_in_ui > 99) current_age_in_ui = 99;

    lv_label_set_text_fmt(age_value_label, "%d", current_age_in_ui);
    profile_service_set_age(current_age_in_ui);
}


// --- UI Creation ---

static void create_health_settings_screen(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_pad_top(parent, 20, 0);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_settings_app, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "Health Settings");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_pad_bottom(title, 30, 0);

    // --- Age Setting ---
    lv_obj_t * age_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(age_cont);
    lv_obj_set_width(age_cont, lv_pct(100));
    lv_obj_set_height(age_cont, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(age_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(age_cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_hor(age_cont, 20, 0);

    lv_obj_t * age_label = lv_label_create(age_cont);
    lv_label_set_text(age_label, "Your Age");
    lv_obj_set_style_text_font(age_label, &lv_font_montserrat_18, 0);

    lv_obj_t* age_value_cont = lv_obj_create(age_cont);
    lv_obj_remove_style_all(age_value_cont);
    lv_obj_set_size(age_value_cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(age_value_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(age_value_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_gap(age_value_cont, 15, 0);

    current_age_in_ui = profile_service_get_age();

    lv_obj_t* minus_btn = lv_btn_create(age_value_cont);
    lv_obj_add_event_cb(minus_btn, age_change_event_handler, LV_EVENT_CLICKED, (void*)-1);
    lv_obj_t* minus_label = lv_label_create(minus_btn);
    lv_label_set_text(minus_label, LV_SYMBOL_MINUS);

    age_value_label = lv_label_create(age_value_cont);
    lv_obj_set_style_text_font(age_value_label, &lv_font_montserrat_22, 0);
    lv_label_set_text_fmt(age_value_label, "%d", current_age_in_ui);

    lv_obj_t* plus_btn = lv_btn_create(age_value_cont);
    lv_obj_add_event_cb(plus_btn, age_change_event_handler, LV_EVENT_CLICKED, (void*)1);
    lv_obj_t* plus_label = lv_label_create(plus_btn);
    lv_label_set_text(plus_label, LV_SYMBOL_PLUS);
}


void create_settings_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_black(), 0);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(80));
    lv_obj_center(list);

    lv_list_add_text(list, "Settings");

    lv_obj_t* health_btn = lv_list_add_btn(list, LV_SYMBOL_BELL, "Health"); // Using BELL as a placeholder
    lv_obj_add_event_cb(health_btn, settings_list_event_handler, LV_EVENT_CLICKED, "Health");
    
    lv_obj_t* security_btn = lv_list_add_btn(list, LV_SYMBOL_EYE_CLOSE, "Security");
    lv_obj_add_event_cb(security_btn, settings_list_event_handler, LV_EVENT_CLICKED, "Security");

    lv_list_add_btn(list, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    
    lv_obj_t* display_btn = lv_list_add_btn(list, LV_SYMBOL_IMAGE, "Display");
    
    lv_obj_t* quick_settings_btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "Quick Settings");
    lv_obj_add_event_cb(quick_settings_btn, settings_list_event_handler, LV_EVENT_CLICKED, "QuickSettings");

    lv_list_add_text(list, "System");
    lv_obj_t* ota_btn = lv_list_add_btn(list, LV_SYMBOL_DOWNLOAD, "OTA Update");
    lv_obj_add_event_cb(ota_btn, ota_update_event_handler, LV_EVENT_CLICKED, NULL);

}

static void create_datalock_settings_screen(lv_obj_t * parent) {
    lv_obj_clean(parent);
    current_pin_state = STATE_ENTERING_NEW;
    memset(new_pin_buffer, 0, sizeof(new_pin_buffer));

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_settings_app, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_center(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * title = lv_label_create(container);
    lv_label_set_text(title, "Enter New PIN");
    lv_obj_set_style_text_color(title, lv_theme_get_color_primary(parent), 0);

    lv_obj_t * pin_label = lv_label_create(container);
    lv_label_set_text(pin_label, "_ _ _ _");
    lv_obj_set_style_text_font(pin_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_pad_top(pin_label, 10, 0);
    lv_obj_set_style_pad_bottom(pin_label, 10, 0);

    static const char * map[] = {"1", "2", "3", "\n",
                                        "4", "5", "6", "\n",
                                        "7", "8", "9", "\n",
                                        LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK, ""};
    lv_obj_t * keypad = lv_btnmatrix_create(container);
    lv_btnmatrix_set_map(keypad, map);
    lv_obj_set_size(keypad, 220, 180);
    lv_obj_add_event_cb(keypad, datalock_settings_keypad_handler, LV_EVENT_VALUE_CHANGED, parent);
}

static void datalock_settings_keypad_handler(lv_event_t * e) {
    lv_obj_t * keypad = lv_event_get_current_target(e);
    lv_obj_t * container = lv_obj_get_parent(keypad);
    lv_obj_t * title = lv_obj_get_child(container, 0);
    lv_obj_t * pin_label = lv_obj_get_child(container, 1);
    uint32_t id = lv_btnmatrix_get_selected_btn(keypad);
    const char * txt = lv_btnmatrix_get_btn_text(keypad, id);

    static char entered_pin[DATALOCK_PIN_LENGTH + 1] = {0};
    static int pin_len = 0;

    if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
        if (pin_len > 0) {
            pin_len--;
            entered_pin[pin_len] = '\0';
        }
    } else if (strcmp(txt, LV_SYMBOL_OK) == 0) {
        if (pin_len == DATALOCK_PIN_LENGTH) {
            if (current_pin_state == STATE_ENTERING_NEW) {
                strcpy(new_pin_buffer, entered_pin);
                current_pin_state = STATE_CONFIRMING_NEW;
                lv_label_set_text(title, "Confirm New PIN");
            } else { // STATE_CONFIRMING_NEW
                if (strcmp(entered_pin, new_pin_buffer) == 0) {
                    datalock_set_pin(new_pin_buffer);
                    lv_label_set_text(title, "PIN Updated!");
                    lv_obj_add_flag(keypad, LV_OBJ_FLAG_HIDDEN);
                    // Go back to settings after a delay
                    lv_timer_create(navigate_to_settings_timer_cb, 1500, lv_event_get_user_data(e));
                    return; // Avoid clearing pin label
                } else {
                    lv_label_set_text(title, "PINs don't match. Try again.");
                    current_pin_state = STATE_ENTERING_NEW;
                }
            }
            pin_len = 0;
            memset(entered_pin, 0, sizeof(entered_pin));
        }
    } else { // Digit
        if (pin_len < DATALOCK_PIN_LENGTH) {
            strcat(entered_pin, txt);
            pin_len++;
        }
    }

    // Update display
    char pin_display[20] = {0};
    for (int i = 0; i < DATALOCK_PIN_LENGTH; i++) {
        strcat(pin_display, i < pin_len ? "* " : "_ ");
    }
    if (strlen(pin_display) > 0) pin_display[strlen(pin_display)-1] = 0;
    lv_label_set_text(pin_label, pin_display);
}
