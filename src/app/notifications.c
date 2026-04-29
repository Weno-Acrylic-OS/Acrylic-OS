// src/app/notifications.c
#include "lvgl.h"

void create_notifications_shade(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x222222), 0);

    lv_obj_t * label = lv_label_create(panel);
    lv_label_set_text(label, "Notifications");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * notification_list = lv_list_create(panel);
    lv_obj_set_size(notification_list, lv_pct(100), lv_pct(80));
    lv_obj_align(notification_list, LV_ALIGN_CENTER, 0, 20);

    lv_list_add_text(notification_list, "No new notifications");
}
