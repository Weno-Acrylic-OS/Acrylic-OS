#include "weno-fit-os-sdk/include/phone.h"
#include "app/phone_connection_service.h"

bool weno_phone_is_connected() {
    return phone_connection_service_is_connected();
}

void weno_phone_show_required_screen() {
    phone_connection_service_show_required_screen();
}
