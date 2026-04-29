// src/app/main.c
#include "lvgl.h"
#include "lvgl_display.h"
#include <emscripten.h>
#include "quick_settings.h"
#include "notifications.h"
#include "src/extra/themes/default/lv_theme_default.h" // Include the default theme header

static lv_obj_t * quick_settings_panel;
static bool quick_settings_visible = false;

static lv_obj_t * notifications_panel;
static bool notifications_visible = false;

void main_loop() {
    lv_timer_handler();
}

void create_watchface(lv_obj_t * parent);
void create_app_list(lv_obj_t * parent);

static void swipe_event_handler(lv_event_t * e)
{
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

    if (dir == LV_DIR_TOP && !quick_settings_visible && !notifications_visible) {
        lv_obj_clear_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_scroll_to_view(quick_settings_panel, LV_ANIM_ON);
        quick_settings_visible = true;
    } else if (dir == LV_DIR_BOTTOM && quick_settings_visible) {
        lv_obj_scroll_to_view(lv_scr_act(), LV_ANIM_ON);
        quick_settings_visible = false;
    } else if (dir == LV_DIR_BOTTOM && !notifications_visible && !quick_settings_visible) {
        lv_obj_clear_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_scroll_to_view(notifications_panel, LV_ANIM_ON);
        notifications_visible = true;
    } else if (dir == LV_DIR_TOP && notifications_visible) {
        lv_obj_scroll_to_view(lv_scr_act(), LV_ANIM_ON);
        notifications_visible = false;
    }
}

int main()
{
    lv_init();

    lvgl_display_init();

    // Explicitly initialize and set the default theme
    lv_disp_t * disp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_lighten(LV_PALETTE_GREY, 3), false, &lv_font_montserrat_14);
    lv_disp_set_theme(disp, theme);

    lv_obj_t* screen = lv_scr_act();
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);

    quick_settings_panel = lv_obj_create(screen);
    lv_obj_set_size(quick_settings_panel, lv_pct(100), lv_pct(100));
    create_quick_settings(quick_settings_panel);
    lv_obj_add_flag(quick_settings_panel, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t * main_container = lv_obj_create(screen);
    lv_obj_set_size(main_container, lv_pct(100), lv_pct(100));
    lv_obj_add_event_cb(main_container, swipe_event_handler, LV_EVENT_GESTURE, NULL);
    lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(main_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(main_container, 0, 0);

    lv_obj_t * tabview = lv_tabview_create(main_container, LV_DIR_TOP, 50);

    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Watchface");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Apps");

    create_watchface(tab1);
    create_app_list(tab2);

    notifications_panel = lv_obj_create(screen);
    lv_obj_set_size(notifications_panel, lv_pct(100), lv_pct(100));
    create_notifications_shade(notifications_panel);
    lv_obj_add_flag(notifications_panel, LV_OBJ_FLAG_HIDDEN);


    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}
