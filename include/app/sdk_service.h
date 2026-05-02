#ifndef SDK_SERVICE_H
#define SDK_SERVICE_H

#include "weno-fit-os-sdk/include/app.h"

void sdk_service_init();
void register_app(const app_descriptor_t * descriptor);
app_descriptor_t * get_third_party_app(const char * name);
void sdk_service_run_app(const char * name);

#endif // SDK_SERVICE_H
