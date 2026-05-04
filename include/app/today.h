// include/app/today.h
#ifndef TODAY_H
#define TODAY_H

#include "lvgl.h"

void create_today_view(lv_obj_t * parent);
void today_register_native_widgets();
void today_view_update(void);

#endif // TODAY_H
