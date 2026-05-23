#include "app/web_api.h"
#include "app/dnd_service.h"

bool get_dnd_status() {
    return dnd_service_is_active();
}

void toggle_dnd_status() {
    dnd_service_toggle();
}
