#include "app/notification_service.h"
#include <string.h>

static notification_t notifications[MAX_NOTIFICATIONS];
static int notification_count = 0;

void notification_service_init(void) {
    notification_count = 0;
    memset(notifications, 0, sizeof(notifications));
}

bool notification_service_send(const char* app_name, const char* title, const char* body) {
    if (notification_count >= MAX_NOTIFICATIONS) {
        // Here we could implement a strategy to discard old notifications
        return false;
    }

    strncpy(notifications[notification_count].app_name, app_name, sizeof(notifications[notification_count].app_name) - 1);
    strncpy(notifications[notification_count].title, title, sizeof(notifications[notification_count].title) - 1);
    strncpy(notifications[notification_count].body, body, sizeof(notifications[notification_count].body) - 1);
    
    notification_count++;
    
    return true;
}

const notification_t* notification_service_get_notifications(int* count) {
    *count = notification_count;
    return notifications;
}

void notification_service_clear_all(void) {
    notification_count = 0;
    memset(notifications, 0, sizeof(notifications));
}
