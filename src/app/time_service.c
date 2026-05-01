#include "app/time_service.h"
#include "lvgl.h"
#include <emscripten.h>

static uint8_t hours = 12;
static uint8_t minutes = 0;
static uint8_t seconds = 0;

static void time_tick(lv_timer_t * timer) {
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours++;
            if (hours == 24) {
                hours = 0;
            }
        }
    }
}

void time_service_init() {
    hours = emscripten_run_script_int("new Date().getHours()");
    minutes = emscripten_run_script_int("new Date().getMinutes()");
    seconds = emscripten_run_script_int("new Date().getSeconds()");
    lv_timer_create(time_tick, 1000, NULL);
}

uint8_t time_service_get_hours() {
    return hours;
}

uint8_t time_service_get_minutes() {
    return minutes;
}

uint8_t time_service_get_seconds() {
    return seconds;
}
