#include "app/persistence.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

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
