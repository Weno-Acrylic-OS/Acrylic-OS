#include "app/flashlight_service.h"
#include <stdbool.h>

static bool is_active = false;
static lv_obj_t* flashlight_screen = NULL;

static void close_flashlight_event_handler(lv_event_t * e) {
    (void)e; // Unused parameter
    flashlight_service_toggle();
}

void create_flashlight_screen() {
    if (flashlight_screen) return; // Already created

    lv_obj_t* parent = lv_scr_act();
    flashlight_screen = lv_obj_create(parent);
    lv_obj_remove_style_all(flashlight_screen);
    lv_obj_set_size(flashlight_screen, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(flashlight_screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(flashlight_screen, LV_OPA_COVER, 0);
    lv_obj_add_event_cb(flashlight_screen, close_flashlight_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_clear_flag(flashlight_screen, LV_OBJ_FLAG_GESTURE_BUBBLE);
}

void flashlight_service_toggle() {
    is_active = !is_active;

    if (is_active) {
        create_flashlight_screen();
    } else {
        if (flashlight_screen) {
            lv_obj_del(flashlight_screen);
            flashlight_screen = NULL;
        }
    }
}

bool flashlight_service_is_active() {
    return is_active;
}
