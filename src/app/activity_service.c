#include "app/activity_service.h"
#include "lvgl.h"
#include "drivers/heart_rate.h"
#include "app/profile_service.h"
#include "app/persistence.h"
#include "app/time_service.h"

// --- Configuration & State ---

#define DAILY_GOAL_MINUTES 30
#define PERSISTENCE_KEY_ACTIVITY_MINUTES "activity_minutes"
#define PERSISTENCE_KEY_ACTIVITY_DAY "activity_day"

static uint16_t activity_minutes_today = 0;

// --- Private Functions ---

static void activity_check_timer_cb(lv_timer_t * timer) {
    (void)timer;

    uint8_t current_bpm = heart_rate_get_bpm();
    uint8_t user_age = profile_service_get_age();

    // Simple Heart Rate Zone Calculation
    // Zone 1 (Moderate): 50-70% of Max HR
    // Zone 2 (Vigorous): > 70% of Max HR
    int max_hr = 220 - user_age;
    int moderate_threshold = max_hr * 0.5;
    int vigorous_threshold = max_hr * 0.7;

    if (current_bpm >= vigorous_threshold) {
        // Award 2 minutes for every minute in the vigorous zone
        activity_minutes_today += 2;
    } else if (current_bpm >= moderate_threshold) {
        // Award 1 minute for every minute in the moderate zone
        activity_minutes_today += 1;
    }
    
    // Persist the new value
    persistence_set_int(PERSISTENCE_KEY_ACTIVITY_MINUTES, activity_minutes_today);
}


// --- Public API ---

void activity_service_init(void) {
    int today = time_service_get_yday();
    int last_saved_day = persistence_get_int(PERSISTENCE_KEY_ACTIVITY_DAY, -1);

    if (last_saved_day == today) {
        // We're on the same day, load the existing value
        activity_minutes_today = persistence_get_int(PERSISTENCE_KEY_ACTIVITY_MINUTES, 0);
    } else {
        // It's a new day, reset the count and save the new day
        activity_minutes_today = 0;
        persistence_set_int(PERSISTENCE_KEY_ACTIVITY_MINUTES, 0);
        persistence_set_int(PERSISTENCE_KEY_ACTIVITY_DAY, today);
    }

    // Check the heart rate and award minutes once every minute.
    lv_timer_create(activity_check_timer_cb, 60000, NULL);
}

uint16_t activity_service_get_points(void) {
    return activity_minutes_today;
}

uint16_t activity_service_get_goal(void) {
    return DAILY_GOAL_MINUTES;
}
