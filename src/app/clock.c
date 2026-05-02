// src/app/clock.c
#include "lvgl.h"
#include "app_list.h"
#include "app/stopwatch.h"
#include "app/timer.h"
#include "app/alarms.h"

static void back_to_app_list(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * parent = lv_obj_get_parent(lv_obj_get_parent(obj));
    create_app_list(parent);
}

void create_clock_app(lv_obj_t * parent)
{
    lv_obj_clean(parent);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");
    
    lv_obj_t * tabview = lv_tabview_create(parent, LV_DIR_TOP, 50);

    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Alarms");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Timers");
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "Stopwatch");

    create_alarms_app(tab1);
    create_timer_app(tab2);
    create_stopwatch_app(tab3);
}
