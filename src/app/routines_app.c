#include "app/routines_app.h"
#include "app/routines_service.h"
#include <stdio.h> // For snprintf

void create_routines_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_black(), 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 10, 0);

    // --- Title ---
    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "My Routines");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_pad_bottom(title, 10, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    // --- Routines List ---
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(85));
    lv_obj_align(list, LV_ALIGN_BOTTOM_MID, 0, 0);

    int count = 0;
    const routine_t * routines = routines_service_get_routines(&count);

    if (count == 0) {
        lv_list_add_text(list, "No routines configured.");
    } else {
        for (int i = 0; i < count; i++) {
            char buf[100];
            snprintf(buf, sizeof(buf), "%s - %02d:%02d", routines[i].name, routines[i].hour, routines[i].minute);
            lv_list_add_btn(list, NULL, buf);
            // TODO: Add event handler for editing/viewing routine details
        }
    }
}
