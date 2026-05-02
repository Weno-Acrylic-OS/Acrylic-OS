// src/drivers/touch.c
#include <touch.h>
#include <emscripten.h>

void touch_init() {
    // No-op for web
}

EM_JS(TouchEvent, touch_get_event, (), {
  // 0 = NONE, 1 = SWIPE_LEFT, 2 = SWIPE_RIGHT
  var random_event = Math.random() * 100;
  if (random_event < 5) {
      return 1;
  } else if (random_event < 10) {
      return 2;
  }
  return 0;
});
