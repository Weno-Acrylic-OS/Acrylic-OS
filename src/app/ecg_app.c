#include "app/ecg_app.h"
#include "drivers/ecg_afe.h"
#include "app/navigation_service.h" // For the back button

// App state
static lv_obj_t* ecg_app_screen;
static lv_obj_t* chart;
static lv_chart_series_t* series;
static lv_obj_t* status_label;
static lv_obj_t* countdown_label;
static lv_timer_t* countdown_timer;
static int_fast8_t countdown_value;

// Forward declarations
static void start_button_event_handler(lv_event_t * e);
static void back_button_event_handler(lv_event_t * e);
static void ecg_data_handler(int16_t* samples, uint16_t count);
static void countdown_timer_cb(lv_timer_t* timer);
static void cleanup_ecg_app(void);

static void cleanup_ecg_app() {
    ecg_afe_stop_reading();
    ecg_afe_register_callback(NULL);
    if(countdown_timer) {
        lv_timer_del(countdown_timer);
        countdown_timer = NULL;
    }
}

static void back_button_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        cleanup_ecg_app();
        navigation_service_go_back();
    }
}

static void countdown_timer_cb(lv_timer_t* timer) {
    countdown_value--;
    lv_label_set_text_fmt(countdown_label, "%ds", countdown_value);

    if (countdown_value <= 0) {
        // Stop reading and show result
        ecg_afe_stop_reading();
        lv_timer_del(timer);
        countdown_timer = NULL;
        lv_obj_add_flag(chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(status_label, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(status_label, "Analysis Complete:\n"
                                      "Normal Sinus Rhythm\n"
                                      "(mock result)");
    }
}

static void ecg_data_handler(int16_t* samples, uint16_t count) {
    for (uint16_t i = 0; i < count; i++) {
        lv_chart_set_next_value(chart, series, samples[i]);
    }
    lv_chart_refresh(chart);
}

static void start_button_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        lv_obj_t* btn = lv_event_get_target(e);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_HIDDEN); // Hide start button
        lv_obj_add_flag(status_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(chart, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(countdown_label, LV_OBJ_FLAG_HIDDEN);

        countdown_value = 30;
        lv_label_set_text_fmt(countdown_label, "%ds", countdown_value);

        ecg_afe_register_callback(ecg_data_handler);
        ecg_afe_start_reading();

        countdown_timer = lv_timer_create(countdown_timer_cb, 1000, NULL);
    }
}

void create_ecg_app(lv_obj_t* parent) {
    ecg_afe_init();
    
    // Create a screen for the app
    ecg_app_screen = parent;
    lv_obj_set_flex_flow(ecg_app_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ecg_app_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Back button
    lv_obj_t * back_btn = lv_btn_create(ecg_app_screen);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    // Status Label
    status_label = lv_label_create(ecg_app_screen);
    lv_label_set_text(status_label, "Place and hold your finger on the\n"
                                      "right-side button sensor to begin the ECG reading.");
    lv_obj_set_width(status_label, lv_pct(80));
    lv_label_set_long_mode(status_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(status_label, LV_TEXT_ALIGN_CENTER, 0);

    // Countdown Label
    countdown_label = lv_label_create(ecg_app_screen);
    lv_obj_set_style_text_font(countdown_label, &lv_font_montserrat_24, 0);
    lv_obj_add_flag(countdown_label, LV_OBJ_FLAG_HIDDEN);

    // Chart
    chart = lv_chart_create(ecg_app_screen);
    lv_obj_set_size(chart, 200, 100);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -1500, 1500);
    lv_chart_set_point_count(chart, 200);
    series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_obj_add_flag(chart, LV_OBJ_FLAG_HIDDEN);

    // Start Button
    lv_obj_t * start_btn = lv_btn_create(ecg_app_screen);
    lv_obj_add_event_cb(start_btn, start_button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(start_btn);
    lv_label_set_text(label, "Start Reading");
}
