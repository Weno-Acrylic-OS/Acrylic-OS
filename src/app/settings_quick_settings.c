#include "lvgl.h"
#include "app/settings_quick_settings.h"
#include "app/quick_settings.h"
#include "app/persistence_service.h"
#include "app/navigation_service.h" // For back navigation

#ifdef SIMULATOR_BUILD

// --- Checkbox-based implementation for Web/Simulator ---

static lv_obj_t** setting_checkboxes;
static int num_available_settings;

static void save_event_handler(lv_event_t * e) {
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

#else

// --- Drag-and-drop implementation for ARM ---

static lv_obj_t* enabled_list;
static lv_obj_t* disabled_list;
static lv_obj_t* dragged_item = NULL;
static lv_obj_t* placeholder = NULL;

static void save_event_handler(lv_event_t* e) {
    (void)e;
    quick_setting_id_t new_settings[MAX_QUICK_SETTINGS];
    int new_count = 0;
    for (uint32_t i = 0; i < lv_obj_get_child_cnt(enabled_list); i++) {
        if (new_count < MAX_QUICK_SETTINGS) {
            lv_obj_t* item = lv_obj_get_child(enabled_list, i);
            quick_setting_id_t id = (quick_setting_id_t)(intptr_t)lv_obj_get_user_data(item);
            new_settings[new_count++] = id;
        }
    }
    set_current_quick_settings(new_settings, new_count);
    navigation_service_go_back();
}

static void drag_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_DRAG_START) {
        placeholder = lv_obj_create(lv_obj_get_parent(obj));
        lv_obj_set_size(placeholder, lv_obj_get_width(obj), lv_obj_get_height(obj));
        lv_obj_set_style_bg_color(placeholder, lv_color_hex(0xcccccc), 0);
        lv_obj_move_to_index(placeholder, lv_obj_get_index(obj));
        dragged_item = obj;
        lv_obj_add_flag(dragged_item, LV_OBJ_FLAG_HIDDEN);
    }
    else if(code == LV_EVENT_DRAGGING) {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL) return;
        lv_point_t vect;
        lv_indev_get_vect(indev, &vect);
        lv_coord_t y = lv_obj_get_y(placeholder) + vect.y;
        lv_obj_set_y(placeholder, y);

        uint32_t i;
        for(i = 0; i < lv_obj_get_child_cnt(lv_obj_get_parent(placeholder)); i++) {
            lv_obj_t* child = lv_obj_get_child(lv_obj_get_parent(placeholder), i);
            if(child == placeholder || child == dragged_item) continue;
            if(lv_obj_get_y(placeholder) < lv_obj_get_y(child) && lv_obj_get_index(placeholder) > lv_obj_get_index(child)) {
                lv_obj_move_to_index(placeholder, i);
                break;
            }
            if(lv_obj_get_y(placeholder) > lv_obj_get_y(child) && lv_obj_get_index(placeholder) < lv_obj_get_index(child)) {
                lv_obj_move_to_index(placeholder, i);
                break;
            }
        }
    }
    else if(code == LV_EVENT_DRAG_END) {
        lv_obj_t * target_list = NULL;
        lv_point_t drop_point;
        lv_indev_get_point(lv_indev_get_act(), &drop_point);
        
        if (lv_obj_hit_test(enabled_list, &drop_point)) {
            target_list = enabled_list;
        } else if (lv_obj_hit_test(disabled_list, &drop_point)) {
            target_list = disabled_list;
        }

        if (target_list) {
            lv_obj_set_parent(dragged_item, target_list);
            lv_obj_move_to_index(dragged_item, lv_obj_get_index(placeholder));
        }

        lv_obj_clear_flag(dragged_item, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del(placeholder);
        placeholder = NULL;
        dragged_item = NULL;
    }
}


lv_obj_t* create_quick_settings_settings_screen(lv_obj_t* parent) {
    const quick_setting_item_t* available_settings;
    int num_available_settings;
    available_settings = get_available_quick_settings(&num_available_settings);

    quick_setting_id_t current_settings[MAX_QUICK_SETTINGS];
    int num_current_settings;
    persistence_service_get_quick_settings(current_settings, &num_current_settings);

    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_t* title = lv_label_create(cont);
    lv_label_set_text(title, "Customize Quick Settings");

    lv_obj_t* enabled_label = lv_label_create(cont);
    lv_label_set_text(enabled_label, "Enabled");
    enabled_list = lv_list_create(cont);
    lv_obj_set_size(enabled_list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(enabled_list, LV_FLEX_FLOW_COLUMN);

    lv_obj_t* disabled_label = lv_label_create(cont);
    lv_label_set_text(disabled_label, "Disabled");
    disabled_list = lv_list_create(cont);
    lv_obj_set_size(disabled_list, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(disabled_list, LV_FLEX_FLOW_COLUMN);

    for (int i = 0; i < num_available_settings; i++) {
        const quick_setting_item_t* item_def = &available_settings[i];
        bool is_enabled = false;
        for (int j = 0; j < num_current_settings; j++) {
            if (current_settings[j] == item_def->id) {
                is_enabled = true;
                break;
            }
        }

        lv_obj_t* list_item = lv_btn_create(is_enabled ? enabled_list : disabled_list);
        lv_obj_set_user_data(list_item, (void*)(intptr_t)item_def->id);
        lv_obj_set_width(list_item, LV_PCT(100));
        
        lv_obj_t* label = lv_label_create(list_item);
        lv_label_set_text(label, item_def->text);
        lv_obj_center(label);

        lv_obj_add_event_cb(list_item, drag_event_handler, LV_EVENT_DRAG_START, NULL);
        lv_obj_add_event_cb(list_item, drag_event_handler, LV_EVENT_DRAGGING, NULL);
        lv_obj_add_event_cb(list_item, drag_event_handler, LV_EVENT_DRAG_END, NULL);
    }
    
    for(int i = 0; i < num_current_settings; i++) {
         for (uint32_t j = 0; j < lv_obj_get_child_cnt(enabled_list); j++) {
            lv_obj_t* item = lv_obj_get_child(enabled_list, j);
            quick_setting_id_t id = (quick_setting_id_t)(intptr_t)lv_obj_get_user_data(item);
            if(id == current_settings[i]) {
                lv_obj_move_to_index(item, i);
                break;
            }
         }
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

#endif // SIMULATOR_BUILD

