#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "lvgl.h"

lv_obj_t* create_window(lv_obj_t* parent, const char* title, int width, int height);

#endif // UI_WINDOW_H
