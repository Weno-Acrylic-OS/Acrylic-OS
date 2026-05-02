#include "app/breathing_exercise.h"
#include "app/gamification_service.h"
#include "lvgl.h"

// --- UI Objects and State ---
static lv_obj_t * breathing_label;
static lv_timer_t * breathing_timer = NULL;
static int phase_count;
static int total_cycles = 3; // 3 full breath cycles
static int current_cycle;
static lv_obj_t * current_parent_screen;

// --- Breathing Timer Callback ---
static void breathing_timer_callback(lv_timer_t * timer) {
    (void)timer; // Silence unused parameter warning

    phase_count++;
    if (phase_count > 3) { // 1: In, 2: Hold, 3: Out
        phase_count = 1;
        current_cycle++;
    }

    if (current_cycle > total_cycles) {
        lv_label_set_text(breathing_label, "Exercise Complete!");
        if (breathing_timer) {
            lv_timer_del(breathing_timer);
            breathing_timer = NULL;
        }
        
        // Grant XP and unlock achievement
        gamification_add_xp(50); // XP for completing a breathing exercise
        gamification_unlock_achievement(ACHIEVEMENT_FIRST_BREATHING_EXERCISE);

        // After a short delay, return to the main UI
        // For simplicity, we'll just go back to the app list directly.
        // In a real app, this might go to a summary screen or previous app.
        lv_obj_t * current_screen = lv_scr_act();
        // A simple way to get back to the app list from the screen:
        // Assume the parent of the app (current_parent_screen) is the app list container,
        // which was created in main_ui
        // Here, we just clean the current screen and reload main_ui
        // This is a crude navigation, a proper navigation service would be better
        // For now, let's just do a simple transition back to the app list or previous state.
        // Better: just delete the current screen and make app_list visible if needed.
        lv_obj_clean(lv_scr_act()); // Clean the whole screen
        // Go back to the app list using the global navigation
        // This requires the app_list to be able to be re-created easily
        // For now, let's just make sure the user can go back.
        // This is a temporary placeholder
    } else {
        if (phase_count == 1) {
            lv_label_set_text(breathing_label, "Breathe In...");
            lv_timer_set_period(breathing_timer, 4000); // 4 seconds inhale
        } else if (phase_count == 2) {
            lv_label_set_text(breathing_label, "Hold...");
            lv_timer_set_period(breathing_timer, 4000); // 4 seconds hold
        } else { // phase_count == 3
            lv_label_set_text(breathing_label, "Breathe Out...");
            lv_timer_set_period(breathing_timer, 6000); // 6 seconds exhale
        }
    }
}


// --- Main App UI ---
void create_breathing_exercise_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    current_parent_screen = parent; // Store parent for later navigation

    lv_obj_t * screen_container = lv_obj_create(parent);
    lv_obj_set_size(screen_container, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(screen_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(screen_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(screen_container, lv_color_hex(0x000080), 0); // Dark blue background

    breathing_label = lv_label_create(screen_container);
    lv_label_set_text(breathing_label, "Starting Breathing Exercise...");
    lv_obj_set_style_text_font(breathing_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(breathing_label, lv_color_white(), 0);

    // Initialize state
    phase_count = 0;
    current_cycle = 0;

    // Start the timer
    if (breathing_timer == NULL) {
        breathing_timer = lv_timer_create(breathing_timer_callback, 100, NULL); // Initial delay before first phase
        lv_timer_ready(breathing_timer); // Run immediately
    }
}
