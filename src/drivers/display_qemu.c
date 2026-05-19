#include <stdint.h>             /* MUST BE AT THE TOP */
#include "drivers/display.h"

#define WIDTH 640
#define HEIGHT 480

#define CLCD_BASE         0x4001F000
#define CLCD_UPBASE       (*(volatile uint32_t *)(CLCD_BASE + 0x10))
#define CLCD_CONTROL      (*(volatile uint32_t *)(CLCD_BASE + 0x18))
#define CLCD_TIM0         (*(volatile uint32_t *)(CLCD_BASE + 0x00))
#define CLCD_TIM1         (*(volatile uint32_t *)(CLCD_BASE + 0x04))
#define CLCD_TIM2         (*(volatile uint32_t *)(CLCD_BASE + 0x08))

#define FRAMEBUFFER_ADDR  0x41000000 

static volatile uint16_t *framebuffer = (uint16_t *)FRAMEBUFFER_ADDR;

void display_init() {
    CLCD_UPBASE = FRAMEBUFFER_ADDR;
    CLCD_TIM0 = 0x3F1F3F9C; 
    CLCD_TIM1 = 0x090B61DF;
    CLCD_TIM2 = 0x067F0500;
    CLCD_CONTROL = 0x00000821; 
}

void display_draw_pixel(int x, int y, uint16_t color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        framebuffer[y * WIDTH + x] = color;
    }
}

void display_clear(uint16_t color) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        framebuffer[i] = color;
    }
}