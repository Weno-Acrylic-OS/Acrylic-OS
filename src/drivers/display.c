// src/drivers/display.c
#include <display.h>
#include <emscripten.h>

EM_JS(void, display_init, (), {
  console.log("display_init called");
});

EM_JS(void, display_clear, (uint16_t color), {
  // Decode RGB565 color to RGB888
  var r = (color >> 11) & 0x1F;
  var g = (color >> 5) & 0x3F;
  var b = color & 0x1F;
  r = Math.round((r * 255) / 31);
  g = Math.round((g * 255) / 63);
  b = Math.round((b * 255) / 31);
  var css_color = "rgb(" + r + "," + g + "," + b + ")";

  var ctx = Module.canvas.getContext('2d');
  ctx.fillStyle = css_color;
  ctx.fillRect(0, 0, Module.canvas.width, Module.canvas.height);
});

EM_JS(void, display_draw_pixel, (int x, int y, uint16_t color), {
  // Decode RGB565 color to RGB888
  var r = (color >> 11) & 0x1F;
  var g = (color >> 5) & 0x3F;
  var b = color & 0x1F;
  r = Math.round((r * 255) / 31);
  g = Math.round((g * 255) / 63);
  b = Math.round((b * 255) / 31);
  var css_color = "rgb(" + r + "," + g + "," + b + ")";

  var ctx = Module.canvas.getContext('2d');
  ctx.fillStyle = css_color;
  ctx.fillRect(x, y, 1, 1);
});

void display_update() {
    // No-op
}
