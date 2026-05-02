#include "app/weather.h"
#include "lvgl.h"

static weather_data_t mock_weather_data = {
    .temperature = 75,
    .condition = "Sunny",
    .icon = LV_SYMBOL_DUMMY // Placeholder, will use text
};

void create_weather_app(lv_obj_t * parent) {
    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_center(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Temperature Label
    lv_obj_t * temp_label = lv_label_create(container);
    char temp_str[10];
    lv_snprintf(temp_str, sizeof(temp_str), "%d°F", mock_weather_data.temperature);
    lv_label_set_text(temp_label, temp_str);
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_48, 0);

    // Condition Label
    lv_obj_t * condition_label = lv_label_create(container);
    lv_label_set_text(condition_label, mock_weather_data.condition);
    lv_obj_set_style_text_font(condition_label, &lv_font_montserrat_24, 0);

    // TODO: Add forecast data
}
