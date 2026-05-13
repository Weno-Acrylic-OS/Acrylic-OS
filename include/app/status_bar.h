// src/app/status_bar.h
#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "lvgl.h"
#include <stdbool.h>

void create_status_bar(lv_obj_t * parent);
void update_time_label(const char * time_text);
void status_bar_show_workout_indicator(bool show);
void status_bar_update_privacy_indicators();
void status_bar_show_dnd_indicator(bool show);

#endif // STATUS_BAR_H
