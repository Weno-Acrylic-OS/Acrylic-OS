#include "app/dnd_service.h"
#include "app/persistence.h"
#include <stdio.h> // For sprintf

#ifdef SIMULATOR_BUILD
#include <emscripten.h>
#endif

static bool dnd_active_state = false;

void dnd_service_init(void) {
    // Load the persisted DND state, defaulting to 0 (false) if not found.
    dnd_active_state = persistence_get_int("dnd_is_active", 0);
}

void dnd_service_toggle(void) {
    dnd_active_state = !dnd_active_state;
    // Save the new state to persistence.
    persistence_set_int("dnd_is_active", dnd_active_state);

    #ifdef SIMULATOR_BUILD
    char script[64]; // Buffer to hold the script string
    sprintf(script, "window.notifyDndChanged(%d);", dnd_active_state);
    emscripten_run_script_int(script);
    #endif
}

bool dnd_service_is_active(void) {
    return dnd_active_state;
}
