// include/app/fitness.h
#ifndef FITNESS_H
#define FITNESS_H

#include "lvgl.h"
#include <stdbool.h>

/**
 * @brief Checks if a workout is currently in progress.
 * @return True if a workout is active, false otherwise.
 */
bool fitness_app_is_active();

/**
 * @brief Creates the fitness application UI.
 * @param parent The parent object for the UI.
 */
void create_fitness_app(lv_obj_t * parent);

#endif // FITNESS_H
