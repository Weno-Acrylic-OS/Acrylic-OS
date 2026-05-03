#include "app/today_service.h"
#include <stdlib.h>

#define MAX_WIDGETS 20

static const today_widget_descriptor_t* registered_widgets[MAX_WIDGETS];
static int num_registered_widgets = 0;

void today_service_init() {
    num_registered_widgets = 0;
}

void today_service_register_widget(const today_widget_descriptor_t * descriptor) {
    if (descriptor && num_registered_widgets < MAX_WIDGETS) {
        registered_widgets[num_registered_widgets++] = descriptor;
    }
}

void today_service_render_widgets(lv_obj_t * parent) {
    for (int i = 0; i < num_registered_widgets; i++) {
        if (registered_widgets[i] && registered_widgets[i]->create_func) {
            // We could add a container for each widget here to give them
            // a consistent look and feel (e.g. a card background).
            // For now, just call the create function directly on the parent.
            registered_widgets[i]->create_func(parent);
        }
    }
}
