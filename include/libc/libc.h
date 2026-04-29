// include/libc/libc.h
#ifndef LIBC_H
#define LIBC_H

#include <stddef.h> // For size_t
#include <stdint.h>

void* malloc(size_t size);
int rand();
int sprintf(char* str, const char* format, ...);

#endif // LIBC_H
