// src/app/app_list.c
#include "lvgl.h"
#include "app/clock.h"
#include "app/fitness.h"
#include "app/phone.h"
#include "app/settings.h"
#include "app/watchface_studio.h"

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
    }
}

void create_app_list(lv_obj_t * parent)
{
    lv_obj_t * list = lv_list_create(parent);
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
}
