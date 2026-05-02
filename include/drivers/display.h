// include/drivers/display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_init();
void display_clear(uint16_t color);
void display_draw_pixel(int x, int y, uint16_t color);
void display_update(); // To refresh the display if it's buffered

#endif // DISPLAY_H
