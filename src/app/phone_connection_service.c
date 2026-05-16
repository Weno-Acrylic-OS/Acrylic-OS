#include "app/phone_connection_service.h"
#include "app/navigation_service.h"
#include "app/phone_required_screen.h"

static bool is_connected = false; // Hardcoded for now

void phone_connection_service_init() {
    // In a real implementation, we would register for
    // connection events from the BLE service.
}

bool phone_connection_service_is_connected() {
    return is_connected;
}

void phone_connection_service_show_required_screen() {
    navigation_service_navigate_to(create_phone_required_screen);
}
