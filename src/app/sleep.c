#include "app/sleep.h"
#include "lvgl.h"

static sleep_data_t mock_sleep_data = {
    .time_in_bed = 480, // 8 hours
    .time_asleep = 450, // 7.5 hours
    .rem_sleep = 90,
    .deep_sleep = 120,
    .light_sleep = 240
};

void create_sleep_app(lv_obj_t * parent) {
    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_center(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * title = lv_label_create(container);
    lv_label_set_text(title, "Sleep Analysis");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);

    char buf[64];

    lv_snprintf(buf, sizeof(buf), "Time in bed: %d hr %d min", mock_sleep_data.time_in_bed / 60, mock_sleep_data.time_in_bed % 60);
    lv_obj_t * time_in_bed_label = lv_label_create(container);
    lv_label_set_text(time_in_bed_label, buf);

    lv_snprintf(buf, sizeof(buf), "Time asleep: %d hr %d min", mock_sleep_data.time_asleep / 60, mock_sleep_data.time_asleep % 60);
    lv_obj_t * time_asleep_label = lv_label_create(container);
    lv_label_set_text(time_asleep_label, buf);

    lv_snprintf(buf, sizeof(buf), "Light sleep: %d hr %d min", mock_sleep_data.light_sleep / 60, mock_sleep_data.light_sleep % 60);
    lv_obj_t * light_sleep_label = lv_label_create(container);
    lv_label_set_text(light_sleep_label, buf);

    lv_snprintf(buf, sizeof(buf), "Deep sleep: %d hr %d min", mock_sleep_data.deep_sleep / 60, mock_sleep_data.deep_sleep % 60);
    lv_obj_t * deep_sleep_label = lv_label_create(container);
    lv_label_set_text(deep_sleep_label, buf);

    lv_snprintf(buf, sizeof(buf), "REM sleep: %d hr %d min", mock_sleep_data.rem_sleep / 60, mock_sleep_data.rem_sleep % 60);
    lv_obj_t * rem_sleep_label = lv_label_create(container);
    lv_label_set_text(rem_sleep_label, buf);
}
