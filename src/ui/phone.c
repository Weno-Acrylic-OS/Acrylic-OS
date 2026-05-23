#include "ui/phone.h"
#include "ui/ui_hal.h"
#include "app/time_service.h"
#include "app/datalock.h"
#include "app/navigation_service.h"
#include "app/quick_settings.h"
#include "app/notifications.h"
#include "ui/ui_datalock_screen.h"
#include "app/calculator.h"
#include <stdio.h>

static lv_obj_t * time_label;
static lv_obj_t * quick_settings_panel;
static lv_obj_t * notifications_panel;
static lv_timer_t* time_updater_timer;

void create_phone_home_screen(lv_obj_t * parent);
static void swipe_event_handler(lv_event_t * e);

static void time_updater_task(lv_timer_t * timer) {
    (void)timer;
    char time_buffer[6];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", time_service_get_hours(), time_service_get_minutes());
    lv_label_set_text(time_label, time_buffer);
}

static void create_phone_status_bar(lv_obj_t* parent) {
    lv_obj_t* status_bar = lv_obj_create(parent);
    lv_obj_set_size(status_bar, lv_pct(100), 30);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x111111), 0);
    lv_obj_set_style_border_width(status_bar, 0, 0);
    lv_obj_set_style_radius(status_bar, 0, 0);
    lv_obj_set_flex_flow(status_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(status_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_hor(status_bar, 15, 0);

    time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "00:00");
    lv_obj_set_style_text_color(time_label, lv_color_white(), 0);

    lv_obj_t* icons = lv_obj_create(status_bar);
    lv_obj_set_layout(icons, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(icons, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(icons, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(icons, LV_OPA_TRANSP, 0);
     lv_obj_set_style_border_width(icons, 0, 0);

    lv_obj_t* wifi_icon = lv_label_create(icons);
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_color(wifi_icon, lv_color_white(), 0);
    lv_obj_set_style_pad_hor(wifi_icon, 5, 0);

    lv_obj_t* battery_icon = lv_label_create(icons);
    lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_FULL);
    lv_obj_set_style_text_color(battery_icon, lv_color_white(), 0);
    lv_obj_set_style_pad_left(battery_icon, 5, 0);
}

static void create_phone_nav_bar(lv_obj_t* parent) {
    lv_obj_t* nav_bar = lv_obj_create(parent);
    lv_obj_set_size(nav_bar, lv_pct(100), 50);
    lv_obj_align(nav_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(nav_bar, lv_color_hex(0x111111), 0);
    lv_obj_set_style_border_width(nav_bar, 0, 0);
    lv_obj_set_style_radius(nav_bar, 0, 0);
    lv_obj_set_flex_flow(nav_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(nav_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* back_btn = lv_label_create(nav_bar);
    lv_label_set_text(back_btn, LV_SYMBOL_PREV);
    lv_obj_set_style_text_color(back_btn, lv_color_white(), 0);

    lv_obj_t* home_btn = lv_label_create(nav_bar);
    lv_label_set_text(home_btn, LV_SYMBOL_HOME);
    lv_obj_set_style_text_color(home_btn, lv_color_white(), 0);

    lv_obj_t* overview_btn = lv_label_create(nav_bar);
    lv_label_set_text(overview_btn, LV_SYMBOL_IMAGE);
    lv_obj_set_style_text_color(overview_btn, lv_color_white(), 0);
}

static void app_launcher_event_handler(lv_event_t * e) {
    printf("app_launcher_event_handler called\n");
    screen_create_func_t create_func = (screen_create_func_t)lv_event_get_user_data(e);
    if (create_func) {
        // navigation_service_navigate_to(create_func);
        create_func(lv_scr_act());
    }
}

static void swipe_event_handler(lv_event_t * e) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

    if (dir == LV_DIR_BOTTOM) {
        lv_obj_clear_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
    } else if (dir == LV_DIR_TOP) {
        lv_obj_add_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
    }
}


void create_phone_home_screen(lv_obj_t * screen) {
    lv_obj_clean(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x333333), 0);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(screen, swipe_event_handler, LV_EVENT_GESTURE, NULL);

    lv_obj_t* home_container = lv_obj_create(screen);
    lv_obj_set_size(home_container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(home_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(home_container, 0, 0);

    lv_obj_t* app_container = lv_obj_create(screen);
    lv_obj_set_size(app_container, lv_pct(100), lv_pct(100));
    lv_obj_add_flag(app_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_bg_opa(app_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(app_container, 0, 0);

    quick_settings_panel = lv_obj_create(screen);
    lv_obj_set_size(quick_settings_panel, lv_pct(100), lv_pct(100));
    create_quick_settings(quick_settings_panel);
    lv_obj_add_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(quick_settings_panel, LV_OBJ_FLAG_SCROLLABLE);

    notifications_panel = lv_obj_create(screen);
    lv_obj_set_size(notifications_panel, lv_pct(100), lv_pct(100));
    create_notifications_shade(notifications_panel);
    lv_obj_add_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(notifications_panel, LV_OBJ_FLAG_SCROLLABLE);


    navigation_service_init(app_container, home_container);

    create_phone_status_bar(home_container);

    lv_obj_t* app_grid = lv_obj_create(home_container);
    lv_obj_set_size(app_grid, lv_pct(100), lv_pct(85));
    lv_obj_align(app_grid, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_layout(app_grid, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_opa(app_grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(app_grid, 0, 0);

    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_grid_dsc_array(app_grid, col_dsc, row_dsc);

    const char* apps[] = {
        LV_SYMBOL_CALL, "Phone",
        LV_SYMBOL_ENVELOPE, "Messages",
        LV_SYMBOL_GPS, "Maps",
        LV_SYMBOL_VIDEO, "Camera",
        LV_SYMBOL_IMAGE, "Photos",
        LV_SYMBOL_SETTINGS, "Settings",
        LV_SYMBOL_FILE, "Files",
        "C", "Calculator"
    };
    
    screen_create_func_t app_create_funcs[] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, create_calculator_app
    };

    for(int i = 0; i < 8; i++) {
        uint8_t row = i / 4;
        uint8_t col = i % 4;

        lv_obj_t* icon_cont = lv_btn_create(app_grid);
        lv_obj_set_grid_cell(icon_cont, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
        lv_obj_set_flex_flow(icon_cont, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(icon_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_bg_opa(icon_cont, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(icon_cont, 0, 0);
        lv_obj_add_event_cb(icon_cont, app_launcher_event_handler, LV_EVENT_CLICKED, app_create_funcs[i]);

        lv_obj_t* icon = lv_label_create(icon_cont);
        lv_label_set_text(icon, apps[i*2]);
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_color(icon, lv_color_white(), 0);

        lv_obj_t* label = lv_label_create(icon_cont);
        lv_label_set_text(label, apps[i*2 + 1]);
        lv_obj_set_style_text_color(label, lv_color_white(), 0);
    }

    create_phone_nav_bar(home_container);
    time_updater_timer = lv_timer_create(time_updater_task, 1000, NULL);
}

void phone_deinit_ui() {
    lv_timer_del(time_updater_timer);
}

void unlock_phone_cb() {
    create_phone_home_screen(lv_scr_act());
}

static void create_phone_ui(lv_obj_t* screen) {
    if (datalock_is_locked()) {
        create_ui_datalock_screen(screen, unlock_phone_cb);
    } else {
        create_phone_home_screen(screen);
    }
}

static ui_personality_t phone_personality = {
    .init_ui = create_phone_ui,
    .deinit_ui = phone_deinit_ui
};

ui_personality_t* get_phone_personality() {
    return &phone_personality;
}
