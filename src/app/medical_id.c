#include "app/medical_id.h"
#include "app/persistence.h"
#include <stdio.h>
#include <string.h>

static medical_id_t my_medical_id;

typedef struct {
    lv_obj_t * blood_type_ta;
    lv_obj_t * allergies_ta[MAX_ALLERGIES];
    lv_obj_t * conditions_ta[MAX_CONDITIONS];
    lv_obj_t * contact_name_ta[MAX_EMERGENCY_CONTACTS];
    lv_obj_t * contact_phone_ta[MAX_EMERGENCY_CONTACTS];
} medical_id_edit_widgets_t;

static medical_id_edit_widgets_t edit_widgets;

static void create_medical_id_edit_screen(lv_obj_t * parent); // Forward declaration
void create_medical_id_app(lv_obj_t * parent); // Forward declaration for navigation

static void edit_button_event_handler(lv_event_t * e) {
    lv_obj_t * parent = (lv_obj_t *)lv_event_get_user_data(e);
    create_medical_id_edit_screen(parent);
}

void create_medical_id_app(lv_obj_t * parent) {
    lv_obj_clean(parent);

    ssize_t res = persistence_read("medical_id", &my_medical_id, sizeof(medical_id_t));
    if (res <= 0) {
        memset(&my_medical_id, 0, sizeof(medical_id_t));
    }

    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    lv_obj_t * edit_btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Edit");
    lv_obj_add_event_cb(edit_btn, edit_button_event_handler, LV_EVENT_CLICKED, parent);

    char buffer[128];
    lv_list_add_text(list, "Medical Information");
    snprintf(buffer, sizeof(buffer), "Blood Type: %s", my_medical_id.blood_type[0] == '\0' ? "Not Set" : my_medical_id.blood_type);
    lv_list_add_btn(list, LV_SYMBOL_DUMMY, buffer);

    for (int i = 0; i < MAX_ALLERGIES && my_medical_id.allergies[i][0] != '\0'; i++) {
        snprintf(buffer, sizeof(buffer), "Allergy: %s", my_medical_id.allergies[i]);
        lv_list_add_btn(list, LV_SYMBOL_DUMMY, buffer);
    }
    for (int i = 0; i < MAX_CONDITIONS && my_medical_id.conditions[i][0] != '\0'; i++) {
        snprintf(buffer, sizeof(buffer), "Condition: %s", my_medical_id.conditions[i]);
        lv_list_add_btn(list, LV_SYMBOL_DUMMY, buffer);
    }

    lv_list_add_text(list, "Emergency Contacts");
    for (int i = 0; i < MAX_EMERGENCY_CONTACTS && my_medical_id.emergency_contacts[i].name[0] != '\0'; i++) {
        snprintf(buffer, sizeof(buffer), "%s: %s", 
                 my_medical_id.emergency_contacts[i].name,
                 my_medical_id.emergency_contacts[i].phone_number);
        lv_list_add_btn(list, LV_SYMBOL_CALL, buffer);
    }
}

static lv_obj_t * create_form_field(lv_obj_t * parent, const char * label_text) {
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cont, 0, 0);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, label_text);

    lv_obj_t * ta = lv_textarea_create(cont);
    lv_textarea_set_one_line(ta, true);
    lv_obj_set_width(ta, lv_pct(60));

    return ta;
}

static void save_event_handler(lv_event_t * e) {
    lv_obj_t * parent = (lv_obj_t *)lv_event_get_user_data(e);

    strncpy(my_medical_id.blood_type, lv_textarea_get_text(edit_widgets.blood_type_ta), MAX_BLOOD_TYPE_LEN - 1);
    for (int i = 0; i < MAX_ALLERGIES; i++) {
        strncpy(my_medical_id.allergies[i], lv_textarea_get_text(edit_widgets.allergies_ta[i]), MAX_ALLERGY_LEN - 1);
    }
    for (int i = 0; i < MAX_CONDITIONS; i++) {
        strncpy(my_medical_id.conditions[i], lv_textarea_get_text(edit_widgets.conditions_ta[i]), MAX_CONDITION_LEN - 1);
    }
    for (int i = 0; i < MAX_EMERGENCY_CONTACTS; i++) {
        strncpy(my_medical_id.emergency_contacts[i].name, lv_textarea_get_text(edit_widgets.contact_name_ta[i]), MAX_NAME_LEN - 1);
        strncpy(my_medical_id.emergency_contacts[i].phone_number, lv_textarea_get_text(edit_widgets.contact_phone_ta[i]), MAX_PHONE_LEN - 1);
    }

    persistence_write("medical_id", &my_medical_id, sizeof(medical_id_t));
    create_medical_id_app(parent);
}

static void cancel_event_handler(lv_event_t * e) {
    lv_obj_t * parent = (lv_obj_t *)lv_event_get_user_data(e);
    create_medical_id_app(parent);
}

static void create_medical_id_edit_screen(lv_obj_t * parent) {
    lv_obj_clean(parent);
    
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    edit_widgets.blood_type_ta = create_form_field(cont, "Blood Type:");
    lv_textarea_set_text(edit_widgets.blood_type_ta, my_medical_id.blood_type);

    for (int i = 0; i < MAX_ALLERGIES; i++) {
        char label[32];
        snprintf(label, sizeof(label), "Allergy %d:", i + 1);
        edit_widgets.allergies_ta[i] = create_form_field(cont, label);
        lv_textarea_set_text(edit_widgets.allergies_ta[i], my_medical_id.allergies[i]);
    }

    for (int i = 0; i < MAX_CONDITIONS; i++) {
        char label[32];
        snprintf(label, sizeof(label), "Condition %d:", i + 1);
        edit_widgets.conditions_ta[i] = create_form_field(cont, label);
        lv_textarea_set_text(edit_widgets.conditions_ta[i], my_medical_id.conditions[i]);
    }

    for (int i = 0; i < MAX_EMERGENCY_CONTACTS; i++) {
        char label[32];
        snprintf(label, sizeof(label), "Contact %d Name:", i + 1);
        edit_widgets.contact_name_ta[i] = create_form_field(cont, label);
        lv_textarea_set_text(edit_widgets.contact_name_ta[i], my_medical_id.emergency_contacts[i].name);

        snprintf(label, sizeof(label), "Contact %d Phone:", i + 1);
        edit_widgets.contact_phone_ta[i] = create_form_field(cont, label);
        lv_textarea_set_text(edit_widgets.contact_phone_ta[i], my_medical_id.emergency_contacts[i].phone_number);
    }
    
    lv_obj_t * btn_cont = lv_obj_create(cont);
    lv_obj_set_size(btn_cont, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btn_cont, LV_FLEX_FLOW_ROW);

    lv_obj_t * save_btn = lv_btn_create(btn_cont);
    lv_obj_add_event_cb(save_btn, save_event_handler, LV_EVENT_CLICKED, parent);
    lv_obj_t * save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save");

    lv_obj_t * cancel_btn = lv_btn_create(btn_cont);
    lv_obj_add_event_cb(cancel_btn, cancel_event_handler, LV_EVENT_CLICKED, parent);
    lv_obj_t * cancel_label = lv_label_create(cancel_btn);
    lv_label_set_text(cancel_label, "Cancel");
}

