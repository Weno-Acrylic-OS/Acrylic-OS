#include "app/persistence.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#ifdef SIMULATOR_BUILD
#include <emscripten.h>
#endif

static bool oobe_completed = false;

void persistence_init() {
    // For the web/simulator build, we can create a directory in the virtual filesystem.
    mkdir("persistence", 0777);
}

bool persistence_get_oobe_completed() {
    return oobe_completed;
}

void persistence_set_oobe_completed(bool completed) {
    oobe_completed = completed;
}

bool persistence_write(const char * key, const void * data, size_t size) {
    char path[256];
    snprintf(path, sizeof(path), "persistence/%s", key);
    FILE * f = fopen(path, "wb");
    if (!f) {
        return false;
    }
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

ssize_t persistence_read(const char * key, void * data, size_t max_size) {
    char path[256];
    snprintf(path, sizeof(path), "persistence/%s", key);
    FILE * f = fopen(path, "rb");
    if (!f) {
        return -1;
    }
    ssize_t read = fread(data, 1, max_size, f);
    fclose(f);
    return read;
}

// --- Simple Key-Value API for Integers/Booleans ---

void persistence_set_int(const char* key, int value) {
#ifdef SIMULATOR_BUILD
    char js_command[256];
    snprintf(js_command, sizeof(js_command), "localStorage.setItem('%s', '%d');", key, value);
    emscripten_run_script(js_command);
#else
    // For a real device, this would write to NVM or a file
    (void)key;
    (void)value;
#endif
}

int persistence_get_int(const char* key, int default_value) {
#ifdef SIMULATOR_BUILD
    char js_command[256];
    snprintf(js_command, sizeof(js_command), "parseInt(localStorage.getItem('%s'))", key);
    int result = emscripten_run_script_int(js_command);

    // emscripten_run_script_int returns 0 for script errors or non-numeric results.
    // We need to distinguish between a stored '0' and a non-existent value.
    snprintf(js_command, sizeof(js_command), "localStorage.getItem('%s') === null", key);
    if (emscripten_run_script_int(js_command)) {
        return default_value;
    }
    return result;
#else
    // For a real device, this would read from NVM or a file
    (void)key;
    return default_value;
#endif
}
