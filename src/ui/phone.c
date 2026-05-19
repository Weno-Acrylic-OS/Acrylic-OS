#include "ui/phone.h"
#include "ui/ui_hal.h"

// A simple phone UI for demonstration
static void create_phone_ui(lv_obj_t* screen) {
    lv_obj_t* label = lv_label_create(screen);
    lv_label_set_text(label, "Acrylic OS - Phone Mode");
    lv_obj_center(label);
}

static ui_personality_t phone_personality = {
    .init_ui = create_phone_ui
};

ui_personality_t* get_phone_personality() {
    return &phone_personality;
}
