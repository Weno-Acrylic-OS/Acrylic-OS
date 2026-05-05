#include "app/calculator.h"
#include "app_list.h"
#include <string.h>
#include <stdio.h>

static lv_obj_t * display;
static char display_buffer[256];

static void back_to_app_list_event_handler(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_obj_t * parent = lv_obj_get_parent(obj);
    create_app_list(parent);
}

static void btn_event_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    const char * txt = lv_label_get_text(lv_obj_get_child(btn, 0));

    if (strcmp(txt, "C") == 0) {
        display_buffer[0] = '\0';
    } else if (strcmp(txt, "=") == 0) {
        // TODO: evaluation logic
    } else {
        strcat(display_buffer, txt);
    }

    lv_label_set_text(display, display_buffer);
}

void create_calculator_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    display_buffer[0] = '\0';

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, back_to_app_list_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");

    display = lv_label_create(parent);
    lv_obj_align(display, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_label_set_text(display, "");

    lv_obj_t * btn_grid = lv_obj_create(parent);
    lv_obj_set_size(btn_grid, lv_pct(100), lv_pct(80));
    lv_obj_align(btn_grid, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_layout(btn_grid, LV_LAYOUT_GRID);

    static const char * map[] = {
        "7", "8", "9", "/", "\n",
        "4", "5", "6", "*", "\n",
        "1", "2", "3", "-", "\n",
        "C", "0", "=", "+", ""
    };

    lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_grid_dsc_array(btn_grid, col_dsc, row_dsc);

    for (int i = 0; map[i][0] != '\0'; i++) {
        if (strcmp(map[i], "\n") == 0) continue;

        uint32_t row = i / 5;
        uint32_t col = i % 5;

        lv_obj_t * btn = lv_btn_create(btn_grid);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, map[i]);
        lv_obj_center(label);
        lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, NULL);
    }
}
