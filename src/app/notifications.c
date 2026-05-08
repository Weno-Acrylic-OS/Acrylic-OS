// src/app/notifications.c
#include "lvgl.h"
#include "app/notification_service.h"

static void clear_all_event_handler(lv_event_t * e) {
    notification_service_clear_all();
    lv_obj_t * list = lv_event_get_user_data(e);
    lv_obj_clean(list);
    lv_list_add_text(list, "No new notifications");
}

void create_notifications_shade(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x222222), 0);

    lv_obj_t * title = lv_label_create(panel);
    lv_label_set_text(title, "Notifications");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * notification_list = lv_list_create(panel);
    lv_obj_set_size(notification_list, lv_pct(90), lv_pct(70));
    lv_obj_align(notification_list, LV_ALIGN_TOP_MID, 0, 40);

    int count = 0;
    const notification_t* notifications = notification_service_get_notifications(&count);

    if (count == 0) {
        lv_list_add_text(notification_list, "No new notifications");
    } else {
        for (int i = 0; i < count; i++) {
            lv_obj_t * btn = lv_list_add_btn(notification_list, NULL, notifications[i].title);
            lv_obj_t * label = lv_obj_get_child(btn, 0);
            lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

            lv_obj_t * body_label = lv_label_create(btn);
            lv_label_set_text(body_label, notifications[i].body);
            lv_obj_set_style_text_font(body_label, &lv_font_montserrat_12, 0);
            lv_obj_set_style_text_color(body_label, lv_color_hex(0x888888), 0);
        }
    }

    lv_obj_t * clear_btn = lv_btn_create(panel);
    lv_obj_align(clear_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(clear_btn, clear_all_event_handler, LV_EVENT_CLICKED, notification_list);
    lv_obj_t * clear_label = lv_label_create(clear_btn);
    lv_label_set_text(clear_label, "Clear All");
}
