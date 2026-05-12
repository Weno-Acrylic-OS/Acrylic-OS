// src/app/app_list.c
#include "lvgl.h"
#include "app/clock.h"
#include "app/fitness.h"
#include "app/phone.h"
#include "app/settings.h"
#include "app/watchface_studio.h"
#include "app/weather.h"
#include "app/sleep.h"
#include "app/music.h"
#include "app/gps.h"
#include "app/weno_store.h"
#include "app/calendar.h"
#include "app/profile.h"
#include "app/sdk_service.h"
#include "app/navigation_service.h"
#include "app/ecg_app.h"
#include "app/calculator.h"
#include "app/medical_id.h"
#include "app/handwashing.h"
#include "app/routines_app.h"
#include <stdlib.h>

typedef struct {
    const char* name;
    const char* icon_path;
    void (*launch_func)(lv_obj_t*);
} app_info_t;

static void app_list_event_handler(lv_event_t* e);

static const app_info_t preinstalled_apps[] = {
    {"Settings", "assets/icons/settings.png", create_settings_app},
    {"Fitness", "assets/icons/fitness.png", create_fitness_app},
    {"Clock", "assets/icons/clock.png", create_clock_app},
    {"Handwashing", LV_SYMBOL_TINT, create_handwashing_app},
    {"Routines", LV_SYMBOL_LIST, create_routines_app},
    {"Phone", LV_SYMBOL_CALL, create_phone_app},
    {"Watch Face Studio", LV_SYMBOL_EDIT, create_watchface_studio},
    {"Weather", LV_SYMBOL_DUMMY, create_weather_app},
    {"Sleep", LV_SYMBOL_DUMMY, create_sleep_app},
    {"Music", LV_SYMBOL_AUDIO, create_music_app},
    {"GPS", LV_SYMBOL_GPS, create_gps_app},
    {"Weno Store", LV_SYMBOL_DOWNLOAD, create_weno_store_app},
    {"Calendar", LV_SYMBOL_EDIT, create_calendar_app},
    {"Profile", LV_SYMBOL_DUMMY, create_profile_app},
    {"ECG", LV_SYMBOL_DUMMY, create_ecg_app},
    {"Calculator", LV_SYMBOL_DUMMY, create_calculator_app},
    {"Medical ID", LV_SYMBOL_PLUS, create_medical_id_app},
};

static lv_obj_t* list;

static void app_list_event_handler(lv_event_t * e)
{
    const app_info_t * app_info = lv_event_get_user_data(e);
    if (app_info->launch_func) {
        navigation_service_navigate_to(app_info->launch_func);
    } else {
        sdk_service_run_app(app_info->name);
    }
}

void add_to_app_list(const char* app_name, const char* icon_path) {
    if (list) {
        lv_obj_t* btn = lv_list_add_btn(list, icon_path, app_name);
        // This is a memory leak, as we don't free the app_info.
        // In a real app, we'd need a proper memory management strategy.
        app_info_t* app_info = malloc(sizeof(app_info_t));
        app_info->name = app_name;
        app_info->icon_path = icon_path;
        app_info->launch_func = NULL; // SDK apps are run differently
        lv_obj_add_event_cb(btn, app_list_event_handler, LV_EVENT_CLICKED, app_info);
    }
}

void create_app_list(lv_obj_t * parent)
{
    list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    lv_list_add_text(list, "Apps");

    for (size_t i = 0; i < sizeof(preinstalled_apps) / sizeof(preinstalled_apps[0]); i++) {
        const app_info_t* app = &preinstalled_apps[i];
        lv_obj_t* btn = lv_list_add_btn(list, app->icon_path, app->name);
        lv_obj_add_event_cb(btn, app_list_event_handler, LV_EVENT_CLICKED, (void*)app);
    }
}
