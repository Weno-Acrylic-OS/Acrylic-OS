// include/drivers/touch.h
#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>

typedef enum {
    TOUCH_EVENT_NONE,
    TOUCH_EVENT_SWIPE_LEFT,
    TOUCH_EVENT_SWIPE_RIGHT
} TouchEvent;

void touch_init();
TouchEvent touch_get_event();

#endif // TOUCH_H
