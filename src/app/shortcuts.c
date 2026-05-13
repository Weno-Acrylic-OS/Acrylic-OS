// src/app/shortcuts.c
#include <emscripten.h>
#include "app/shortcuts.h"
#include "app/navigation_service.h"
#include "app/weno_pay.h"
#include "app/messages.h"
#include "app/music.h"
#include "app/settings.h"

// TODO: This is a temporary solution for hiding the panel.
// The panel's visibility state (`shortcuts_visible` in main.c) is not updated,
// which will likely prevent other overlay panels (like Notifications) from opening
// after a shortcut has been used. A central state management solution is needed.
static void hide_current_panel(lv_event_t * e) {
    // Hierarchy: shortcuts_panel -> panel -> grid -> btn
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* grid = lv_obj_get_parent(btn);
    lv_obj_t* panel = lv_obj_get_parent(grid);
    lv_obj_t* shortcuts_panel = lv_obj_get_parent(panel);
    lv_obj_add_flag(shortcuts_panel, LV_OBJ_FLAG_HIDDEN);
}

static void open_weno_pay(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Weno Pay");
    hide_current_panel(e);
    navigation_service_navigate_to(create_weno_pay_app);
}

static void open_music(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Music");
    hide_current_panel(e);
    navigation_service_navigate_to(create_music_app);
}

static void open_messages(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Messages");
    hide_current_panel(e);
    navigation_service_navigate_to(create_messages_app);
}

static void open_settings(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Settings");
    hide_current_panel(e);
    navigation_service_navigate_to(create_settings_app);
}

// Helper function to create a button with an icon and a label
static lv_obj_t * create_shortcut_btn(lv_obj_t * parent, const char * icon_path, const char * text, lv_event_cb_t event_cb) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 80, 80);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * img = lv_img_create(btn);
    lv_img_set_src(img, icon_path);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);

    return btn;
}

void create_shortcuts_menu(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x4A4A4A), 0);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 20, 0);
    lv_obj_set_style_pad_top(panel, 40, 0);

    lv_obj_t * title = lv_label_create(panel);
    lv_label_set_text(title, "Shortcuts");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_width(title, lv_pct(100));
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);

    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t * grid = lv_obj_create(panel);
    lv_obj_set_size(grid, lv_pct(100), lv_pct(100));
    lv_obj_center(grid);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    lv_obj_set_style_pad_all(grid, 10, 0); // Padding around the grid
    lv_obj_set_style_pad_column(grid, 10, 0); // Padding between columns
    lv_obj_set_style_pad_row(grid, 10, 0); // Padding between rows

    lv_obj_t * btn_payment = create_shortcut_btn(grid, "assets/icons/weno_pay.png", "Weno Pay", open_weno_pay);
    lv_obj_set_grid_cell(btn_payment, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    lv_obj_t * btn_music = create_shortcut_btn(grid, "assets/icons/music.png", "Music", open_music);
    lv_obj_set_grid_cell(btn_music, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    lv_obj_t * btn_messages = create_shortcut_btn(grid, "assets/icons/messages.png", "Messages", open_messages);
    lv_obj_set_grid_cell(btn_messages, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    lv_obj_t * btn_settings = create_shortcut_btn(grid, "assets/icons/Settings.png", "Settings", open_settings);
    lv_obj_set_grid_cell(btn_settings, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
}
