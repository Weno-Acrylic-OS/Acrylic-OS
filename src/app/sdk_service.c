#include "app/sdk_service.h"
#include "app/app_list.h"
#include "js_engine.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THIRD_PARTY_APPS 10

static app_descriptor_t third_party_apps[MAX_THIRD_PARTY_APPS];
static int num_third_party_apps = 0;

void sdk_service_init() {
    // Nothing to do here for now
}

void register_app(const app_descriptor_t * descriptor) {
    if (num_third_party_apps < MAX_THIRD_PARTY_APPS) {
        third_party_apps[num_third_party_apps] = *descriptor;
        // Pass a default icon (e.g., LV_SYMBOL_DOWNLOAD) as the second argument
        add_to_app_list(descriptor->name, LV_SYMBOL_DOWNLOAD); 
        num_third_party_apps++;
    }
}

app_descriptor_t * get_third_party_app(const char * name) {
    for (int i = 0; i < num_third_party_apps; i++) {
        if (strcmp(third_party_apps[i].name, name) == 0) {
            return &third_party_apps[i];
        }
    }
    return NULL;
}

void sdk_service_run_app(const char * name) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "third_party_apps/%s/index.js", name);

    FILE * file = fopen(file_path, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        char * buffer = (char *)malloc(length + 1);
        if (buffer) {
            fread(buffer, 1, length, file);
            buffer[length] = '\0';
            js_engine_run(buffer);
            free(buffer);
        }
        fclose(file);
    }
}
