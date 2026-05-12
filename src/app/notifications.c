// src/app/notifications.c
#include "lvgl.h"
#include "app/notification_service.h"

static lv_obj_t * notification_list_cont; // Container for the cards

// Event handler for the "Clear All" button
static void clear_all_event_handler(lv_event_t * e) {
    (void)e;
    notification_service_clear_all();

    // Delete all notification cards and show the "No notifications" label
    if (notification_list_cont != NULL) {
        lv_obj_clean(notification_list_cont);
        lv_obj_t * label = lv_label_create(notification_list_cont);
        lv_label_set_text(label, "No New Notifications");
        lv_obj_set_style_text_color(label, lv_color_hex(0x888888), 0);
        lv_obj_center(label);
    }
}

// Redraws all the notification cards in the list
static void redraw_notification_list() {
    if (notification_list_cont == NULL) return;
    lv_obj_clean(notification_list_cont);

    int count = 0;
    const notification_t* notifications = notification_service_get_notifications(&count);

    if (count == 0) {
        lv_obj_t * label = lv_label_create(notification_list_cont);
        lv_label_set_text(label, "No New Notifications");
        lv_obj_set_style_text_color(label, lv_color_hex(0x888888), 0);
        lv_obj_center(label);
    } else {
        // Create a card for each notification
        for (int i = 0; i < count; i++) {
            lv_obj_t * card = lv_obj_create(notification_list_cont);
            lv_obj_set_width(card, lv_pct(100));
            lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
            lv_obj_set_style_pad_all(card, 5, 0);

            // App Name Label
            lv_obj_t * app_label = lv_label_create(card);
            lv_label_set_text(app_label, notifications[i].app_name);
            lv_obj_set_style_text_font(app_label, &lv_font_montserrat_12, 0);
            lv_obj_set_style_text_color(app_label, lv_color_hex(0xaaaaaa), 0);

            // Title Label
            lv_obj_t * title_label = lv_label_create(card);
            lv_label_set_text(title_label, notifications[i].title);
            lv_obj_set_style_text_font(title_label, &lv_font_montserrat_16, 0);
            lv_obj_set_style_text_color(title_label, lv_color_white(), 0);

            // Body Label
            lv_obj_t * body_label = lv_label_create(card);
            lv_label_set_text(body_label, notifications[i].body);
            lv_label_set_long_mode(body_label, LV_LABEL_LONG_WRAP);
            lv_obj_set_width(body_label, lv_pct(100));
            lv_obj_set_style_text_font(body_label, &lv_font_montserrat_14, 0);
            lv_obj_set_style_text_color(body_label, lv_color_hex(0xcccccc), 0);
        }
    }
}

// Callback for the notification service, though the service doesn't support it yet.
// If it did, this is where we would call redraw_notification_list()
static void on_new_notification(void) {
    redraw_notification_list();
}


void create_notifications_shade(lv_obj_t * parent) {
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x111111), 0);
    lv_obj_set_style_pad_all(parent, 5, 0);

    // --- Top Bar: Title and Clear Button ---
    lv_obj_t * top_bar = lv_obj_create(parent);
    lv_obj_set_width(top_bar, lv_pct(100));
    lv_obj_set_height(top_bar, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(top_bar, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(top_bar, 0, 0);
    lv_obj_set_style_pad_all(top_bar, 5, 0);

    lv_obj_t * title = lv_label_create(top_bar);
    lv_label_set_text(title, "Notifications");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t * clear_btn = lv_btn_create(top_bar);
    lv_obj_align(clear_btn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(clear_btn, clear_all_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * clear_label = lv_label_create(clear_btn);
    lv_label_set_text(clear_label, "Clear All");

    // --- Notification List ---
    notification_list_cont = lv_obj_create(parent);
    lv_obj_set_width(notification_list_cont, lv_pct(100));
    lv_obj_set_layout(notification_list_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(notification_list_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(notification_list_cont, 1);
    lv_obj_set_style_pad_all(notification_list_cont, 5, 0);

    // Initial drawing of the list
    redraw_notification_list();
}
