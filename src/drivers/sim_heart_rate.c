#include "drivers/sim_heart_rate.h"
#include <emscripten.h>
#include <stddef.h>

static void sim_init(void) {
    // No-op for web simulator
}

static uint8_t sim_get_bpm(void) {
    return 70 + (rand() % 20);
}

const heart_rate_driver_t sim_heart_rate_driver = {
    .init = sim_init,
    .get_bpm = sim_get_bpm,
};
