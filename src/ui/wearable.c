#include "ui/wearable.h"
#include "ui/ui_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For rand()
#include "lvgl.h"

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
#include "app/persistence_service.h"
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
#include "ui/ui_datalock_screen.h"


// --- Global UI Objects ---
static lv_obj_t * quick_settings_panel;
static lv_obj_t * notifications_panel;
static lv_obj_t * shortcuts_panel;
static lv_obj_t * voice_assistant_panel;
static lv_obj_t * workout_detect_msgbox = NULL;
static lv_obj_t * stress_detect_msgbox = NULL;

// --- Timers ---
static lv_timer_t* status_bar_timer;
static lv_timer_t* context_updater_timer;
static lv_timer_t* step_simulator_timer;
static lv_timer_t* stress_simulator_timer;
static lv_timer_t* notification_simulator_timer;
static lv_timer_t* gamification_daily_update_timer;

// --- Global State ---
static bool quick_settings_visible = false;
static bool notifications_visible = false;
static bool shortcuts_visible = false;
static bool voice_assistant_visible = false;

// --- Forward Declarations ---
void create_watchface(lv_obj_t * parent);
void create_app_list(lv_obj_t * parent);
void create_today_view(lv_obj_t * parent);
void create_shortcuts_menu(lv_obj_t * parent);
void create_voice_assistant(lv_obj_t * parent);
static void context_updater_cb(lv_timer_t * timer);
static void workout_detect_msgbox_event_handler(lv_event_t * e);
static void stress_detect_msgbox_event_handler(lv_event_t * e);
static void stress_simulator_task(lv_timer_t * timer);
static void notification_simulator_task(lv_timer_t * timer);
void gamification_daily_update_task(lv_timer_t * timer);
static void swipe_event_handler(lv_event_t * e);
static void status_bar_time_updater_task(lv_timer_t * timer);
static void step_simulator_task(lv_timer_t * timer);
void create_wearable_home_screen(lv_obj_t * parent);
void wearable_deinit_ui();

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
    //ble_service_update_goals(goals, goal_count);
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

static void va_close_handler(lv_event_t * e) {
    (void)e;
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

void create_wearable_home_screen(lv_obj_t * parent) {
    lv_obj_add_event_cb(parent, swipe_event_handler, LV_EVENT_GESTURE, NULL);
    status_bar_timer = lv_timer_create(status_bar_time_updater_task, 1000, NULL);
    context_updater_timer = lv_timer_create(context_updater_cb, 1000, NULL);
    step_simulator_timer = lv_timer_create(step_simulator_task, 2000, NULL);
    stress_simulator_timer = lv_timer_create(stress_simulator_task, 10000, NULL);
    notification_simulator_timer = lv_timer_create(notification_simulator_task, 15000, NULL);
    gamification_daily_update_timer = lv_timer_create(gamification_daily_update_task, 86400000, NULL); // 24 hours

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    // --- Create Overlay Panels ---
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
    lv_obj_t * tab_container = lv_obj_create(parent);
    lv_obj_set_size(tab_container, lv_pct(100), lv_pct(95));
    lv_obj_align(tab_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(tab_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(tab_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(tab_container, 0, 0);

    lv_obj_t * app_container = lv_obj_create(parent);
    lv_obj_set_size(app_container, lv_pct(100), lv_pct(100));
    lv_obj_add_flag(app_container, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t * tabview = lv_tabview_create(tab_container, LV_DIR_TOP, 50);

    lv_obj_set_style_bg_opa(tabview, LV_OPA_TRANSP, LV_PART_MAIN);
    
    lv_obj_t * tab_btns = lv_tabview_get_tab_btns(tabview);
    
    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_ITEMS | LV_STATE_CHECKED);
    
    lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, LV_PART_INDICATOR);
    lv_obj_set_style_radius(tab_btns, 50, LV_PART_INDICATOR);

    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, LV_SYMBOL_HOME);
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, LV_SYMBOL_CHARGE);
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, LV_SYMBOL_LIST);

    create_watchface(tab1);
    create_today_view(tab2);
    create_app_list(tab3);
    
    navigation_service_init(app_container, tab_container); 
}

void wearable_deinit_ui() {
    lv_timer_del(status_bar_timer);
    lv_timer_del(context_updater_timer);
    lv_timer_del(step_simulator_timer);
    lv_timer_del(stress_simulator_timer);
    lv_timer_del(notification_simulator_timer);
    lv_timer_del(gamification_daily_update_timer);
}

void unlock_wearable_cb() {
    create_wearable_home_screen(lv_scr_act());
}

void create_main_ui(lv_obj_t * parent) {
    if (datalock_is_locked()) {
        create_ui_datalock_screen(parent, unlock_wearable_cb);
    } else {
        create_wearable_home_screen(parent);
    }
}


// --- Personality Definition ---
static ui_personality_t wearable_personality = {
    .init_ui = create_main_ui,
    .deinit_ui = wearable_deinit_ui
};

ui_personality_t* get_wearable_personality() {
    return &wearable_personality;
}
