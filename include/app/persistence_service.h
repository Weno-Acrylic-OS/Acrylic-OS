#ifndef PERSISTENCE_SERVICE_H
#define PERSISTENCE_SERVICE_H

#include "app/quick_settings_model.h"

void persistence_service_init();
void persistence_service_load();
void persistence_service_save();

void persistence_service_get_quick_settings(quick_setting_id_t* settings, int* count);
void persistence_service_set_quick_settings(const quick_setting_id_t* settings, int count);


#endif // PERSISTENCE_SERVICE_H
