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
#include "app/sdk_service.h"

static void app_list_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    const char * app_name = lv_event_get_user_data(e);
    lv_obj_t * parent = lv_obj_get_parent(lv_obj_get_parent(obj));

    if (strcmp(app_name, "Settings") == 0) {
        create_settings_app(parent);
    } else if (strcmp(app_name, "Fitness") == 0) {
        create_fitness_app(parent);
    } else if (strcmp(app_name, "Clock") == 0) {
        create_clock_app(parent);
    } else if (strcmp(app_name, "Phone") == 0) {
        create_phone_app(parent);
    } else if (strcmp(app_name, "Watch Face Studio") == 0) {
        create_watchface_studio(parent);
    } else if (strcmp(app_name, "Weather") == 0) {
        create_weather_app(parent);
    } else if (strcmp(app_name, "Sleep") == 0) {
        create_sleep_app(parent);
    } else if (strcmp(app_name, "Music") == 0) {
        create_music_app(parent);
    } else if (strcmp(app_name, "GPS") == 0) {
            create_gps_app(parent);
        } else if (strcmp(app_name, "Weno Store") == 0) {
                create_weno_store_app(parent);
            } else if (strcmp(app_name, "Calendar") == 0) {
                create_calendar_app(parent);
            } else {        sdk_service_run_app(app_name);
    }
}

static lv_obj_t * list;

void add_to_app_list(const char * app_name) {
    if (list) {
        lv_obj_t * btn = lv_list_add_btn(list, NULL, app_name);
        lv_obj_add_event_cb(btn, app_list_event_handler, LV_EVENT_CLICKED, (void*)app_name);
    }
}

void create_app_list(lv_obj_t * parent)
{
    list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    lv_list_add_text(list, "Apps");

    lv_obj_t * settings_btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "Settings");
    lv_obj_add_event_cb(settings_btn, app_list_event_handler, LV_EVENT_CLICKED, "Settings");

    lv_obj_t * fitness_btn = lv_list_add_btn(list, LV_SYMBOL_GPS, "Fitness");
    lv_obj_add_event_cb(fitness_btn, app_list_event_handler, LV_EVENT_CLICKED, "Fitness");

    lv_obj_t * clock_btn = lv_list_add_btn(list, LV_SYMBOL_BELL, "Clock");
    lv_obj_add_event_cb(clock_btn, app_list_event_handler, LV_EVENT_CLICKED, "Clock");

    lv_obj_t * phone_btn = lv_list_add_btn(list, LV_SYMBOL_CALL, "Phone");
    lv_obj_add_event_cb(phone_btn, app_list_event_handler, LV_EVENT_CLICKED, "Phone");

    lv_obj_t * wf_studio_btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Watch Face Studio");
    lv_obj_add_event_cb(wf_studio_btn, app_list_event_handler, LV_EVENT_CLICKED, "Watch Face Studio");

    lv_obj_t * weather_btn = lv_list_add_btn(list, LV_SYMBOL_DUMMY, "Weather");
    lv_obj_add_event_cb(weather_btn, app_list_event_handler, LV_EVENT_CLICKED, "Weather");

    lv_obj_t * sleep_btn = lv_list_add_btn(list, LV_SYMBOL_DUMMY, "Sleep");
    lv_obj_add_event_cb(sleep_btn, app_list_event_handler, LV_EVENT_CLICKED, "Sleep");

    lv_obj_t * music_btn = lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Music");
    lv_obj_add_event_cb(music_btn, app_list_event_handler, LV_EVENT_CLICKED, "Music");

    lv_obj_t * gps_btn = lv_list_add_btn(list, LV_SYMBOL_GPS, "GPS");
        lv_obj_add_event_cb(gps_btn, app_list_event_handler, LV_EVENT_CLICKED, "GPS");

        lv_obj_t * store_btn = lv_list_add_btn(list, LV_SYMBOL_DOWNLOAD, "Weno Store");
            lv_obj_add_event_cb(store_btn, app_list_event_handler, LV_EVENT_CLICKED, "Weno Store");

            lv_obj_t * calendar_btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Calendar");
            lv_obj_add_event_cb(calendar_btn, app_list_event_handler, LV_EVENT_CLICKED, "Calendar");}
