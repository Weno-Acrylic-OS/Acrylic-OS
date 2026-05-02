#include "app/weno_store.h"
#include "js_engine.h" // To run the JS code
#include "lvgl.h"
#include <emscripten.h>
#include <stdio.h> // Keep for fprintf if needed, though removed for now
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

// --- UI Objects ---
static lv_obj_t * container;

// --- Forward Declarations ---
void weno_store_fetch_app_source(const char *app_name);
static void install_button_event_handler(lv_event_t * e);

// --- JS->C Callbacks ---

EMSCRIPTEN_KEEPALIVE
void weno_store_receive_app_source(char *js_code) {
    js_engine_run(js_code);
    free(js_code);

    // Show a confirmation toast
    lv_obj_t * toast = lv_label_create(lv_scr_act());
    lv_label_set_text(toast, "App Installed!");
    lv_obj_set_style_bg_color(toast, lv_color_hex(0x28a745), LV_PART_MAIN);
    lv_obj_set_style_text_color(toast, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_pad_all(toast, 10, LV_PART_MAIN);
    lv_obj_align(toast, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_fade_out(toast, 2000, 1000); // Corrected function name
}


EMSCRIPTEN_KEEPALIVE
void weno_store_receive_apps_json(char *json_string) {
    lv_obj_clean(container);
    lv_obj_t * list = lv_list_create(container);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    lv_list_add_text(list, "Available Apps");

    cJSON *apps_json = cJSON_Parse(json_string);
    if (apps_json == NULL) {
        cJSON_Delete(apps_json);
        free(json_string);
        return;
    }

    cJSON *app_json = NULL;
    cJSON_ArrayForEach(app_json, apps_json) {
        cJSON *name_json = cJSON_GetObjectItemCaseSensitive(app_json, "name");
        
        if (cJSON_IsString(name_json) && (name_json->valuestring != NULL)) {
            // Memory for user_data needs to persist until the event is handled
            char *app_name_copy = strdup(name_json->valuestring);
            
            lv_obj_t * btn = lv_list_add_btn(list, LV_SYMBOL_DOWNLOAD, app_name_copy);
            lv_obj_add_event_cb(btn, install_button_event_handler, LV_EVENT_CLICKED, app_name_copy);
        }
    }

    cJSON_Delete(apps_json);
    free(json_string);
}

// --- C->JS Triggers ---

void weno_store_fetch_app_source(const char *app_name) {
    char script[512];
    // Normalize app name for the URL
    char url_app_name[100];
    strncpy(url_app_name, app_name, sizeof(url_app_name) - 1);
    url_app_name[sizeof(url_app_name) - 1] = '\0';
    for (char *p = url_app_name; *p; ++p) {
        if (*p == ' ') *p = '-';
    }
    
    snprintf(script, sizeof(script),
        "fetch('http://localhost:3000/api/apps/source/%s')"
        "  .then(response => response.text())"
        "  .then(text => {"
        "    const len = lengthBytesUTF8(text) + 1;"
        "    const str = _malloc(len);"
        "    stringToUTF8(text, str, len);"
        "    ccall('weno_store_receive_app_source', 'void', ['number'], [str]);"
        "  })"
        "  .catch(err => console.error('Failed to fetch app source:', err));",
        url_app_name
    );
    emscripten_run_script(script);
}


void weno_store_fetch_apps() {
    emscripten_run_script(
        "fetch('http://localhost:3000/api/apps')"
        "  .then(response => response.text())"
        "  .then(text => {"
        "    const len = lengthBytesUTF8(text) + 1;"
        "    const str = _malloc(len);"
        "    stringToUTF8(text, str, len);"
        "    ccall('weno_store_receive_apps_json', 'void', ['number'], [str]);"
        "  })"
        "  .catch(err => console.error('Failed to fetch apps:', err));"
    );
}

// --- Event Handlers & UI Creation ---

static void install_button_event_handler(lv_event_t * e) {
    const char * app_name = (const char *)lv_event_get_user_data(e);
    weno_store_fetch_app_source(app_name);
    // Note: In a real app, you might want to free the app_name_copy memory,
    // but since it's a simple demo, we'll leak it.
}


void create_weno_store_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    container = parent;

    lv_obj_t * label = lv_label_create(container);
    lv_label_set_text(label, "Weno Store - Loading...");
    lv_obj_center(label);
    
    weno_store_fetch_apps();
}
