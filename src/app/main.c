// src/app/main.c
#include <stdio.h>
#include <string.h>
#include "lvgl.h"
#include "lvgl_display.h"
#ifdef SIMULATOR_BUILD
#include <emscripten.h>
#endif
#include <stdlib.h> // For rand()

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
#include "app/today_service.h"
#include "js_engine.h"
#include "app/privacy_service.h"
#include "app/gamification_service.h"
#include "app/breathing_exercise.h"
#include "app/navigation_service.h"
#include "app/spo2_service.h"
#include "app/temperature_service.h"
#include "app/notification_service.h"
#include "app/dnd_service.h"
#include "app/routines_service.h"
#include "app/activity_service.h"
#include "app/profile_service.h"
#include "drivers/heart_rate.h"
#include "drivers/sim_heart_rate.h"
#include "app/ble_service.h"

#include "src/extra/themes/default/lv_theme_default.h"

// New includes for UI variants
#include "weno_config.h"
#include "app/ui_tracker.h"
#include "app/ui_simulator_controls.h"


// --- Global UI Objects ---
static lv_obj_t * quick_settings_panel;
static lv_obj_t * notifications_panel;
static lv_obj_t * shortcuts_panel;
static lv_obj_t * voice_assistant_panel;
static lv_obj_t * datalock_screen;
static lv_obj_t * datalock_pin_label;
static lv_obj_t * workout_detect_msgbox = NULL;
static lv_obj_t * stress_detect_msgbox = NULL;

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
static void workout_detect_msgbox_event_handler(lv_event_t * e);
static void stress_detect_msgbox_event_handler(lv_event_t * e);
static void stress_simulator_task(lv_timer_t * timer);
static void notification_simulator_task(lv_timer_t * timer);
void gamification_daily_update_task();


// --- Main Loop ---
void main_loop() {
    lv_timer_handler();
}

// --- Event Handlers & Timers ---
static void notification_simulator_task(lv_timer_t * timer) {
    (void)timer;
    static int i = 0;
    char title[100];
    sprintf(title, "Test Notification %d", i++);
    notification_service_send("Test App", title, "This is a test notification from the simulator.");
}

static void context_updater_cb(lv_timer_t * timer) {
    (void)timer;
    status_bar_show_workout_indicator(fitness_app_is_active());
    status_bar_update_privacy_indicators();
    status_bar_show_dnd_indicator(dnd_service_is_active());
    today_view_update();
    
    int goal_count;
    const goal_t *goals = gamification_get_goals(&goal_count);
    ble_service_update_goals(goals, goal_count);
}

static void status_bar_time_updater_task(lv_timer_t * timer) {
    (void)timer;
    char time_buffer[6];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", time_service_get_hours(), time_service_get_minutes());
    update_time_label(time_buffer);
}

static void step_simulator_task(lv_timer_t * timer) {
    (void)timer;
    static int step_count = 0;
    step_count += 15 + (rand() % 16);
    gamification_update_goal_progress(GOAL_TYPE_DAILY_STEPS, step_count);

    if (!fitness_app_is_active() && workout_detect_msgbox == NULL && stress_detect_msgbox == NULL && (rand() % 20) < 2) {
        const char * btns[] = {"Yes", "No", ""};
        workout_detect_msgbox = lv_msgbox_create(NULL, "Workout?", "Activity detected. Start a workout?", btns, true);
        lv_obj_add_event_cb(workout_detect_msgbox, workout_detect_msgbox_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_center(workout_detect_msgbox);
    }
}

static void stress_simulator_task(lv_timer_t * timer) {
    (void)timer;
    if (workout_detect_msgbox == NULL && stress_detect_msgbox == NULL && (rand() % 100) < 5) {
        const char * btns[] = {"Yes", "No", ""};
        stress_detect_msgbox = lv_msgbox_create(NULL, "Stress?", "High stress detected. Start breathing exercise?", btns, true);
        lv_obj_add_event_cb(stress_detect_msgbox, stress_detect_msgbox_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_center(stress_detect_msgbox);
    }
}


static void workout_detect_msgbox_event_handler(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_current_target(e);
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        const char * txt = lv_msgbox_get_active_btn_text(obj);
        if (strcmp(txt, "Yes") == 0) {
            fitness_app_start_workout();
            gamification_add_xp(10);
            gamification_unlock_achievement(ACHIEVEMENT_FIRST_WORKOUT);
        }
        lv_msgbox_close(obj);
        workout_detect_msgbox = NULL;
    }
}

static void stress_detect_msgbox_event_handler(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_current_target(e);
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        const char * txt = lv_msgbox_get_active_btn_text(obj);
        if (strcmp(txt, "Yes") == 0) {
            navigation_service_navigate_to(create_breathing_exercise_app);
        }
        lv_msgbox_close(obj);
        stress_detect_msgbox = NULL;
    }
}


static void long_press_event_handler(lv_event_t * e) {
    (void)e;
    // Now handled by nav bar
}

static void va_close_handler(lv_event_t * e) {
    (void)e;
    if (voice_assistant_visible) {
        lv_obj_add_flag(voice_assistant_panel, LV_OBJ_FLAG_HIDDEN);
        voice_assistant_visible = false;
    }
}



static void swipe_event_handler(lv_event_t * e) {
#ifdef SIMULATOR_BUILD
    emscripten_log(EM_LOG_CONSOLE, "Swipe event handler called!");
#else
    printf("Swipe event handler called!\n");
#endif
    (void)e;
    if (voice_assistant_visible) return;

    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
#ifdef SIMULATOR_BUILD
    char buffer[50];
    sprintf(buffer, "Gesture direction: %d", dir);
    emscripten_log(EM_LOG_CONSOLE, buffer);
#else
    printf("Gesture direction: %d\n", dir);
#endif

    if (dir == LV_DIR_BOTTOM && !quick_settings_visible && !notifications_visible && !shortcuts_visible) {
#ifdef SIMULATOR_BUILD
        emscripten_log(EM_LOG_CONSOLE, "Opening quick settings");
#else
        printf("Opening quick settings\n");
#endif
        lv_obj_clear_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
        quick_settings_visible = true;
    } else if (dir == LV_DIR_TOP && quick_settings_visible) {
#ifdef SIMULATOR_BUILD
        emscripten_log(EM_LOG_CONSOLE, "Closing quick settings");
#else
        printf("Closing quick settings\n");
#endif
        lv_obj_add_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
        quick_settings_visible = false;
    } else if (dir == LV_DIR_TOP && !notifications_visible && !quick_settings_visible && !shortcuts_visible) {
#ifdef SIMULATOR_BUILD
        emscripten_log(EM_LOG_CONSOLE, "Opening notifications");
#else
        printf("Opening notifications\n");
#endif
        lv_obj_clear_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
        notifications_visible = true;
    } else if (dir == LV_DIR_BOTTOM && notifications_visible) {
#ifdef SIMULATOR_BUILD
        emscripten_log(EM_LOG_CONSOLE, "Closing notifications");
#else
        printf("Closing notifications\n");
#endif
        lv_obj_add_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
        notifications_visible = false;
    } else if (dir == LV_DIR_LEFT && !shortcuts_visible && !quick_settings_visible && !notifications_visible) {
#ifdef SIMULATOR_BUILD
        emscripten_log(EM_LOG_CONSOLE, "Opening shortcuts");
#else
        printf("Opening shortcuts\n");
#endif
        lv_obj_clear_flag(shortcuts_panel, LV_OBJ_FLAG_HIDDEN);
        shortcuts_visible = true;
    } else if (dir == LV_DIR_RIGHT && shortcuts_visible) {
#ifdef SIMULATOR_BUILD
        emscripten_log(EM_LOG_CONSOLE, "Closing shortcuts");
#else
        printf("Closing shortcuts\n");
#endif
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
    lv_obj_add_event_cb(parent, swipe_event_handler, LV_EVENT_GESTURE, NULL);
    lv_timer_create(status_bar_time_updater_task, 1000, NULL);
    lv_timer_create(context_updater_cb, 1000, NULL);
    lv_timer_create(step_simulator_task, 2000, NULL);
    lv_timer_create(stress_simulator_task, 10000, NULL);
    lv_timer_create(notification_simulator_task, 15000, NULL);
    lv_timer_create(gamification_daily_update_task, 86400000, NULL); // 24 hours

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    // --- Create Overlay Panels ---
    // (These are full-screen panels that appear on top of everything else)
    voice_assistant_panel = lv_obj_create(parent);
    lv_obj_set_size(voice_assistant_panel, lv_pct(100), lv_pct(100));
    create_voice_assistant(voice_assistant_panel);
    lv_obj_add_flag(voice_assistant_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(voice_assistant_panel, va_close_handler, LV_EVENT_CLICKED, NULL);

    shortcuts_panel = lv_obj_create(parent);
    lv_obj_set_size(shortcuts_panel, lv_pct(100), lv_pct(100));
    create_shortcuts_menu(shortcuts_panel);
    lv_obj_add_flag(shortcuts_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(shortcuts_panel, LV_OBJ_FLAG_SCROLLABLE);

    quick_settings_panel = lv_obj_create(parent);
    lv_obj_set_size(quick_settings_panel, lv_pct(100), lv_pct(100));
    create_quick_settings(quick_settings_panel);
    lv_obj_add_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(quick_settings_panel, LV_OBJ_FLAG_SCROLLABLE);
    
    notifications_panel = lv_obj_create(parent);
    lv_obj_set_size(notifications_panel, lv_pct(100), lv_pct(100));
    create_notifications_shade(notifications_panel);
    lv_obj_add_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(notifications_panel, LV_OBJ_FLAG_SCROLLABLE);

    // --- Create Main Content Containers ---
    // This container holds the main tab view (Watchface, Today, Apps)
    lv_obj_t * tab_container = lv_obj_create(parent);
    lv_obj_set_size(tab_container, lv_pct(100), lv_pct(95));
    lv_obj_align(tab_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(tab_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(tab_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(tab_container, 0, 0);

    // This container is where individual apps will be rendered
    lv_obj_t * app_container = lv_obj_create(parent);
    lv_obj_set_size(app_container, lv_pct(100), lv_pct(100));
    lv_obj_add_flag(app_container, LV_OBJ_FLAG_HIDDEN); // Hidden by default

    // Create the tabview inside the tab_container
    lv_obj_t * tabview = lv_tabview_create(tab_container, LV_DIR_TOP, 50);

    // Make the entire tabview background transparent
    lv_obj_set_style_bg_opa(tabview, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Get the button matrix that holds the tab buttons
    lv_obj_t * tab_btns = lv_tabview_get_tab_btns(tabview);
    
    // Make the button matrix background transparent
    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_MAIN);

    // Make the tab buttons themselves transparent (for unselected and selected states)
    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_ITEMS | LV_STATE_CHECKED);
    
    // Ensure the indicator (the blue line) stays visible but its background is transparent
    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_INDICATOR);
    lv_obj_set_style_radius(tab_btns, 50, LV_PART_INDICATOR);

    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, LV_SYMBOL_HOME);
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, LV_SYMBOL_CHARGE);
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, LV_SYMBOL_LIST);

    create_watchface(tab1);
    create_today_view(tab2);
    create_app_list(tab3);
    
    // Initialize the navigation service with both containers
    navigation_service_init(app_container, tab_container); 
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
                lv_obj_del(datalock_screen);
                datalock_screen = NULL;
                create_main_ui(lv_scr_act());
            } else {
                lv_label_set_text(datalock_pin_label, "Wrong PIN");
                pin_len = 0;
                memset(entered_pin, 0, sizeof(entered_pin));
            }
        }
    } else {
        if (pin_len < DATALOCK_PIN_LENGTH) {
            strcat(entered_pin, txt);
            pin_len++;
        }
    }

    if (strcmp(txt, LV_SYMBOL_OK) != 0) {
        char pin_display[20] = {0};
        for (int i = 0; i < DATALOCK_PIN_LENGTH; i++) {
            if (i < pin_len) strcat(pin_display, "* ");
            else strcat(pin_display, "_ ");
        }
        if(strlen(pin_display) > 0) pin_display[strlen(pin_display)-1] = 0;
        lv_label_set_text(datalock_pin_label, pin_display);
    }
}

// --- Main Entry Point ---
int main() {
    lv_init();
    lv_png_init();
    lvgl_display_init();
    time_service_init();
    persistence_init();
    sdk_service_init();
    today_service_init();
    today_register_native_widgets();
    js_engine_init();
    privacy_service_init();
    gamification_service_init();
    notification_service_init();
    dnd_service_init();
    routines_service_init();
    activity_service_init();
    profile_service_init();
    spo2_service_init();
    temperature_service_init();
    datalock_init();
    heart_rate_init(&sim_heart_rate_driver);
    ble_service_init();

    lv_disp_t * disp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_lighten(LV_PALETTE_GREY, 3), true, &lv_font_montserrat_14);
    lv_disp_set_theme(disp, theme);

    lv_obj_t* screen = lv_scr_act();

#ifdef SIMULATOR_BUILD
    // In simulator, we always start with Full OS and add the switcher
    create_main_ui(screen);
     // Permenantly removed due to discontinuation of Weno Fit OS Lite UI.
#else
    // For device builds, boot directly into the main UI like the simulator
    create_main_ui(screen);
#endif

#ifdef SIMULATOR_BUILD
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (1) {
        main_loop();
        // You might need a platform-specific delay or sleep function here
        // to avoid pinning the CPU at 100%, e.g., platform_sleep_ms(5);
    }
#endif

    return 0;
}
