#ifndef UI_HAL_H
#define UI_HAL_H

#include "lvgl.h"

typedef struct {
    void (*init_ui)(lv_obj_t* screen);
    void (*deinit_ui)(void);
} ui_personality_t;

#endif // UI_HAL_H
