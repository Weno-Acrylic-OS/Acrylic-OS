#ifndef MUSIC_H
#define MUSIC_H

#include "lvgl.h"

// Data structure for music track information
typedef struct {
    const char * artist;
    const char * title;
} music_track_t;

// Function to create the music app UI
void create_music_app(lv_obj_t * parent);

#endif // MUSIC_H
