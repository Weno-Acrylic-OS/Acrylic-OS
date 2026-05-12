#ifndef DND_SERVICE_H
#define DND_SERVICE_H

#include <stdbool.h>

void dnd_service_init(void);
void dnd_service_toggle(void);
bool dnd_service_is_active(void);

#endif // DND_SERVICE_H
