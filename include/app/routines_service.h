#ifndef ROUTINES_SERVICE_H
#define ROUTINES_SERVICE_H

#include <stdint.h>

void routines_service_init(void);

// Data structure for a single routine
typedef struct {
    uint8_t hour;
    uint8_t minute;
    const char* name;
    const char* description;
    int last_triggered_day; // To ensure it only fires once per day
} routine_t;

// Public function to get the list of routines
const routine_t* routines_service_get_routines(int* count);

#endif // ROUTINES_SERVICE_H
