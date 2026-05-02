// src/app/voice_assistant.c
#include "app/voice_assistant.h"

static lv_obj_t * status_label;
static lv_obj_t * response_label;
static lv_timer_t * timer;

static void voice_assistant_timer(lv_timer_t * t)
{
    int32_t * step = (int32_t *)t->user_data;

    switch (*step) {
        case 0:
            lv_label_set_text(status_label, "Processing...");
            break;
        case 1:
            lv_label_set_text(status_label, "Here's what I found:");
            lv_label_set_text(response_label, "Weno Fit OS is a real-time operating system for smart health trackers.");
            lv_obj_clear_flag(response_label, LV_OBJ_FLAG_HIDDEN);
            break;
        case 2:
            lv_label_set_text(status_label, "Listening...");
            lv_obj_add_flag(response_label, LV_OBJ_FLAG_HIDDEN);
            lv_timer_del(timer);
            break;
    }

    (*step)++;
}

void simulate_voice_command(lv_event_t * e)
{
    static int32_t step = 0;
    step = 0;
    timer = lv_timer_create(voice_assistant_timer, 1000, &step);
}

void create_voice_assistant(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_70, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_radius(panel, 0, 0);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 20, 0);

    status_label = lv_label_create(panel);
    lv_label_set_text(status_label, "Listening...");
    lv_obj_set_style_text_color(status_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_22, 0);
    lv_obj_align(status_label, LV_ALIGN_CENTER, 0, -40);

    response_label = lv_label_create(panel);
    lv_label_set_text(response_label, "");
    lv_obj_set_style_text_color(response_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(response_label, &lv_font_montserrat_16, 0);
    lv_obj_align(response_label, LV_ALIGN_CENTER, 0, 20);
    lv_label_set_long_mode(response_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(response_label, lv_pct(80));
    lv_obj_set_style_text_align(response_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_flag(response_label, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t * btn = lv_btn_create(panel);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_event_cb(btn, simulate_voice_command, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Simulate Command");
}
