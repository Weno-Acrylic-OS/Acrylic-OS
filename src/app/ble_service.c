#include "app/ble_service.h"
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

/**
 * @file ble_service.c
 * @brief Skeleton implementation of the BLE service for Weno Fit OS.
 *
 * See ble_service.h for detailed notes on the simulator limitations.
 */

// --- UUIDs for Custom Weno Fit OS Service ---
// Weno Fit OS Service UUID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
//
// Characteristics:
// Steps Count:      6E400002-B5A3-F393-E0A9-E50E24DCCA9E (Read, Notify)
// Sleep Data:       6E400003-B5A3-F393-E0A9-E50E24DCCA9E (Read)
// Medical ID:       6E400004-B5A3-F393-E0A9-E50E24DCCA9E (Read, Write)
// Device Control:   6E400005-B5A3-F393-E0A9-E50E24DCCA9E (Write)
// Notification:     6E400006-B5A3-F393-E0A9-E50E24DCCA9E (Write)
// Goals:            6E400007-B5A3-F393-E0A9-E50E24DCCA9E (Read, Notify)

// --- Medical ID Data Structure ---
typedef struct {
    char name[50];
    char dob[20];
    char conditions[100];
    char allergies[100];
    char emergencyContact[50];
} medical_id_t;

static medical_id_t g_medical_id = {
    .name = "John Doe",
    .dob = "1990-01-01",
    .conditions = "None",
    .allergies = "Peanuts",
    .emergencyContact = "Jane Doe (555-1234)"
};

// --- BLE Event Handlers (Stubs) ---

/**
 * @brief Called when the companion app writes to the Medical ID characteristic.
 *
 * On a real device, this would be a callback registered with the BLE stack.
 * We simulate this by having the UI simulator call this directly.
 */
void ble_service_on_medical_id_write(const char* json_string) {
    printf("BLE Service: Received write for Medical ID\\n");
    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        printf("BLE Service: Error parsing Medical ID JSON\\n");
        return;
    }

    cJSON *name = cJSON_GetObjectItem(root, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        strncpy(g_medical_id.name, name->valuestring, sizeof(g_medical_id.name) - 1);
    }

    cJSON *dob = cJSON_GetObjectItem(root, "dob");
    if (cJSON_IsString(dob) && (dob->valuestring != NULL)) {
        strncpy(g_medical_id.dob, dob->valuestring, sizeof(g_medical_id.dob) - 1);
    }

    cJSON *conditions = cJSON_GetObjectItem(root, "conditions");
    if (cJSON_IsString(conditions) && (conditions->valuestring != NULL)) {
        strncpy(g_medical_id.conditions, conditions->valuestring, sizeof(g_medical_id.conditions) - 1);
    }

    cJSON *allergies = cJSON_GetObjectItem(root, "allergies");
    if (cJSON_IsString(allergies) && (allergies->valuestring != NULL)) {
        strncpy(g_medical_id.allergies, allergies->valuestring, sizeof(g_medical_id.allergies) - 1);
    }

    cJSON *emergencyContact = cJSON_GetObjectItem(root, "emergencyContact");
    if (cJSON_IsString(emergencyContact) && (emergencyContact->valuestring != NULL)) {
        strncpy(g_medical_id.emergencyContact, emergencyContact->valuestring, sizeof(g_medical_id.emergencyContact) - 1);
    }

    cJSON_Delete(root);
    printf("BLE Service: Updated Medical ID\\n");
}


/**
 * @brief Called when the companion app reads the Medical ID characteristic.
 *
 * On a real device, this would be a callback registered with the BLE stack.
 * The stack would provide a buffer to write the characteristic's value into.
 */
int ble_service_on_medical_id_read(char* out_buffer, size_t buffer_size) {
    printf("BLE Service: Received read for Medical ID\\n");
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return 0;
    }

    cJSON_AddStringToObject(root, "name", g_medical_id.name);
    cJSON_AddStringToObject(root, "dob", g_medical_id.dob);
    cJSON_AddStringToObject(root, "conditions", g_medical_id.conditions);
    cJSON_AddStringToObject(root, "allergies", g_medical_id.allergies);
    cJSON_AddStringToObject(root, "emergencyContact", g_medical_id.emergencyContact);

    char *json_string = cJSON_PrintUnformatted(root);
    if (json_string == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    strncpy(out_buffer, json_string, buffer_size - 1);
    out_buffer[buffer_size - 1] = '\\0'; // Ensure null termination

    cJSON_free(json_string);
    cJSON_Delete(root);

    return strlen(out_buffer);
}


// --- Service Initialization and Updates ---

void ble_service_init() {
    printf("BLE Service: Initializing (stub)\\n");
    // On a real device, this function would register the read/write handlers:
    printf("BLE Service: 'Registering' Medical ID read/write handlers\\n");
}

void ble_service_update_heart_rate(uint8_t heart_rate) {
    printf("BLE Service: Updating heart rate to %u (stub)\\n", heart_rate);
}

void ble_service_update_steps(uint32_t steps) {
    printf("BLE Service: Updating steps to %u (stub)\\n", steps);
}

void ble_service_update_goals(const goal_t *goals, int count) {
    printf("BLE Service: Updating %d goals (stub)\\n", count);
    for (int i = 0; i < count; i++) {
        printf("  - Goal: %s, Progress: %d/%d\\n", goals[i].description, goals[i].current_value, goals[i].target_value);
    }
}