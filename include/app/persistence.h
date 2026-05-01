#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdbool.h>

void persistence_init();
bool persistence_get_oobe_completed();
void persistence_set_oobe_completed(bool completed);

#endif // PERSISTENCE_H
