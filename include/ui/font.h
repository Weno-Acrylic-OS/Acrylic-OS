// include/ui/font.h
#ifndef FONT_H
#define FONT_H

#include <stdint.h>

// A very simple 5x7 pixel font
// Each character is represented by 5 bytes, where each byte is a column of pixels.
// The 7 bits of each byte correspond to the 7 rows of pixels.
extern const uint8_t font5x7[][5];

#endif // FONT_H
