#include "lvgl.h"
#include "app/settings_quick_settings.h"
#include "app/quick_settings.h"
#include "app/persistence_service.h"
#include "app/navigation_service.h" // For back navigation

static lv_obj_t** setting_checkboxes;
static int num_available_settings;

static void save_event_handler(lv_event_t * e) {
    (void)e; // Unused parameter
    quick_setting_id_t new_settings[MAX_QUICK_SETTINGS];
    int new_count = 0;

    for (int i = 0; i < num_available_settings; i++) {
        if (lv_obj_get_state(setting_checkboxes[i]) & LV_STATE_CHECKED) {
            if (new_count < MAX_QUICK_SETTINGS) {
                quick_setting_id_t id = (quick_setting_id_t)(intptr_t)lv_obj_get_user_data(setting_checkboxes[i]);
                new_settings[new_count++] = id;
            }
        }
    }

    set_current_quick_settings(new_settings, new_count);
    navigation_service_go_back();
}

lv_obj_t* create_quick_settings_settings_screen(lv_obj_t* parent) {
    const quick_setting_item_t* available_settings = get_available_quick_settings(&num_available_settings);
    quick_setting_id_t current_settings[MAX_QUICK_SETTINGS];
    int num_current_settings;
    persistence_service_get_quick_settings(current_settings, &num_current_settings);

    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_t* title = lv_label_create(cont);
    lv_label_set_text(title, "Customize Quick Settings");

    lv_obj_t* list = lv_obj_create(cont);
    lv_obj_set_width(list, LV_PCT(100));
    lv_obj_set_flex_grow(list, 1);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);

    setting_checkboxes = lv_mem_alloc(sizeof(lv_obj_t*) * num_available_settings);

    for (int i = 0; i < num_available_settings; i++) {
        const quick_setting_item_t* item_def = &available_settings[i];
        lv_obj_t* cb = lv_checkbox_create(list);
        lv_checkbox_set_text(cb, item_def->text);
        lv_obj_set_user_data(cb, (void*)(intptr_t)item_def->id);

        for (int j = 0; j < num_current_settings; j++) {
            if (current_settings[j] == item_def->id) {
                lv_obj_add_state(cb, LV_STATE_CHECKED);
                break;
            }
        }
        setting_checkboxes[i] = cb;
    }

    lv_obj_t* save_btn = lv_btn_create(cont);
    lv_obj_set_width(save_btn, LV_PCT(80));
    lv_obj_align(save_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(save_btn, save_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t* save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save");
    lv_obj_center(save_label);

    return cont;
}

