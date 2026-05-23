#include "ui/desktop.h"
#include "ui/ui_hal.h"
#include "app/time_service.h"
#include "ui/components/window.h"
#include <stdio.h>

static lv_obj_t* start_menu;
static lv_obj_t* time_label;

static void open_app_event_handler(lv_event_t * e) {
    const char* app_name = (const char*)lv_event_get_user_data(e);
    lv_obj_t* win = create_window(lv_scr_act(), app_name, 300, 200);
    lv_obj_center(win);
}

static void start_btn_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (lv_obj_has_flag(start_menu, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(start_menu, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(start_menu, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void desktop_time_updater_task(lv_timer_t * timer) {
    (void)timer;
    char time_buffer[9];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d:%02d", time_service_get_hours(), time_service_get_minutes(), time_service_get_seconds());
    lv_label_set_text(time_label, time_buffer);
}

static void create_desktop_taskbar(lv_obj_t* parent) {
    lv_obj_t* taskbar = lv_obj_create(parent);
    lv_obj_set_size(taskbar, lv_pct(100), 40);
    lv_obj_align(taskbar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(taskbar, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_width(taskbar, 0, 0);
    lv_obj_set_style_radius(taskbar, 0, 0);
    lv_obj_set_flex_flow(taskbar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(taskbar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_hor(taskbar, 10, 0);

    lv_obj_t* start_btn = lv_btn_create(taskbar);
    lv_obj_set_size(start_btn, 80, 30);
    lv_obj_add_event_cb(start_btn, start_btn_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t* start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "Start");
    lv_obj_center(start_label);

    time_label = lv_label_create(taskbar);
    lv_label_set_text(time_label, "00:00:00");
    lv_obj_set_style_text_color(time_label, lv_color_white(), 0);
}

static void create_start_menu(lv_obj_t* parent) {
    start_menu = lv_obj_create(parent);
    lv_obj_set_size(start_menu, 200, 300);
    lv_obj_align(start_menu, LV_ALIGN_BOTTOM_LEFT, 10, -50);
    lv_obj_set_style_bg_color(start_menu, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_color(start_menu, lv_color_hex(0x555555), 0);
    lv_obj_set_style_border_width(start_menu, 2, 0);
    lv_obj_set_style_radius(start_menu, 5, 0);
    lv_obj_add_flag(start_menu, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_flex_flow(start_menu, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(start_menu, 10, 0);

    lv_obj_t* label = lv_label_create(start_menu);
    lv_label_set_text(label, "Applications");
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    
    // Add some dummy app entries
    const char* apps[] = {"Calculator", "Calendar", "Fitness", "Settings"};
    for (int i=0; i < 4; i++) {
        lv_obj_t* app_btn = lv_btn_create(start_menu);
        lv_obj_set_width(app_btn, lv_pct(100));
        lv_obj_add_event_cb(app_btn, open_app_event_handler, LV_EVENT_CLICKED, (void*)apps[i]);
        lv_obj_t* app_label = lv_label_create(app_btn);
        lv_label_set_text(app_label, apps[i]);
        lv_obj_center(app_label);
    }
}

void desktop_deinit_ui() {
    // No resources to clean up
}

void create_desktop_ui(lv_obj_t* screen) {
    lv_obj_clean(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x004080), 0); // A dark blue desktop background

    // Desktop icons area
    lv_obj_t* desktop_area = lv_obj_create(screen);
    lv_obj_set_size(desktop_area, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(desktop_area, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(desktop_area, 0, 0);
    lv_obj_set_style_pad_all(desktop_area, 10, 0);

    lv_obj_t* welcome_label = lv_label_create(desktop_area);
    lv_label_set_text(welcome_label, "Welcome to Acrylic OS");
    lv_obj_align(welcome_label, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_color(welcome_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(welcome_label, &lv_font_montserrat_24, 0);


    create_desktop_taskbar(screen);
    create_start_menu(screen);

    lv_timer_create(desktop_time_updater_task, 1000, NULL);
}

static ui_personality_t desktop_personality = {
    .init_ui = create_desktop_ui,
    .deinit_ui = desktop_deinit_ui
};

ui_personality_t* get_desktop_personality() {
    return &desktop_personality;
}
