#ifndef UI_DATALOCK_SCREEN_H
#define UI_DATALOCK_SCREEN_H

#include "lvgl.h"

void create_ui_datalock_screen(lv_obj_t * parent, void (*unlock_cb)(void));

#endif // UI_DATALOCK_SCREEN_H
