// src/app/voice_assistant.c
#include "app/voice_assistant.h"
#include "app/watchface.h"
#include "drivers/heart_rate.h"
#include "app/gamification_service.h"
#include <stdio.h>
#include <string.h>

static lv_obj_t * status_label;
static lv_obj_t * response_label;
static lv_obj_t * command_textarea;

static void close_voice_assistant_event_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_current_target(e);
    lv_obj_t * panel = lv_obj_get_parent(btn);
    create_watchface(lv_obj_get_parent(panel));
}

static void parse_and_execute_command(const char* command) {
    char response[128];
    if (strcmp(command, "what's my heart rate?") == 0) {
        uint8_t bpm = heart_rate_get_bpm();
        snprintf(response, sizeof(response), "Your heart rate is %d bpm.", bpm);
    } else if (strcmp(command, "what are my steps?") == 0) {
        int steps = gamification_get_step_count();
        snprintf(response, sizeof(response), "You have taken %d steps today.", steps);
    } else {
        snprintf(response, sizeof(response), "Sorry, I don't understand the command: \"%s\"", command);
    }

    lv_label_set_text(response_label, response);
    lv_obj_clear_flag(response_label, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(status_label, "Here's what I found:");
}

static void send_command_event_handler(lv_event_t * e) {
    const char * command = lv_textarea_get_text(command_textarea);
    parse_and_execute_command(command);
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

    lv_obj_t * close_btn = lv_btn_create(panel);
    lv_obj_align(close_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(close_btn, close_voice_assistant_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * close_label = lv_label_create(close_btn);
    lv_label_set_text(close_label, LV_SYMBOL_CLOSE);

    status_label = lv_label_create(panel);
    lv_label_set_text(status_label, "Enter a command:");
    lv_obj_set_style_text_color(status_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_22, 0);
    lv_obj_align(status_label, LV_ALIGN_CENTER, 0, -80);

    command_textarea = lv_textarea_create(panel);
    lv_textarea_set_one_line(command_textarea, true);
    lv_textarea_set_placeholder_text(command_textarea, "e.g., what's my heart rate?");
    lv_obj_align(command_textarea, LV_ALIGN_CENTER, 0, -40);

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
    lv_obj_add_event_cb(btn, send_command_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Send Command");
}
