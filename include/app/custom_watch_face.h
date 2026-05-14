#ifndef CUSTOM_WATCH_FACE_H
#define CUSTOM_WATCH_FACE_H

#include "lvgl.h"

#define MAX_WIDGETS 4
#define MAX_WALLPAPER_PATH_LEN 64

typedef enum {
    WIDGET_TYPE_NONE = -1,
    WIDGET_TYPE_STEPS,
    WIDGET_TYPE_HEART_RATE,
    // Add more widget types here
    WIDGET_TYPE_COUNT
} widget_type_t;

typedef struct {
    int font_index;
    lv_color_t time_color;
    lv_color_t date_color;
    widget_type_t widgets[MAX_WIDGETS];
    char wallpaper_path[MAX_WALLPAPER_PATH_LEN];
} custom_watch_face_t;

#endif // CUSTOM_WATCH_FACE_H
