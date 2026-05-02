// src/drivers/display.c
#include <display.h>
#include <emscripten.h>

EM_JS(void, display_init, (), {
  console.log("display_init called");
});

EM_JS(void, display_clear, (uint16_t color), {
  var ctx = Module.canvas.getContext('2d');
  var css_color = "#" + ("000000" + color.toString(16)).slice(-6);
  ctx.fillStyle = css_color;
  ctx.fillRect(0, 0, Module.canvas.width, Module.canvas.height);
});

EM_JS(void, display_draw_pixel, (int x, int y, uint16_t color), {
  var ctx = Module.canvas.getContext('2d');
  var css_color = "#" + ("000000" + color.toString(16)).slice(-6);
  ctx.fillStyle = css_color;
  ctx.fillRect(x, y, 1, 1);
});

void display_update() {
    // No-op
}
