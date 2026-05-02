#include "app/oobe.h"
#include "app/persistence.h"
#include "lvgl.h"

// Forward declaration
void create_main_ui(lv_obj_t * parent);

static void get_started_event_handler(lv_event_t * e) {
    persistence_set_oobe_completed(true);
    lv_obj_t * parent = lv_obj_get_parent(lv_event_get_current_target(e));
    lv_obj_clean(parent);
    create_main_ui(parent);
}

void create_oobe_screen(lv_obj_t * parent) {
    lv_obj_clean(parent);

    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_center(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * title = lv_label_create(container);
    lv_label_set_text(title, "Welcome to Weno Fit OS");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);

    lv_obj_t * get_started_btn = lv_btn_create(container);
    lv_obj_add_event_cb(get_started_btn, get_started_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * get_started_label = lv_label_create(get_started_btn);
    lv_label_set_text(get_started_label, "Get Started");
}
