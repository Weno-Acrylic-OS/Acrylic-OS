#ifndef NOTIFICATION_SERVICE_H
#define NOTIFICATION_SERVICE_H

#include <stdbool.h>

#define MAX_NOTIFICATIONS 10

typedef struct {
    char app_name[50];
    char title[100];
    char body[256];
} notification_t;

void notification_service_init(void);
bool notification_service_send(const char* app_name, const char* title, const char* body);
const notification_t* notification_service_get_notifications(int* count);
void notification_service_clear_all(void);

#endif // NOTIFICATION_SERVICE_H
