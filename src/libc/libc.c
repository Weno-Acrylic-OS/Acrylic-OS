// src/libc/libc.c
#include <libc.h>
#include <stdarg.h>

// A very simple, non-functional malloc
void* malloc(size_t size) {
    (void)size;
    // This is not a real malloc. We'll need a proper heap implementation later.
    // For now, we just need to satisfy the linker.
    static char heap[1024]; // A tiny, static heap
    return heap;
}

// A simple pseudo-random number generator
static unsigned int next_rand = 1;
int rand() {
    next_rand = next_rand * 1103515245 + 12345;
    return (unsigned int)(next_rand / 65536) % 32768;
}

// A very simple, non-functional sprintf that only handles %d
int sprintf(char* str, const char* format, ...) {
    va_list args;
    va_start(args, format);

    const char* p = format;
    char* s = str;

    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == 'd') {
                int i = va_arg(args, int);
                // A very basic integer to string conversion
                char buf[12];
                char* b = buf;
                if (i < 0) {
                    *s++ = '-';
                    i = -i;
                }
                do {
                    *b++ = (i % 10) + '0';
                    i /= 10;
                } while (i > 0);
                while (b > buf) {
                    *s++ = *--b;
                }
            } else {
                *s++ = *p;
            }
        } else {
            *s++ = *p;
        }
        p++;
    }

    *s = '\0';
    va_end(args);
    return s - str;
}
