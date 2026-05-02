#include "app/fall_detection.h"
#include "app/phone_service.h"
#include "lvgl.h"

// --- Module-level State ---
static lv_obj_t * alert_screen = NULL;
static lv_obj_t * countdown_label = NULL;
static lv_timer_t * countdown_timer = NULL;
static int countdown_value = 30;

// --- Forward Declarations ---
static void countdown_timer_cb(lv_timer_t * timer);
static void im_ok_event_handler(lv_event_t * e);
static void call_help_event_handler(lv_event_t * e);
static void close_alert();

// --- Logic Implementation ---

static void close_alert() {
    if (countdown_timer) {
        lv_timer_del(countdown_timer);
        countdown_timer = NULL;
    }
    if (alert_screen) {
        lv_obj_del(alert_screen);
        alert_screen = NULL;
    }
}

static void countdown_timer_cb(lv_timer_t * timer) {
    (void)timer; // Unused
    countdown_value--;
    lv_label_set_text_fmt(countdown_label, "Calling for help in %d...", countdown_value);

    if (countdown_value <= 0) {
        // Time ran out, simulate call for help
        call_help_event_handler(NULL); 
    }
}

static void im_ok_event_handler(lv_event_t * e) {
    (void)e; // Unused
    close_alert();
}

static void call_help_event_handler(lv_event_t * e) {
    (void)e; // Unused
    phone_service_call_for_help();
    close_alert();
}


void show_fall_alert() {
    // Prevent showing multiple alerts
    if (alert_screen) {
        return;
    }

    countdown_value = 30; // Reset countdown

    // Create a modal full-screen object
    alert_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(alert_screen, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(alert_screen, lv_color_hex(0x8B0000), 0); // Dark Red
    lv_obj_set_flex_flow(alert_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(alert_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Warning Icon
    lv_obj_t * icon = lv_label_create(alert_screen);
    lv_label_set_text(icon, LV_SYMBOL_WARNING);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(icon, lv_color_white(), 0);

    // "Fall Detected" Text
    lv_obj_t * title = lv_label_create(alert_screen);
    lv_label_set_text(title, "Fall Detected");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_pad_top(title, 10, 0);

    // "Are you OK?" Text
    lv_obj_t * subtitle = lv_label_create(alert_screen);
    lv_label_set_text(subtitle, "Are you OK?");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0xCCCCCC), 0);
    lv_obj_set_style_pad_bottom(subtitle, 20, 0);

    // Countdown Label
    countdown_label = lv_label_create(alert_screen);
    lv_label_set_text_fmt(countdown_label, "Calling for help in %d...", countdown_value);
    lv_obj_set_style_text_font(countdown_label, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(countdown_label, lv_color_white(), 0);

    // "I'm OK" Button
    lv_obj_t * ok_btn = lv_btn_create(alert_screen);
    lv_obj_set_width(ok_btn, lv_pct(80));
    lv_obj_add_event_cb(ok_btn, im_ok_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(ok_btn, lv_color_hex(0x006400), 0); // Dark Green
    lv_obj_set_style_pad_top(ok_btn, 20, 0);
    lv_obj_t * ok_label = lv_label_create(ok_btn);
    lv_label_set_text(ok_label, "I'm OK");
    lv_obj_center(ok_label);

    // "Call for Help" Button
    lv_obj_t * help_btn = lv_btn_create(alert_screen);
    lv_obj_set_width(help_btn, lv_pct(80));
    lv_obj_add_event_cb(help_btn, call_help_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(help_btn, lv_color_hex(0xFF8C00), 0); // Dark Orange
    lv_obj_t * help_label = lv_label_create(help_btn);
    lv_label_set_text(help_label, "Call for Help");
    lv_obj_center(help_label);

    // Start the countdown timer
    countdown_timer = lv_timer_create(countdown_timer_cb, 1000, NULL);
}
