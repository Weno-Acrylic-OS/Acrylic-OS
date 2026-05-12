#include "app/routines_service.h"
#include "lvgl.h"
#include "app/time_service.h"
#include "app/notification_service.h"

// --- Data Structures & State ---

// Hardcoded routines for demonstration
static routine_t predefined_routines[] = {
    { .hour = 6, .minute = 12, .name = "Morning Routine", .description = "Time to plan your day!", .last_triggered_day = -1 },
    { .hour = 21, .minute = 30, .name = "Evening Wind Down", .description = "Time to relax and get ready for bed.", .last_triggered_day = -1 }
};

#define NUM_ROUTINES (sizeof(predefined_routines) / sizeof(routine_t))


// --- Timer Callback ---

static void check_routines_cb(lv_timer_t * timer) {
    (void)timer;

    uint8_t current_hour = time_service_get_hours();
    uint8_t current_minute = time_service_get_minutes();
    
    // Using day of the year as a simple way to track "today"
    // A more robust implementation would use a full date.
    int current_day = time_service_get_yday();

    for (size_t i = 0; i < NUM_ROUTINES; i++) {
        routine_t *routine = &predefined_routines[i];
        if (routine->hour == current_hour && routine->minute == current_minute) {
            // Check if it has already been triggered for the current day
            if (routine->last_triggered_day != current_day) {
                notification_service_send(routine->name, routine->description, NULL);
                routine->last_triggered_day = current_day;
            }
        }
    }
}


// --- Public API ---

void routines_service_init(void) {
    // Check for routine triggers every 30 seconds
    lv_timer_create(check_routines_cb, 30000, NULL);
}

const routine_t* routines_service_get_routines(int* count) {
    *count = NUM_ROUTINES;
    return predefined_routines;
}
