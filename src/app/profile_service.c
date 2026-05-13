#include "app/profile_service.h"
#include "app/persistence.h"

#define DEFAULT_AGE 30
#define PERSISTENCE_KEY_AGE "user_age"

static uint8_t current_user_age;

void profile_service_init(void) {
    current_user_age = persistence_get_int(PERSISTENCE_KEY_AGE, DEFAULT_AGE);
}

void profile_service_set_age(uint8_t age) {
    if (age > 10 && age < 100) { // Basic validation
        current_user_age = age;
        persistence_set_int(PERSISTENCE_KEY_AGE, current_user_age);
    }
}

uint8_t profile_service_get_age(void) {
    return current_user_age;
}
