#include <stdint.h>
#include <stdbool.h>
#include "lvgl.h"
#include "drivers/heart_rate.h"

/*
 * This file provides dummy (stub) implementations for functions that are
 * web-specific and not available in the native ARM/QEMU build.
 * This allows the project to link correctly without full functionality.
*/

// --- Stubs for time_service.c ---
void time_service_init(void) { }
uint8_t time_service_get_hours(void) { return 12; }
uint8_t time_service_get_minutes(void) { return 34; }
uint8_t time_service_get_seconds(void) { return 56; }

// --- Stubs for phone_service.c ---
void phone_service_call_for_help(void) { }
const char* phone_service_get_current_track(void) { return "Track Unknown"; }
bool phone_service_is_playing(void) { return false; }
void phone_service_music_pause(void) { }
void phone_service_music_play(void) { }
void phone_service_music_prev(void) { }
void phone_service_music_next(void) { }
void phone_service_find_my_phone(void) { }

// --- Stubs for shortcuts.c ---
void create_shortcuts_menu(lv_obj_t * parent) { (void)parent; }

// --- Stubs for weno_store.c ---
void create_weno_store_app(lv_obj_t * parent) { (void)parent; }
void weno_store_receive_apps_json(char* json) { (void)json; }
void weno_store_receive_app_source(char* source) { (void)source; }


// --- Stubs for js_engine.c ---
void js_engine_init(void) { }
void js_engine_run(const char* code) { (void)code; }

// --- Stubs for sim_heart_rate.c ---
// This provides a dummy driver to satisfy the linker
const heart_rate_driver_t sim_heart_rate_driver = {
    .init = NULL,
    .get_bpm = NULL,
};
