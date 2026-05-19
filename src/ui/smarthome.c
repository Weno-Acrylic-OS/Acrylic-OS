#include "ui/smarthome.h"
#include "ui/ui_hal.h"

// A simple smart home UI for demonstration
static void create_smarthome_ui(lv_obj_t* screen) {
    lv_obj_t* label = lv_label_create(screen);
    lv_label_set_text(label, "Acrylic OS - Smart Home Mode");
    lv_obj_center(label);
}

static ui_personality_t smarthome_personality = {
    .init_ui = create_smarthome_ui
};

ui_personality_t* get_smarthome_personality() {
    return &smarthome_personality;
}
