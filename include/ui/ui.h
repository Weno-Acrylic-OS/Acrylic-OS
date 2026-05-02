// include/ui/ui.h
#ifndef UI_H
#define UI_H

#include <stdint.h>

typedef enum {
    UI_STATE_CLOCK,
    UI_STATE_HEART_RATE,
    UI_STATE_DATALOCK
} UIState;

void ui_init();
void ui_set_state(UIState state);
UIState ui_get_state();
void ui_draw_screen(const char* bpm_str);
void ui_draw_string(int x, int y, const char* str, uint16_t color);


#endif // UI_H
