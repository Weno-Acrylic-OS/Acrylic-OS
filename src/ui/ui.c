// src/ui/ui.c
#include <ui.h>
#include <display.h> // Include display driver to draw
#include <font.h>    // Include font data
#include <stddef.h>  // For NULL

#define BLACK 0x0000
#define WHITE 0xFFFF

static UIState current_state = UI_STATE_CLOCK;

void ui_init() {
    display_init();
}

void ui_set_state(UIState state) {
    current_state = state;
}

UIState ui_get_state() {
    return current_state;
}

void ui_draw_char(int x, int y, char c, uint16_t color) {
    if (c < ' ' || c > 'W') { // Simple check for available characters
        c = '?'; // Default to '?' if character not in font data
    }

    const uint8_t* char_data = font5x7[c - ' '];

    for (int col = 0; col < 5; ++col) {
        for (int row = 0; row < 7; ++row) {
            if ((char_data[col] >> row) & 1) {
                display_draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void ui_draw_string(int x, int y, const char* str, uint16_t color) {
    if (str == NULL) {
        return;
    }

    int current_x = x;
    while (*str) {
        ui_draw_char(current_x, y, *str, color);
        current_x += 6; // 5 pixels for char + 1 for spacing
        str++;
    }
}

void ui_draw_clock_screen() {
    display_clear(BLACK);
    ui_draw_string(90, 150, "12:34", WHITE);
    display_update();
}

void ui_draw_datalock_screen(const char* pin_display) {
    display_clear(BLACK);
    ui_draw_string(80, 120, "ENTER PIN", WHITE);
    if (pin_display != NULL) {
        ui_draw_string(96, 150, pin_display, WHITE);
    }
    display_update();
}

void ui_draw_heart_rate_screen(const char* bpm_str) {
    display_clear(BLACK);
    ui_draw_string(70, 140, "HEART RATE:", WHITE);
    ui_draw_string(90, 160, bpm_str, WHITE);
    display_update();
}

void ui_draw_screen(const char* draw_param) {
    switch (current_state) {
        case UI_STATE_CLOCK:
            ui_draw_clock_screen();
            break;
        case UI_STATE_HEART_RATE:
            ui_draw_heart_rate_screen(draw_param);
            break;
        case UI_STATE_DATALOCK:
            ui_draw_datalock_screen(draw_param);
            break;
    }
}
