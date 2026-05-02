// src/app/quick_settings.c
#include "lvgl.h"
#include "app/phone_service.h"

static void find_my_phone_event_handler(lv_event_t * e) {
    phone_service_find_my_phone();
}

void create_quick_settings(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x333333), 0);

    lv_obj_t * label = lv_label_create(panel);
    lv_label_set_text(label, "Quick Settings");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

    // Create a grid for the buttons
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    lv_obj_t * cont = lv_obj_create(panel);
    lv_obj_set_size(cont, lv_pct(90), LV_SIZE_CONTENT);
    lv_obj_center(cont);
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cont, 0, 0);

    // WiFi Button
    lv_obj_t * btn1 = lv_btn_create(cont);
    lv_obj_set_grid_cell(btn1, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t * btn_label1 = lv_label_create(btn1);
    lv_label_set_text(btn_label1, LV_SYMBOL_WIFI);
    lv_obj_center(btn_label1);

    // Bluetooth Button
    lv_obj_t * btn2 = lv_btn_create(cont);
    lv_obj_set_grid_cell(btn2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t * btn_label2 = lv_label_create(btn2);
    lv_label_set_text(btn_label2, LV_SYMBOL_BLUETOOTH);
    lv_obj_center(btn_label2);
    
    // Find My Phone Button
    lv_obj_t * btn3 = lv_btn_create(cont);
    lv_obj_set_grid_cell(btn3, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_add_event_cb(btn3, find_my_phone_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * btn_label3 = lv_label_create(btn3);
    lv_label_set_text(btn_label3, LV_SYMBOL_BELL);
    lv_obj_center(btn_label3);
}
