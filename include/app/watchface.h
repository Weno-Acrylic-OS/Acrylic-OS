// include/app/watchface.h
#ifndef WATCHFACE_H
#define WATCHFACE_H

#include "lvgl.h"

void create_watchface(lv_obj_t * parent);
void update_watchface_style(int style);
void update_watchface_widgets(bool show_steps, bool show_hr, bool show_date);
void redraw_watchface(void);
lv_obj_t * get_watchface_parent(void);

#endif // WATCHFACE_H
