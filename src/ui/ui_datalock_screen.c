#include "ui/ui_datalock_screen.h"
#include "app/datalock.h"
#include <string.h>

static lv_obj_t * datalock_screen;
static lv_obj_t * datalock_pin_label;
static void (*unlock_callback)(void);

static void datalock_keypad_event_handler(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    const char * txt = lv_btnmatrix_get_btn_text(obj, id);

    static char entered_pin[DATALOCK_PIN_LENGTH + 1] = {0};
    static int pin_len = 0;

    if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
        if (pin_len > 0) {
            pin_len--;
            entered_pin[pin_len] = '\0';
        }
    } else if (strcmp(txt, LV_SYMBOL_OK) == 0) {
        if (pin_len == DATALOCK_PIN_LENGTH) {
            if (datalock_check_pin(entered_pin)) {
                lv_obj_del(datalock_screen);
                datalock_screen = NULL;
                if (unlock_callback) {
                    unlock_callback();
                }
            } else {
                lv_label_set_text(datalock_pin_label, "Wrong PIN");
                pin_len = 0;
                memset(entered_pin, 0, sizeof(entered_pin));
            }
        }
    } else {
        if (pin_len < DATALOCK_PIN_LENGTH) {
            strcat(entered_pin, txt);
            pin_len++;
        }
    }

    if (strcmp(txt, LV_SYMBOL_OK) != 0) {
        char pin_display[20] = {0};
        for (int i = 0; i < DATALOCK_PIN_LENGTH; i++) {
            if (i < pin_len) strcat(pin_display, "* ");
            else strcat(pin_display, "_ ");
        }
        if(strlen(pin_display) > 0) pin_display[strlen(pin_display)-1] = 0;
        lv_label_set_text(datalock_pin_label, pin_display);
    }
}

void create_ui_datalock_screen(lv_obj_t * parent, void (*unlock_cb)(void)) {
    unlock_callback = unlock_cb;

    datalock_screen = lv_obj_create(parent);
    lv_obj_set_size(datalock_screen, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(datalock_screen, lv_color_black(), 0);
    lv_obj_set_style_border_width(datalock_screen, 0, 0);
    lv_obj_set_flex_flow(datalock_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(datalock_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * title = lv_label_create(datalock_screen);
    lv_label_set_text(title, "Enter PIN");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);

    datalock_pin_label = lv_label_create(datalock_screen);
    lv_label_set_text(datalock_pin_label, "_ _ _ _");
    lv_obj_set_style_text_font(datalock_pin_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(datalock_pin_label, lv_color_white(), 0);
    lv_obj_set_style_pad_top(datalock_pin_label, 10, 0);
    lv_obj_set_style_pad_bottom(datalock_pin_label, 10, 0);

    static const char * map[] = {"1", "2", "3", "\n",
                                "4", "5", "6", "\n",
                                "7", "8", "9", "\n",
                                LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK, ""};

    lv_obj_t * keypad = lv_btnmatrix_create(datalock_screen);
    lv_btnmatrix_set_map(keypad, map);
    lv_obj_set_size(keypad, lv_pct(80), lv_pct(50));
    lv_obj_align(keypad, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(keypad, datalock_keypad_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}
