#include "app/watchface_service.h"
#include <string.h>
#include <stdlib.h> // For NULL

#define MAX_WATCHFACES 10

// --- Module-level State ---
static watchface_descriptor_t registered_faces[MAX_WATCHFACES];
static int face_count = 0;
static int active_face_index = 0; // Default to the first registered face

// Forward declaration for the built-in watchface
void create_watchface(lv_obj_t * parent);

// --- Public API Implementation ---

void watchface_service_init() {
    face_count = 0;
    active_face_index = 0;

    // Register the default, built-in watch face
    static const watchface_descriptor_t default_face = {
        .name = "Default",
        .create_func = create_watchface
    };
    watchface_service_register(&default_face);
}

void watchface_service_register(const watchface_descriptor_t * descriptor) {
    if (face_count < MAX_WATCHFACES) {
        registered_faces[face_count] = *descriptor;
        face_count++;
    }
}

void watchface_service_render_active(lv_obj_t * parent) {
    if (face_count > 0 && active_face_index < face_count) {
        // Get the create function from the active descriptor
        watchface_create_func_t create_func = registered_faces[active_face_index].create_func;
        // Call it to draw the UI
        create_func(parent);
    } else {
        // Fallback if no faces are registered
        lv_obj_t * label = lv_label_create(parent);
        lv_label_set_text(label, "No Watch Face");
        lv_obj_center(label);
    }
}

bool watchface_service_set_active(const char * name) {
    for (int i = 0; i < face_count; i++) {
        if (strcmp(registered_faces[i].name, name) == 0) {
            active_face_index = i;
            // In a real app, you would save this choice to persistence here
            return true;
        }
    }
    return false; // Not found
}

const watchface_descriptor_t * watchface_service_get_faces(int *count) {
    *count = face_count;
    return registered_faces;
}
