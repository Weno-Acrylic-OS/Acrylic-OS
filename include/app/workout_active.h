#ifndef WORKOUT_ACTIVE_H
#define WORKOUT_ACTIVE_H

#include "lvgl.h"

/**
 * @brief Creates the active workout screen UI.
 * @param parent The parent object for the screen.
 * @param workout_type A string describing the workout type that was selected.
 */
void create_workout_active_screen(lv_obj_t * parent, const char* workout_type);

#endif // WORKOUT_ACTIVE_H
