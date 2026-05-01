// src/app/settings.c
#include "lvgl.h"
#include "app_list.h"
#include "app/datalock.h" // Needed for datalock functions
#include <string.h>

// --- Forward Declarations ---
void create_settings_app(lv_obj_t * parent);
static void create_datalock_settings_screen(lv_obj_t * parent);
static void datalock_settings_keypad_handler(lv_event_t * e);
static void navigate_to_settings_timer_cb(lv_timer_t * timer);

// --- State for PIN setup ---
typedef enum {
    STATE_ENTERING_NEW,
    STATE_CONFIRMING_NEW
} pin_setup_state_t;

static pin_setup_state_t current_pin_state;
static char new_pin_buffer[DATALOCK_PIN_LENGTH + 1];

// --- Event Handlers & Timers ---

static void back_to_app_list(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_app_list(parent);
}

static void back_to_settings_app(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_settings_app(parent);
}

static void navigate_to_settings_timer_cb(lv_timer_t * timer) {
    lv_obj_t * parent = timer->user_data;
    if (parent) {
        create_settings_app(parent);
    }
}

static void settings_list_event_handler(lv_event_t * e) {
    const char * selection = lv_event_get_user_data(e);
    lv_obj_t * parent = lv_obj_get_parent(lv_obj_get_parent(lv_event_get_current_target(e)));

    if (strcmp(selection, "Security") == 0) {
        create_datalock_settings_screen(parent);
    }
}

static void theme_dropdown_event_handler(lv_event_t * e) {
    lv_obj_t * dropdown = lv_event_get_target(e);
    int selected_option = lv_dropdown_get_selected(dropdown);

    lv_disp_t * disp = lv_disp_get_default();
    lv_theme_t * theme = NULL;

    if (selected_option == 0) { // Dark
        theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_lighten(LV_PALETTE_GREY, 3), false, &lv_font_montserrat_14);
    } else { // Light
        theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_lighten(LV_PALETTE_GREY, 3), true, &lv_font_montserrat_14);
    }
    lv_disp_set_theme(disp, theme);
}

// --- UI Creation ---

void create_settings_app(lv_obj_t * parent) {
    lv_obj_clean(parent);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(80));
    lv_obj_center(list);

    lv_list_add_text(list, "Settings");

    lv_obj_t* security_btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "Security");
    lv_obj_add_event_cb(security_btn, settings_list_event_handler, LV_EVENT_CLICKED, "Security");

    lv_list_add_btn(list, LV_SYMBOL_WIFI, "Bluetooth");
    lv_list_add_btn(list, LV_SYMBOL_IMAGE, "Display");

    lv_list_add_text(list, "Theme");
    lv_obj_t * theme_dropdown = lv_dropdown_create(parent);
    lv_dropdown_set_options(theme_dropdown, "Dark\nLight");
    lv_obj_align(theme_dropdown, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(theme_dropdown, theme_dropdown_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_list_add_btn(list, LV_SYMBOL_BELL, "Alarms");
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

    static const char * map[] = {"1", "2", "3", "\n", "4", "5", "6", "\n", "7", "8", "9", "\n", LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK, ""};
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
