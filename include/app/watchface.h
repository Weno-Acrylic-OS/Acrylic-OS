// include/app/watchface.h
#ifndef WATCHFACE_H
#define WATCHFACE_H

#include "lvgl.h"

void create_watchface(lv_obj_t * parent);
void update_watchface_style(int style);
void update_watchface_widgets(bool show_steps, bool show_hr, bool show_date);
void redraw_watchface(void);
lv_obj_t * get_watchface_parent(void);

// New functions for Watch Face Studio v2
lv_obj_t * watchface_get_time_label(void);
lv_obj_t * watchface_get_date_label(void);

void watchface_set_time_font(const lv_font_t *font);
void watchface_set_time_color(lv_color_t color);
void watchface_set_date_color(lv_color_t color);
void watchface_set_wallpaper(const char *path);

#endif // WATCHFACE_H
