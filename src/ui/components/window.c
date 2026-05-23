#include "ui/components/window.h"

static void drag_event_handler(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    lv_indev_t * indev = lv_indev_get_act();
    if(indev == NULL) return;

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    lv_obj_t* parent = lv_obj_get_parent(obj);

    lv_coord_t x = lv_obj_get_x(parent) + vect.x;
    lv_coord_t y = lv_obj_get_y(parent) + vect.y;

    lv_obj_set_pos(parent, x, y);
}

static void close_event_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * win = lv_obj_get_parent(lv_obj_get_parent(btn));
    lv_obj_del(win);
}

lv_obj_t* create_window(lv_obj_t* parent, const char* title, int width, int height) {
    lv_obj_t* win = lv_obj_create(parent);
    lv_obj_set_size(win, width, height);
    lv_obj_set_style_bg_color(win, lv_color_hex(0xcccccc), 0);
    lv_obj_set_style_border_color(win, lv_color_hex(0xaaaaaa), 0);
    lv_obj_set_style_border_width(win, 2, 0);
    lv_obj_set_style_radius(win, 5, 0);

    lv_obj_t* title_bar = lv_obj_create(win);
    lv_obj_set_size(title_bar, lv_pct(100), 30);
    lv_obj_align(title_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(title_bar, lv_color_hex(0x444444), 0);
    lv_obj_add_event_cb(title_bar, drag_event_handler, LV_EVENT_PRESSING, NULL);

    lv_obj_t* title_label = lv_label_create(title_bar);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_color(title_label, lv_color_white(), 0);
    lv_obj_center(title_label);

    lv_obj_t* close_btn = lv_btn_create(title_bar);
    lv_obj_set_size(close_btn, 20, 20);
    lv_obj_align(close_btn, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_add_event_cb(close_btn, close_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t* close_label = lv_label_create(close_btn);
    lv_label_set_text(close_label, LV_SYMBOL_CLOSE);
    lv_obj_center(close_label);
    
    return win;
}
