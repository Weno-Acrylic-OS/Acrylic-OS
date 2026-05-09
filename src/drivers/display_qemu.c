#include <display.h>

// This is a simple framebuffer driver for a common QEMU graphical device.
// The vexpress-a9 machine provides this framebuffer at this memory address.
#define FRAMEBUFFER_ADDR 0x10020000
#define WIDTH 240
#define HEIGHT 320

static volatile uint16_t *framebuffer = (uint16_t *)FRAMEBUFFER_ADDR;

void display_init() {
    // No-op
}

void display_clear(uint16_t color) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        framebuffer[i] = color;
    }
}

void display_draw_pixel(int x, int y, uint16_t color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        framebuffer[y * WIDTH + x] = color;
    }
}

void display_update() {
    // No-op for a direct framebuffer
}
