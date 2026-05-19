#include "ui/desktop.h"
#include "ui/ui_hal.h"

// Common app includes that a desktop UI might need
#include "app/status_bar.h"
#include "app/app_list.h"
#include "app/navigation_service.h"
#include "app/time_service.h" // For status bar time
#include <stdio.h> // For snprintf

// Forward declarations for desktop-specific UI elements
static void create_desktop_status_bar(lv_obj_t* parent);
static void create_desktop_app_grid(lv_obj_t* parent);
static void create_desktop_dock(lv_obj_t* parent);

static void desktop_time_updater_task(lv_timer_t * timer) {
    (void)timer;
    char time_buffer[6];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", time_service_get_hours(), time_service_get_minutes());
    update_time_label(time_buffer); // Assuming status_bar provides this
}

static void create_desktop_status_bar(lv_obj_t* parent) {
    lv_obj_t* status_bar_cont = lv_obj_create(parent);
    lv_obj_set_size(status_bar_cont, LV_PCT(100), 30); // Top bar height
    lv_obj_set_style_bg_color(status_bar_cont, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_width(status_bar_cont, 0, 0);
    lv_obj_set_style_radius(status_bar_cont, 0, 0);
    lv_obj_set_flex_flow(status_bar_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(status_bar_cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(status_bar_cont, 5, 0);

    lv_obj_t* time_label = lv_label_create(status_bar_cont);
    lv_label_set_text(time_label, "00:00");
    lv_obj_set_style_text_color(time_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_14, 0);
    
    // Timer to update time (reusing existing status_bar_time_updater_task if update_time_label is external)
    lv_timer_create(desktop_time_updater_task, 1000, NULL);
}

static void create_desktop_app_grid(lv_obj_t* parent) {
    lv_obj_t* app_grid = lv_obj_create(parent);
    lv_obj_set_size(app_grid, LV_PCT(100), LV_PCT(100));
    lv_obj_center(app_grid);
    lv_obj_set_style_bg_color(app_grid, lv_color_hex(0x444444), 0);
    lv_obj_set_style_border_width(app_grid, 0, 0);
    lv_obj_set_style_radius(app_grid, 0, 0);

    lv_obj_t* label = lv_label_create(app_grid);
    lv_label_set_text(label, "Desktop App Grid (Coming Soon)");
    lv_obj_center(label);
}

static void create_desktop_dock(lv_obj_t* parent) {
    lv_obj_t* dock_cont = lv_obj_create(parent);
    lv_obj_set_size(dock_cont, LV_PCT(100), 50); // Bottom dock height
    lv_obj_set_style_bg_color(dock_cont, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_width(dock_cont, 0, 0);
    lv_obj_set_style_radius(dock_cont, 0, 0);
    lv_obj_set_flex_flow(dock_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(dock_cont, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(dock_cont, 5, 0);

    lv_obj_t* icon1 = lv_label_create(dock_cont);
    lv_label_set_text(icon1, LV_SYMBOL_HOME);
    lv_obj_set_style_text_color(icon1, lv_color_white(), 0);

    lv_obj_t* icon2 = lv_label_create(dock_cont);
    lv_label_set_text(icon2, LV_SYMBOL_FILE);
    lv_obj_set_style_text_color(icon2, lv_color_white(), 0);

    lv_obj_t* icon3 = lv_label_create(dock_cont);
    lv_label_set_text(icon3, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_color(icon3, lv_color_white(), 0);
}


void create_desktop_ui(lv_obj_t* screen) {
    lv_obj_clean(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);

    create_desktop_status_bar(screen);
    create_desktop_app_grid(screen);
    create_desktop_dock(screen);
}

static ui_personality_t desktop_personality = {
    .init_ui = create_desktop_ui
};

ui_personality_t* get_desktop_personality() {
    return &desktop_personality;
}
