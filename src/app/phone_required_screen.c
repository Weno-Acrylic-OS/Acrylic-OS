#include "app/phone_required_screen.h"
#include "app/navigation_service.h"

static void cancel_event_handler(lv_event_t * e) {
    (void)e; // Unused parameter
    navigation_service_go_back();
}

void create_phone_required_screen(lv_obj_t* parent) {
    lv_obj_clean(parent);

    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cont, 20, 0);

    // --- Bluetooth Icon ---
    lv_obj_t* icon = lv_label_create(cont);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_48, 0);
    lv_label_set_text(icon, LV_SYMBOL_BLUETOOTH);

    // --- Title ---
    lv_obj_t* title = lv_label_create(cont);
    lv_label_set_text(title, "Phone Required");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);

    // --- Message ---
    lv_obj_t* message = lv_label_create(cont);
    lv_label_set_text(message, "Please connect your phone to use this feature.");
    lv_obj_set_style_text_align(message, LV_TEXT_ALIGN_CENTER, 0);

    // --- Cancel Button ---
    lv_obj_t* cancel_btn = lv_btn_create(cont);
    lv_obj_add_event_cb(cancel_btn, cancel_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t* cancel_label = lv_label_create(cancel_btn);
    lv_label_set_text(cancel_label, "Cancel");
    lv_obj_center(cancel_label);
}
