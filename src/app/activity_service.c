#include "app/activity_service.h"
#include "lvgl.h"
#include "drivers/heart_rate.h"

// --- Configuration & State ---

// For this demo, we hardcode the user's age and daily goal.
// In a real app, this would come from the user's profile.
#define USER_AGE 30
#define DAILY_GOAL_MINUTES 30

static uint16_t activity_minutes_today = 0;

// --- Private Functions ---

static void activity_check_timer_cb(lv_timer_t * timer) {
    (void)timer;

    uint8_t current_bpm = heart_rate_get_bpm();

    // Simple Heart Rate Zone Calculation
    // Zone 1 (Moderate): 50-70% of Max HR
    // Zone 2 (Vigorous): > 70% of Max HR
    int max_hr = 220 - USER_AGE;
    int moderate_threshold = max_hr * 0.5;
    int vigorous_threshold = max_hr * 0.7;

    if (current_bpm >= vigorous_threshold) {
        // Award 2 minutes for every minute in the vigorous zone
        activity_minutes_today += 2;
    } else if (current_bpm >= moderate_threshold) {
        // Award 1 minute for every minute in the moderate zone
        activity_minutes_today += 1;
    }
}


// --- Public API ---

void activity_service_init(void) {
    // For this demo, reset minutes on startup.
    // A real implementation would load today's value from persistence.
    activity_minutes_today = 0;

    // Check the heart rate and award minutes once every minute.
    lv_timer_create(activity_check_timer_cb, 60000, NULL);
}

uint16_t activity_service_get_points(void) {
    // The user wants "minutes", so we'll return the accumulated minutes.
    // The name "get_points" is kept from the header for consistency for now.
    return activity_minutes_today;
}

uint16_t activity_service_get_goal(void) {
    return DAILY_GOAL_MINUTES;
}
