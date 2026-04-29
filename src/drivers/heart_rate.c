// src/drivers/heart_rate.c
#include <heart_rate.h>
#include <emscripten.h>

void heart_rate_init() {
    // No-op for web
}

EM_JS(uint8_t, heart_rate_get_bpm, (), {
  return 70 + (Math.random() * 20);
});
