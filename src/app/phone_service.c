#include "app/phone_service.h"
#include <emscripten.h>

static bool is_playing = false;
static music_track_t tracks[] = {
    {"Artist 1", "Track 1"},
    {"Artist 2", "Track 2"},
    {"Artist 3", "Track 3"}
};
static int current_track_index = 0;

void phone_service_init() {
    // Nothing to do here for now
}

void phone_service_music_play() {
    is_playing = true;
}

void phone_service_music_pause() {
    is_playing = false;
}

void phone_service_music_next() {
    current_track_index++;
    if (current_track_index >= sizeof(tracks) / sizeof(music_track_t)) {
        current_track_index = 0;
    }
}

void phone_service_music_prev() {
    current_track_index--;
    if (current_track_index < 0) {
        current_track_index = sizeof(tracks) / sizeof(music_track_t) - 1;
    }
}

bool phone_service_is_playing() {
    return is_playing;
}

music_track_t phone_service_get_current_track() {
    return tracks[current_track_index];
}

void phone_service_find_my_phone() {
    emscripten_run_script("alert('Ringing your phone!');");
}

void phone_service_call_for_help() {
    emscripten_run_script("alert('Simulating emergency call to contact...');");
}
