#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

void persistence_init();
bool persistence_get_oobe_completed();
void persistence_set_oobe_completed(bool completed);

bool persistence_write(const char * key, const void * data, size_t size);
ssize_t persistence_read(const char * key, void * data, size_t max_size);

// Simple Key-Value API for integers/booleans
void persistence_set_int(const char* key, int value);
int persistence_get_int(const char* key, int default_value);


#endif // PERSISTENCE_H
