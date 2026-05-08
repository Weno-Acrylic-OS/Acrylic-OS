#include "lvgl.h"
#include "app/navigation_service.h"

static void back_event_handler(lv_event_t * e) {
    (void)e;
    navigation_service_go_back();
}

static void keyboard_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * kb = lv_event_get_target(e);
    if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

void create_messages_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x000000), 0);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * recipient_input = lv_textarea_create(parent);
    lv_textarea_set_one_line(recipient_input, true);
    lv_obj_set_width(recipient_input, lv_pct(80));
    lv_textarea_set_placeholder_text(recipient_input, "To:");
    lv_obj_align(recipient_input, LV_ALIGN_TOP_MID, 0, 50);

    lv_obj_t * text_area = lv_textarea_create(parent);
    lv_obj_set_size(text_area, lv_pct(80), 100);
    lv_textarea_set_placeholder_text(text_area, "Message...");
    lv_obj_align(text_area, LV_ALIGN_TOP_MID, 0, 90);

    lv_obj_t * send_btn = lv_btn_create(parent);
    lv_obj_align(send_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_t * send_label = lv_label_create(send_btn);
    lv_label_set_text(send_label, "Send");

    lv_obj_t * kb = lv_keyboard_create(parent);
    lv_keyboard_set_textarea(kb, text_area);
    lv_obj_add_event_cb(kb, keyboard_event_handler, LV_EVENT_ALL, NULL);
}
