// src/app/today.c
#include "app/today.h"
#include "app/today_service.h"
#include "app/spo2_service.h"
#include "app/temperature_service.h"
#include <stdio.h>

// --- Widget Labels ---
static lv_obj_t * spo2_data_label = NULL;
static lv_obj_t * temp_data_label = NULL;

// --- Native Widget Implementations ---

static void create_fitness_widget(lv_obj_t * parent) {
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
}

static void create_hr_widget(lv_obj_t * parent) {
    lv_obj_t * hr_panel = lv_obj_create(parent);
    lv_obj_set_size(hr_panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(hr_panel, lv_color_hex(0xF44336), 0); // Red
    lv_obj_set_style_border_width(hr_panel, 0, 0);
    lv_obj_set_style_radius(hr_panel, 10, 0);
    lv_obj_set_style_pad_all(hr_panel, 10, 0);
    lv_obj_set_flex_flow(hr_panel, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * hr_title = lv_label_create(hr_panel);
    lv_label_set_text(hr_title, "Heart Rate");
    lv_obj_set_style_text_font(hr_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(hr_title, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t * hr_data = lv_label_create(hr_panel);
    lv_label_set_text(hr_data, "Avg: 72 bpm | Current: 78 bpm");
    lv_obj_set_style_text_color(hr_data, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(hr_data, &lv_font_montserrat_14, 0);
}

static void create_sleep_widget(lv_obj_t * parent) {
    lv_obj_t * sleep_panel = lv_obj_create(parent);
    lv_obj_set_size(sleep_panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(sleep_panel, lv_color_hex(0x2196F3), 0); // Blue
    lv_obj_set_style_border_width(sleep_panel, 0, 0);
    lv_obj_set_style_radius(sleep_panel, 10, 0);
    lv_obj_set_style_pad_all(sleep_panel, 10, 0);
    lv_obj_set_flex_flow(sleep_panel, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * sleep_title = lv_label_create(sleep_panel);
    lv_label_set_text(sleep_title, "Sleep");
    lv_obj_set_style_text_font(sleep_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(sleep_title, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t * sleep_data = lv_label_create(sleep_panel);
    lv_label_set_text(sleep_data, "Last Night: 7h 30m | Deep: 2h 15m");
    lv_obj_set_style_text_color(sleep_data, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(sleep_data, &lv_font_montserrat_14, 0);
}

static void create_spo2_widget(lv_obj_t * parent) {
    lv_obj_t * spo2_panel = lv_obj_create(parent);
    lv_obj_set_size(spo2_panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(spo2_panel, lv_color_hex(0x673AB7), 0); // Deep Purple
    lv_obj_set_style_border_width(spo2_panel, 0, 0);
    lv_obj_set_style_radius(spo2_panel, 10, 0);
    lv_obj_set_style_pad_all(spo2_panel, 10, 0);
    lv_obj_set_flex_flow(spo2_panel, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * spo2_title = lv_label_create(spo2_panel);
    lv_label_set_text(spo2_title, "Blood Oxygen");
    lv_obj_set_style_text_font(spo2_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(spo2_title, lv_color_hex(0xFFFFFF), 0);

    spo2_data_label = lv_label_create(spo2_panel);
    lv_label_set_text(spo2_data_label, "Latest: --%");
    lv_obj_set_style_text_color(spo2_data_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(spo2_data_label, &lv_font_montserrat_14, 0);
}

static void create_temperature_widget(lv_obj_t * parent) {
    lv_obj_t * temp_panel = lv_obj_create(parent);
    lv_obj_set_size(temp_panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(temp_panel, lv_color_hex(0xFABE7A), 0); // Orange
    lv_obj_set_style_border_width(temp_panel, 0, 0);
    lv_obj_set_style_radius(temp_panel, 10, 0);
    lv_obj_set_style_pad_all(temp_panel, 10, 0);
    lv_obj_set_flex_flow(temp_panel, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * temp_title = lv_label_create(temp_panel);
    lv_label_set_text(temp_title, "Skin Temperature");
    lv_obj_set_style_text_font(temp_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(temp_title, lv_color_hex(0xFFFFFF), 0);

    temp_data_label = lv_label_create(temp_panel);
    lv_label_set_text(temp_data_label, "Trend: -- C");
    lv_obj_set_style_text_color(temp_data_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(temp_data_label, &lv_font_montserrat_14, 0);
}

// --- Native Widget Registration ---

static const today_widget_descriptor_t fitness_widget_desc = {
    .name = "Fitness",
    .create_func = create_fitness_widget,
};

static const today_widget_descriptor_t hr_widget_desc = {
    .name = "Heart Rate",
    .create_func = create_hr_widget,
};

static const today_widget_descriptor_t sleep_widget_desc = {
    .name = "Sleep",
    .create_func = create_sleep_widget,
};

static const today_widget_descriptor_t spo2_widget_desc = {
    .name = "SpO2",
    .create_func = create_spo2_widget,
};

static const today_widget_descriptor_t temp_widget_desc = {
    .name = "Temperature",
    .create_func = create_temperature_widget,
};

void today_register_native_widgets() {
    today_service_register_widget(&fitness_widget_desc);
    today_service_register_widget(&hr_widget_desc);
    today_service_register_widget(&sleep_widget_desc);
    today_service_register_widget(&spo2_widget_desc);
    today_service_register_widget(&temp_widget_desc);
}

// --- Main View Creation ---

void create_today_view(lv_obj_t * parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 10, 0);
    lv_obj_set_style_pad_row(parent, 10, 0); // Add spacing between widgets

    lv_obj_t * title_label = lv_label_create(parent);
    lv_label_set_text(title_label, "Today's Summary");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_22, 0);
    lv_obj_set_style_pad_top(title_label, 10, 0);
    lv_obj_set_style_pad_bottom(title_label, 10, 0);
    lv_obj_set_width(title_label, lv_pct(100));
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);

    // Render all registered widgets
    today_service_render_widgets(parent);
}

// --- View Update ---
void today_view_update(void) {
    if (spo2_data_label) {
        uint8_t spo2_val = spo2_service_get_latest_value();
        char buffer[32];
        if (spo2_val > 0) {
            snprintf(buffer, sizeof(buffer), "Latest: %u%%", spo2_val);
        } else {
            snprintf(buffer, sizeof(buffer), "Latest: --%%");
        }
        lv_label_set_text(spo2_data_label, buffer);
    }

    if (temp_data_label) {
        float temp_val = temperature_service_get_latest_value();
        char buffer[32];
        if (temp_val > 0.0f) {
            snprintf(buffer, sizeof(buffer), "Trend: %.1f C", temp_val);
        } else {
            snprintf(buffer, sizeof(buffer), "Trend: -- C");
        }
        lv_label_set_text(temp_data_label, buffer);
    }
}
