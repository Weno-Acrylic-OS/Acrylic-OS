// src/app/shortcuts.c
#include <emscripten.h>
#include "app/shortcuts.h"
#include "app/navigation_service.h"
#include "app/weno_pay.h"
#include "app/messages.h"
#include "app/music.h"
#include "app/settings.h"

static void open_weno_pay(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Weno Pay");
    lv_obj_add_flag(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))), LV_OBJ_FLAG_HIDDEN);
    navigation_service_navigate_to(create_weno_pay_app);
}

static void open_music(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Music");
    lv_obj_add_flag(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))), LV_OBJ_FLAG_HIDDEN);
    navigation_service_navigate_to(create_music_app);
}

static void open_messages(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Messages");
    lv_obj_add_flag(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))), LV_OBJ_FLAG_HIDDEN);
    navigation_service_navigate_to(create_messages_app);
}

static void open_settings(lv_event_t * e) {
    emscripten_log(EM_LOG_CONSOLE, "Opening Settings");
    lv_obj_add_flag(lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))), LV_OBJ_FLAG_HIDDEN);
    navigation_service_navigate_to(create_settings_app);
}

void create_shortcuts_menu(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x02F7BE), 0);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 20, 0);

    lv_obj_t * title = lv_label_create(panel);
    lv_label_set_text(title, "Shortcuts");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_pad_bottom(title, 20, 0);
    lv_obj_set_width(title, lv_pct(100));
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_t * btn_payment = lv_btn_create(panel);
    lv_obj_set_width(btn_payment, lv_pct(100));
    lv_obj_add_event_cb(btn_payment, open_weno_pay, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_payment = lv_label_create(btn_payment);
    lv_label_set_text(label_payment, "Weno Pay");

    lv_obj_t * btn_music = lv_btn_create(panel);
    lv_obj_set_width(btn_music, lv_pct(100));
    lv_obj_add_event_cb(btn_music, open_music, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_music = lv_label_create(btn_music);
    lv_label_set_text(label_music, "Music");

    lv_obj_t * btn_messages = lv_btn_create(panel);
    lv_obj_set_width(btn_messages, lv_pct(100));
    lv_obj_add_event_cb(btn_messages, open_messages, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_messages = lv_label_create(btn_messages);
    lv_label_set_text(label_messages, "Messages");

    lv_obj_t * btn_settings = lv_btn_create(panel);
    lv_obj_set_width(btn_settings, lv_pct(100));
    lv_obj_add_event_cb(btn_settings, open_settings, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_settings = lv_label_create(btn_settings);
    lv_label_set_text(label_settings, "Settings");
}
