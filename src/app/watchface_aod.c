#include "app/watchface_aod.h"
#include "app/time_service.h"
#include "lvgl.h"

static lv_obj_t * aod_time_label;

static void time_updater_task(lv_timer_t * timer) {
    char time_buffer[6];
    lv_snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", time_service_get_hours(), time_service_get_minutes());
    lv_label_set_text(aod_time_label, time_buffer);
}

void create_watchface_aod(lv_obj_t * parent) {
    // Use a black background
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);

    // Create a label for the time
    aod_time_label = lv_label_create(parent);
    lv_label_set_text(aod_time_label, "12:00");
    lv_obj_set_style_text_color(aod_time_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(aod_time_label, &lv_font_montserrat_24, 0);
    lv_obj_align(aod_time_label, LV_ALIGN_CENTER, 0, 0);

    // Create a timer to update the time every minute
    lv_timer_create(time_updater_task, 60000, NULL);
}
