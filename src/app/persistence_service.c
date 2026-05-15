#include "app/persistence_service.h"
#include "vendor/cjson/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SETTINGS_FILE "settings.json"

static cJSON* settings_json = NULL;

void persistence_service_init() {
    persistence_service_load();
}

void persistence_service_load() {
    FILE* f = fopen(SETTINGS_FILE, "rb");
    if (f == NULL) {
        // File doesn't exist, create a new default structure
        settings_json = cJSON_CreateObject();
        cJSON_AddItemToObject(settings_json, "quick_settings", cJSON_CreateArray());
        return;
    }

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* data = (char*)malloc(length + 1);
    fread(data, 1, length, f);
    fclose(f);

    data[length] = '\0';
    settings_json = cJSON_Parse(data);
    free(data);

    if (settings_json == NULL) {
        // Error parsing, create a default structure
        settings_json = cJSON_CreateObject();
        cJSON_AddItemToObject(settings_json, "quick_settings", cJSON_CreateArray());
    }
}

void persistence_service_save() {
    if (settings_json == NULL) return;

    char* json_string = cJSON_Print(settings_json);
    FILE* f = fopen(SETTINGS_FILE, "wb");
    if (f == NULL) {
        // Cannot open file for writing.
        // In a real scenario, we should handle this error.
        free(json_string);
        return;
    }

    fprintf(f, "%s", json_string);
    fclose(f);
    free(json_string);
}

void persistence_service_get_quick_settings(quick_setting_id_t* settings, int* count) {
    if (settings_json == NULL) {
        *count = 0;
        return;
    }

    cJSON* qs_array = cJSON_GetObjectItem(settings_json, "quick_settings");
    if (!cJSON_IsArray(qs_array)) {
        *count = 0;
        return;
    }

    int i = 0;
    cJSON* element;
    cJSON_ArrayForEach(element, qs_array) {
        if (cJSON_IsNumber(element)) {
            settings[i++] = (quick_setting_id_t)element->valueint;
        }
    }
    *count = i;
}

void persistence_service_set_quick_settings(const quick_setting_id_t* settings, int count) {
    if (settings_json == NULL) {
        // This should not happen if init is called properly
        settings_json = cJSON_CreateObject();
    }

    cJSON* qs_array = cJSON_CreateArray();
    for (int i = 0; i < count; i++) {
        cJSON_AddItemToArray(qs_array, cJSON_CreateNumber(settings[i]));
    }

    if (cJSON_HasObjectItem(settings_json, "quick_settings")) {
        cJSON_ReplaceItemInObject(settings_json, "quick_settings", qs_array);
    } else {
        cJSON_AddItemToObject(settings_json, "quick_settings", qs_array);
    }

    persistence_service_save();
}
