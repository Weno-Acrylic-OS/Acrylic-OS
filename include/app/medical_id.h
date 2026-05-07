#ifndef MEDICAL_ID_H
#define MEDICAL_ID_H

#include "lvgl.h"

#define MAX_NAME_LEN 32
#define MAX_PHONE_LEN 20
#define MAX_BLOOD_TYPE_LEN 4
#define MAX_ALLERGY_LEN 32
#define MAX_CONDITION_LEN 32

#define MAX_EMERGENCY_CONTACTS 3
#define MAX_ALLERGIES 5
#define MAX_CONDITIONS 5

typedef struct {
    char name[MAX_NAME_LEN];
    char phone_number[MAX_PHONE_LEN];
} emergency_contact_t;

typedef struct {
    char blood_type[MAX_BLOOD_TYPE_LEN];
    char allergies[MAX_ALLERGIES][MAX_ALLERGY_LEN];
    char conditions[MAX_CONDITIONS][MAX_CONDITION_LEN];
    emergency_contact_t emergency_contacts[MAX_EMERGENCY_CONTACTS];
} medical_id_t;

void create_medical_id_app(lv_obj_t * parent);


#endif // MEDICAL_ID_H
