#include "app/persistence.h"

static bool oobe_completed = false;

void persistence_init() {
    // In a real application, this would load the flag from non-volatile memory.
}

bool persistence_get_oobe_completed() {
    return oobe_completed;
}

void persistence_set_oobe_completed(bool completed) {
    oobe_completed = completed;
    // In a real application, this would save the flag to non-volatile memory.
}
