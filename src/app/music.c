#include "app/music.h"
#include "app/phone_service.h"
#include "lvgl.h"

static lv_obj_t * title_label;
static lv_obj_t * artist_label;
static lv_obj_t * play_pause_label;

static void update_track_info() {
    music_track_t track = phone_service_get_current_track();
    lv_label_set_text(title_label, track.title);
    lv_label_set_text(artist_label, track.artist);
}

static void play_pause_event_handler(lv_event_t * e) {
    if (phone_service_is_playing()) {
        phone_service_music_pause();
        lv_label_set_text(play_pause_label, LV_SYMBOL_PLAY);
    } else {
        phone_service_music_play();
        lv_label_set_text(play_pause_label, LV_SYMBOL_PAUSE);
    }
}

static void prev_event_handler(lv_event_t * e) {
    phone_service_music_prev();
    update_track_info();
}

static void next_event_handler(lv_event_t * e) {
    phone_service_music_next();
    update_track_info();
}

static void track_updater_task(lv_timer_t * timer) {
    update_track_info();
}

void create_music_app(lv_obj_t * parent) {
    lv_obj_clean(parent);

    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_center(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    title_label = lv_label_create(container);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_24, 0);

    artist_label = lv_label_create(container);
    lv_obj_set_style_text_font(artist_label, &lv_font_montserrat_18, 0);

    update_track_info();

    lv_obj_t * controls_container = lv_obj_create(container);
    lv_obj_set_size(controls_container, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(controls_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(controls_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * prev_btn = lv_btn_create(controls_container);
    lv_obj_add_event_cb(prev_btn, prev_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * prev_label = lv_label_create(prev_btn);
    lv_label_set_text(prev_label, LV_SYMBOL_PREV);

    lv_obj_t * play_pause_btn = lv_btn_create(controls_container);
    lv_obj_add_event_cb(play_pause_btn, play_pause_event_handler, LV_EVENT_CLICKED, NULL);
    play_pause_label = lv_label_create(play_pause_btn);
    lv_label_set_text(play_pause_label, phone_service_is_playing() ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);

    lv_obj_t * next_btn = lv_btn_create(controls_container);
    lv_obj_add_event_cb(next_btn, next_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * next_label = lv_label_create(next_btn);
    lv_label_set_text(next_label, LV_SYMBOL_NEXT);

    lv_timer_create(track_updater_task, 1000, NULL);
}
