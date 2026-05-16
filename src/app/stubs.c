#include <stdint.h>
#include <stdbool.h>
#include "lvgl.h"
#include "drivers/heart_rate.h"
#include <sys/types.h>  /* Required for mode_t [cite: 17] */
#include <sys/stat.h>   /* Standard header for mkdir [cite: 17] */
/*
 * This file provides dummy (stub) implementations for functions that are
 * web-specific and not available in the native ARM/QEMU build.
 * This allows the project to link correctly without full functionality.
*/

/* Manually define the missing font symbols to satisfy the linker.
   Instead of pointers, we define them as actual lv_font_t objects 
   by copying the data from the 12pt font. */
   
   extern const lv_font_t lv_font_montserrat_12;

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

// Add this stub to satisfy activity_service and routines_service

/* Add these to stubs.c to satisfy the libc_nano warnings */
int _close(int file) { (void)file; return -1; }
int _fstat(int file, struct stat *st) { (void)file; (void)st; return 0; }
int _getpid(void) { return 1; }
int _isatty(int file) { (void)file; return 1; }
int _kill(int pid, int sig) { (void)pid; (void)sig; return -1; }
int _lseek(int file, int ptr, int dir) { (void)file; (void)ptr; (void)dir; return 0; }
int _read(int file, char *ptr, int len) { (void)file; (void)ptr; (void)len; return 0; }
int _write(int file, char *ptr, int len) { (void)file; (void)ptr; (void)len; return len; }

// Satisfies activity_service.c and routines_service.c
int time_service_get_yday(void) {
    return 0; 
}

// Satisfies persistence.c
int mkdir(const char *pathname, mode_t mode) {
    (void)pathname; 
    (void)mode;
    return -1; // Standard return for not implemented/failed
}

// --- Stubs for js_engine.c ---
void js_engine_init(void) { }
void js_engine_run(const char* code) { (void)code; }

// --- Stubs for sim_heart_rate.c ---
// This provides a dummy driver to satisfy the linker
const heart_rate_driver_t sim_heart_rate_driver = {
    .init = NULL,
    .get_bpm = NULL,
};
