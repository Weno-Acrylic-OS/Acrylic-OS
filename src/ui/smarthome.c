#include "ui/smarthome.h"
#include "ui/ui_hal.h"

static void control_button_event_handler(lv_event_t * e) {
    lv_obj_t* btn = lv_event_get_target(e);
    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_obj_clear_state(btn, LV_STATE_CHECKED);
    } else {
        lv_obj_add_state(btn, LV_STATE_CHECKED);
    }
}

static void create_control_button(lv_obj_t* parent, const char* icon, const char* text) {
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 150, 100);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x555555), 0);
    lv_obj_set_style_border_width(btn, 2, 0);
    lv_obj_set_style_radius(btn, 10, 0);

    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_STATE_CHECKED);

    lv_obj_t* cont = lv_obj_create(btn);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cont, 0, 0);


    lv_obj_t* icon_label = lv_label_create(cont);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(icon_label, lv_color_white(), 0);

    lv_obj_t* text_label = lv_label_create(cont);
    lv_label_set_text(text_label, text);
    lv_obj_set_style_text_color(text_label, lv_color_white(), 0);

    lv_obj_add_event_cb(btn, control_button_event_handler, LV_EVENT_CLICKED, NULL);
}

void smarthome_deinit_ui() {
    // No resources to clean up
}

static void create_smarthome_ui(lv_obj_t* screen) {
    lv_obj_clean(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x111111), 0);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen, 20, 0);

    lv_obj_t* header = lv_label_create(screen);
    lv_label_set_text(header, "Smart Home Controls");
    lv_obj_set_style_text_font(header, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(header, lv_color_white(), 0);
    lv_obj_set_align(header, LV_ALIGN_TOP_LEFT);

    lv_obj_t* grid = lv_obj_create(screen);
    lv_obj_set_size(grid, lv_pct(100), lv_pct(90));
    lv_obj_align(grid, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    lv_obj_set_style_pad_all(grid, 10, 0);
    lv_obj_set_style_grid_column_dsc_array(grid, (const lv_coord_t[]){LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}, 0);
    lv_obj_set_style_grid_row_dsc_array(grid, (const lv_coord_t[]){LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}, 0);
    lv_obj_set_style_pad_row(grid, 20, 0);
    lv_obj_set_style_pad_column(grid, 20, 0);


    create_control_button(grid, LV_SYMBOL_POWER, "Living Room Lights");
    create_control_button(grid, LV_SYMBOL_POWER, "Kitchen Lights");
    create_control_button(grid, LV_SYMBOL_POWER, "Bedroom Lights");
    
    create_control_button(grid, LV_SYMBOL_SETTINGS, "Thermostat");
    create_control_button(grid, LV_SYMBOL_VIDEO, "Security Cameras");
    create_control_button(grid, LV_SYMBOL_AUDIO, "Music");
}

static ui_personality_t smarthome_personality = {
    .init_ui = create_smarthome_ui,
    .deinit_ui = smarthome_deinit_ui
};

ui_personality_t* get_smarthome_personality() {
    return &smarthome_personality;
}
