// src/app/today.c
#include "app/today.h"

void create_today_view(lv_obj_t * parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 10, 0);

    lv_obj_t * title_label = lv_label_create(parent);
    lv_label_set_text(title_label, "Today's Summary");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_22, 0);
    lv_obj_set_style_pad_top(title_label, 10, 0);
    lv_obj_set_style_pad_bottom(title_label, 10, 0);
    lv_obj_set_width(title_label, lv_pct(100));
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);

    // Fitness Section
    lv_obj_t * fitness_panel = lv_obj_create(parent);
    lv_obj_set_size(fitness_panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(fitness_panel, lv_color_hex(0x4CAF50), 0); // Green
    lv_obj_set_style_border_width(fitness_panel, 0, 0);
    lv_obj_set_style_radius(fitness_panel, 10, 0);
    lv_obj_set_style_pad_all(fitness_panel, 10, 0);
    lv_obj_set_flex_flow(fitness_panel, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * fitness_title = lv_label_create(fitness_panel);
    lv_label_set_text(fitness_title, "Activity");
    lv_obj_set_style_text_font(fitness_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(fitness_title, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t * fitness_data = lv_label_create(fitness_panel);
    lv_label_set_text(fitness_data, "Steps: 8,500 | Calories: 350 | Distance: 6.2 km");
    lv_obj_set_style_text_color(fitness_data, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(fitness_data, &lv_font_montserrat_14, 0);

    // Heart Rate Section
    lv_obj_t * hr_panel = lv_obj_create(parent);
    lv_obj_set_size(hr_panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(hr_panel, lv_color_hex(0xF44336), 0); // Red
    lv_obj_set_style_border_width(hr_panel, 0, 0);
    lv_obj_set_style_radius(hr_panel, 10, 0);
    lv_obj_set_style_pad_all(hr_panel, 10, 0);
    lv_obj_set_flex_flow(hr_panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_top(hr_panel, 10, 0);

    lv_obj_t * hr_title = lv_label_create(hr_panel);
    lv_label_set_text(hr_title, "Heart Rate");
    lv_obj_set_style_text_font(hr_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(hr_title, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t * hr_data = lv_label_create(hr_panel);
    lv_label_set_text(hr_data, "Avg: 72 bpm | Current: 78 bpm");
    lv_obj_set_style_text_color(hr_data, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(hr_data, &lv_font_montserrat_14, 0);

    // Sleep Section
    lv_obj_t * sleep_panel = lv_obj_create(parent);
    lv_obj_set_size(sleep_panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(sleep_panel, lv_color_hex(0x2196F3), 0); // Blue
    lv_obj_set_style_border_width(sleep_panel, 0, 0);
    lv_obj_set_style_radius(sleep_panel, 10, 0);
    lv_obj_set_style_pad_all(sleep_panel, 10, 0);
    lv_obj_set_flex_flow(sleep_panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_top(sleep_panel, 10, 0);

    lv_obj_t * sleep_title = lv_label_create(sleep_panel);
    lv_label_set_text(sleep_title, "Sleep");
    lv_obj_set_style_text_font(sleep_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(sleep_title, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t * sleep_data = lv_label_create(sleep_panel);
    lv_label_set_text(sleep_data, "Last Night: 7h 30m | Deep: 2h 15m");
    lv_obj_set_style_text_color(sleep_data, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(sleep_data, &lv_font_montserrat_14, 0);
}
