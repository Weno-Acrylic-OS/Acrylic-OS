#include "lvgl.h"
#include "app/navigation_service.h"

static void back_event_handler(lv_event_t * e) {
    (void)e;
    navigation_service_go_back();
}

void create_weno_pay_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x000000), 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    lv_obj_t * card = lv_obj_create(parent);
    lv_obj_set_size(card, 200, 120);
    lv_obj_align(card, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_bg_color(card, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_bg_grad_color(card, lv_palette_darken(LV_PALETTE_BLUE, 3), 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_radius(card, 10, 0);

    lv_obj_t * card_chip = lv_obj_create(card);
    lv_obj_set_size(card_chip, 30, 20);
    lv_obj_align(card_chip, LV_ALIGN_TOP_LEFT, 20, 15);
    lv_obj_set_style_bg_color(card_chip, lv_color_hex(0xFFD700), 0);
    lv_obj_set_style_radius(card_chip, 4, 0);

    lv_obj_t * card_number = lv_label_create(card);
    lv_label_set_text(card_number, "**** **** **** 1234");
    lv_obj_set_style_text_color(card_number, lv_color_white(), 0);
    lv_obj_align(card_number, LV_ALIGN_BOTTOM_LEFT, 15, -35);

    lv_obj_t * card_name = lv_label_create(card);
    lv_label_set_text(card_name, "WENO USER");
    lv_obj_set_style_text_color(card_name, lv_color_white(), 0);
    lv_obj_align(card_name, LV_ALIGN_BOTTOM_LEFT, 15, -15);

    lv_obj_t * exp_date = lv_label_create(card);
    lv_label_set_text(exp_date, "12/28");
    lv_obj_set_style_text_color(exp_date, lv_color_white(), 0);
    lv_obj_align(exp_date, LV_ALIGN_BOTTOM_RIGHT, -15, -15);

    lv_obj_t * pay_btn = lv_btn_create(parent);
    lv_obj_align(pay_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_t * pay_label = lv_label_create(pay_btn);
    lv_label_set_text(pay_label, LV_SYMBOL_WIFI " Pay");
}
