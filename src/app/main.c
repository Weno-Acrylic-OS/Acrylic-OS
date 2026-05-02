// src/app/main.c
#include <stdio.h>
#include <string.h>
#include "lvgl.h"
#include "lvgl_display.h"
#include <emscripten.h>

// App-specific includes
#include "app/datalock.h"
#include "app/status_bar.h"
#include "app/today.h"
#include "app/shortcuts.h"
#include "app/quick_settings.h"
#include "app/notifications.h"
#include "app/voice_assistant.h"
#include "app/fitness.h"
#include "app/time_service.h"
#include "app/persistence.h"
#include "app/oobe.h"
#include "app/sdk_service.h"
#include "js_engine.h"
#include "app/privacy_service.h"

#include "src/extra/themes/default/lv_theme_default.h"

// --- Global UI Objects ---
static lv_obj_t * quick_settings_panel;
static lv_obj_t * notifications_panel;
static lv_obj_t * shortcuts_panel;
static lv_obj_t * voice_assistant_panel;
static lv_obj_t * datalock_screen;
static lv_obj_t * datalock_pin_label;

// --- Global State ---
static bool quick_settings_visible = false;
static bool notifications_visible = false;
static bool shortcuts_visible = false;
static bool voice_assistant_visible = false;

// --- Forward Declarations ---
void create_main_ui(lv_obj_t * parent);
void create_watchface(lv_obj_t * parent);
void create_app_list(lv_obj_t * parent);
void create_today_view(lv_obj_t * parent);
void create_shortcuts_menu(lv_obj_t * parent);
void create_voice_assistant(lv_obj_t * parent);
static void datalock_keypad_event_handler(lv_event_t * e);
static void context_updater_cb(lv_timer_t * timer);


// --- Main Loop ---
void main_loop() {
    lv_timer_handler();
}

// --- Event Handlers & Timers ---
static void context_updater_cb(lv_timer_t * timer) {
    status_bar_show_workout_indicator(fitness_app_is_active());
    status_bar_update_privacy_indicators();
}

static void status_bar_time_updater_task(lv_timer_t * timer) {
    char time_buffer[6];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", time_service_get_hours(), time_service_get_minutes());
    update_time_label(time_buffer);
}

static void long_press_event_handler(lv_event_t * e) {
    if (!voice_assistant_visible) {
        lv_obj_clear_flag(voice_assistant_panel, LV_OBJ_FLAG_HIDDEN);
        voice_assistant_visible = true;
    }
}

static void va_close_handler(lv_event_t * e) {
    if (voice_assistant_visible) {
        lv_obj_add_flag(voice_assistant_panel, LV_OBJ_FLAG_HIDDEN);
        voice_assistant_visible = false;
    }
}

static void swipe_event_handler(lv_event_t * e) {
    if (voice_assistant_visible) return;

    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

    if (dir == LV_DIR_BOTTOM && !quick_settings_visible && !notifications_visible && !shortcuts_visible) {
        lv_obj_clear_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
        quick_settings_visible = true;
    } else if (dir == LV_DIR_TOP && quick_settings_visible) {
        lv_obj_add_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
        quick_settings_visible = false;
    } else if (dir == LV_DIR_TOP && !notifications_visible && !quick_settings_visible && !shortcuts_visible) {
        lv_obj_clear_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
        notifications_visible = true;
    } else if (dir == LV_DIR_BOTTOM && notifications_visible) {
        lv_obj_add_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
        notifications_visible = false;
    } else if (dir == LV_DIR_LEFT && !shortcuts_visible && !quick_settings_visible && !notifications_visible) {
        lv_obj_clear_flag(shortcuts_panel, LV_OBJ_FLAG_HIDDEN);
        shortcuts_visible = true;
    } else if (dir == LV_DIR_RIGHT && shortcuts_visible) {
        lv_obj_add_flag(shortcuts_panel, LV_OBJ_FLAG_HIDDEN);
        shortcuts_visible = false;
    }
}

// --- UI Creation Functions ---

void create_datalock_screen(lv_obj_t * parent) {
    datalock_screen = lv_obj_create(parent);
    lv_obj_set_size(datalock_screen, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(datalock_screen, lv_color_black(), 0);
    lv_obj_set_style_border_width(datalock_screen, 0, 0);
    lv_obj_set_flex_flow(datalock_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(datalock_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * title = lv_label_create(datalock_screen);
    lv_label_set_text(title, "Enter PIN");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);

    datalock_pin_label = lv_label_create(datalock_screen);
    lv_label_set_text(datalock_pin_label, "_ _ _ _");
    lv_obj_set_style_text_font(datalock_pin_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(datalock_pin_label, lv_color_white(), 0);
    lv_obj_set_style_pad_top(datalock_pin_label, 10, 0);
    lv_obj_set_style_pad_bottom(datalock_pin_label, 10, 0);

    static const char * map[] = {"1", "2", "3", "\n",
                                "4", "5", "6", "\n",
                                "7", "8", "9", "\n",
                                LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK, ""};

    lv_obj_t * keypad = lv_btnmatrix_create(datalock_screen);
    lv_btnmatrix_set_map(keypad, map);
    lv_obj_set_size(keypad, 220, 180);
    lv_obj_align(keypad, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(keypad, datalock_keypad_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}

void create_main_ui(lv_obj_t * parent) {
    lv_timer_create(status_bar_time_updater_task, 1000, NULL);
    lv_timer_create(context_updater_cb, 1000, NULL); // Create the new context timer

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    voice_assistant_panel = lv_obj_create(parent);
    lv_obj_set_size(voice_assistant_panel, lv_pct(100), lv_pct(100));
    create_voice_assistant(voice_assistant_panel);
    lv_obj_add_flag(voice_assistant_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(voice_assistant_panel, va_close_handler, LV_EVENT_CLICKED, NULL);

    shortcuts_panel = lv_obj_create(parent);
    lv_obj_set_size(shortcuts_panel, lv_pct(100), lv_pct(100));
    create_shortcuts_menu(shortcuts_panel);
    lv_obj_add_flag(shortcuts_panel, LV_OBJ_FLAG_HIDDEN);

    quick_settings_panel = lv_obj_create(parent);
    lv_obj_set_size(quick_settings_panel, lv_pct(100), lv_pct(100));
    create_quick_settings(quick_settings_panel);
    lv_obj_add_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t * main_container = lv_obj_create(parent);
    lv_obj_set_size(main_container, lv_pct(100), lv_pct(100));
    lv_obj_add_event_cb(main_container, swipe_event_handler, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(main_container, long_press_event_handler, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(main_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(main_container, 0, 0);

    lv_obj_t * tabview = lv_tabview_create(main_container, LV_DIR_TOP, 50);

    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Home");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Today");
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "Apps");

    create_watchface(tab1);
    create_today_view(tab2);
    create_app_list(tab3);

    notifications_panel = lv_obj_create(parent);
    lv_obj_set_size(notifications_panel, lv_pct(100), lv_pct(100));
    create_notifications_shade(notifications_panel);
    lv_obj_add_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
}

static void datalock_keypad_event_handler(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    const char * txt = lv_btnmatrix_get_btn_text(obj, id);

    static char entered_pin[DATALOCK_PIN_LENGTH + 1] = {0};
    static int pin_len = 0;

    if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
        if (pin_len > 0) {
            pin_len--;
            entered_pin[pin_len] = '\0';
        }
    } else if (strcmp(txt, LV_SYMBOL_OK) == 0) {
        if (pin_len == DATALOCK_PIN_LENGTH) {
            if (datalock_check_pin(entered_pin)) {
                // Correct PIN: clean up and show main UI
                lv_obj_del(datalock_screen);
                datalock_screen = NULL;
                create_main_ui(lv_scr_act());
            } else {
                // Incorrect PIN
                lv_label_set_text(datalock_pin_label, "Wrong PIN");
                pin_len = 0;
                memset(entered_pin, 0, sizeof(entered_pin));
            }
        }
    } else { // Digit
        if (pin_len < DATALOCK_PIN_LENGTH) {
            strcat(entered_pin, txt);
            pin_len++;
        }
    }

    // Update the display label
    if (strcmp(txt, LV_SYMBOL_OK) != 0) {
        char pin_display[20] = {0};
        for (int i = 0; i < DATALOCK_PIN_LENGTH; i++) {
            if (i < pin_len) {
                strcat(pin_display, "* ");
            } else {
                strcat(pin_display, "_ ");
            }
        }
        // Remove trailing space
        if(strlen(pin_display) > 0) pin_display[strlen(pin_display)-1] = 0;

        lv_label_set_text(datalock_pin_label, pin_display);
    }
}

// --- Main Entry Point ---
int main() {
    lv_init();
    lvgl_display_init();
    time_service_init();
    persistence_init();
    sdk_service_init();
    js_engine_init();
    privacy_service_init();
    datalock_init();

    lv_disp_t * disp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_lighten(LV_PALETTE_GREY, 3), false, &lv_font_montserrat_14);
    lv_disp_set_theme(disp, theme);

    lv_obj_t* screen = lv_scr_act();

    if (persistence_get_oobe_completed() && !datalock_is_locked()) {
        create_main_ui(screen);
    } else if (datalock_is_locked()) {
        create_datalock_screen(screen);
    } else {
        create_oobe_screen(screen);
    }

    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}
