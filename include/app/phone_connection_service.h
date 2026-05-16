#ifndef PHONE_CONNECTION_SERVICE_H
#define PHONE_CONNECTION_SERVICE_H

#include <stdbool.h>

void phone_connection_service_init();
bool phone_connection_service_is_connected();
void phone_connection_service_show_required_screen();

#endif // PHONE_CONNECTION_SERVICE_H
