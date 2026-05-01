#ifndef PHONE_SERVICE_H
#define PHONE_SERVICE_H

#include <stdbool.h>
#include "app/music.h"

void phone_service_init();
void phone_service_music_play();
void phone_service_music_pause();
void phone_service_music_next();
void phone_service_music_prev();
bool phone_service_is_playing();
music_track_t phone_service_get_current_track();

#endif // PHONE_SERVICE_H
