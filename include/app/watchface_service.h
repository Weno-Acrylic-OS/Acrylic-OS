#ifndef WATCHFACE_SERVICE_H
#define WATCHFACE_SERVICE_H

#include "lvgl.h"

// A function pointer type for creating the watch face UI
typedef void (*watchface_create_func_t)(lv_obj_t * parent);

// Describes a single watch face
typedef struct {
    const char * name;
    watchface_create_func_t create_func;
    // We could add a preview image here later
} watchface_descriptor_t;

/**
 * @brief Initializes the watch face service.
 */
void watchface_service_init();

/**
 * @brief Registers a new watch face with the service.
 * @param descriptor A pointer to the watch face descriptor.
 */
void watchface_service_register(const watchface_descriptor_t * descriptor);

/**
 * @brief Renders the currently active watch face.
 * @param parent The parent object to draw the watch face on.
 */
void watchface_service_render_active(lv_obj_t * parent);

/**
 * @brief Sets the active watch face by name.
 * @param name The name of the watch face to set as active.
 * @return True if the watch face was found and set, false otherwise.
 */
bool watchface_service_set_active(const char * name);

/**
 * @brief Gets the list of all registered watch faces.
 * @param count A pointer to an integer that will be filled with the number of faces.
 * @return A constant pointer to the array of watch face descriptors.
 */
const watchface_descriptor_t * watchface_service_get_faces(int *count);


#endif // WATCHFACE_SERVICE_H
