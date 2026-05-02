#include "app/gps.h"
#include "app/privacy_service.h"
#include "lvgl.h"

// --- Forward Declarations ---
static void create_destination_selection_screen(lv_obj_t * parent);
static void update_navigation_display();
static void start_route(navigation_instruction_t * route, size_t route_size, lv_obj_t * parent);

// --- Mock Navigation Data ---
static navigation_instruction_t mock_route_home[] = {
    {TURN_RIGHT, "Main St", 300.0},
    {TURN_LEFT, "Oak Ave", 500.0},
    {ARRIVED, "Home", 0.0}
};
static navigation_instruction_t mock_route_work[] = {
    {STRAIGHT, "Highway 101", 1200.0},
    {TURN_RIGHT, "Commerce Blvd", 250.0},
    {TURN_LEFT, "Office Park Rd", 500.0},
    {ARRIVED, "Work", 0.0}
};

// --- State Management ---
static navigation_instruction_t * active_route = NULL;
static size_t active_route_size = 0;
static int current_route_step = 0;
static double distance_to_next_turn = 0;

// --- UI Objects ---
static lv_obj_t * direction_label;
static lv_obj_t * distance_label;
static lv_obj_t * street_label;
static lv_timer_t * navigation_timer = NULL;


static void stop_navigation() {
    if(navigation_timer) {
        lv_timer_del(navigation_timer);
        navigation_timer = NULL;
    }
    active_route = NULL;
    active_route_size = 0;
    privacy_service_set_sensor_active(PRIVACY_SENSOR_LOCATION, false);
}

static void start_route(navigation_instruction_t * route, size_t route_size, lv_obj_t * parent) {
    active_route = route;
    active_route_size = route_size;
    current_route_step = 0;
    distance_to_next_turn = active_route[current_route_step].distance_meters;
    privacy_service_set_sensor_active(PRIVACY_SENSOR_LOCATION, true);
    create_gps_app(parent); // Refresh the view
}

static void destination_selection_event_handler(lv_event_t * e) {
    const char * selection = lv_event_get_user_data(e);
    lv_obj_t * parent = lv_obj_get_parent(lv_obj_get_parent(lv_event_get_target(e))); // button -> list -> parent

    if (strcmp(selection, "Home") == 0) {
        start_route(mock_route_home, sizeof(mock_route_home) / sizeof(navigation_instruction_t), parent);
    } else if (strcmp(selection, "Work") == 0) {
        start_route(mock_route_work, sizeof(mock_route_work) / sizeof(navigation_instruction_t), parent);
    }
}

static void create_destination_selection_screen(lv_obj_t * parent) {
    lv_obj_clean(parent);
    
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    lv_list_add_text(list, "Set Destination");

    lv_obj_t * home_btn = lv_list_add_btn(list, LV_SYMBOL_HOME, "Home");
    lv_obj_add_event_cb(home_btn, destination_selection_event_handler, LV_EVENT_CLICKED, "Home");

    lv_obj_t * work_btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "Work"); // Using placeholder icon
    lv_obj_add_event_cb(work_btn, destination_selection_event_handler, LV_EVENT_CLICKED, "Work");
}

static void set_destination_event_handler(lv_event_t * e) {
    create_destination_selection_screen(lv_obj_get_parent(lv_event_get_target(e)));
}

// --- Functions ---
static void update_navigation_display() {
    if (!active_route) return;

    navigation_instruction_t current_instruction = active_route[current_route_step];

    const char * dir_symbol = "";
    switch (current_instruction.direction) {
        case TURN_LEFT:  dir_symbol = LV_SYMBOL_LEFT; break;
        case TURN_RIGHT: dir_symbol = LV_SYMBOL_RIGHT; break;
        case STRAIGHT:   dir_symbol = LV_SYMBOL_UP; break;
        case ARRIVED:    dir_symbol = LV_SYMBOL_OK; break;
    }
    lv_label_set_text(direction_label, dir_symbol);

    char dist_str[32];
    if (distance_to_next_turn > 1000) {
        lv_snprintf(dist_str, sizeof(dist_str), "%.1f km", distance_to_next_turn / 1000.0);
    } else {
        lv_snprintf(dist_str, sizeof(dist_str), "%.0f m", distance_to_next_turn);
    }
    lv_label_set_text(distance_label, dist_str);

    if (current_instruction.direction == ARRIVED) {
        lv_label_set_text(street_label, "You have arrived!");
        stop_navigation();
    } else {
        lv_label_set_text(street_label, current_instruction.street_name);
    }
}

static void navigation_updater_task(lv_timer_t * timer) {
    // Simulate moving at ~10 m/s for a 1 second timer
    distance_to_next_turn -= 10;

    if (distance_to_next_turn <= 0) {
        current_route_step++;
        if (current_route_step >= active_route_size) {
            stop_navigation();
            create_gps_app(lv_obj_get_parent(lv_scr_act())); // Should be safe if parent is screen
            return;
        }
        
        navigation_instruction_t next_instruction = active_route[current_route_step];
        distance_to_next_turn = next_instruction.distance_meters;
    }
    update_navigation_display();
}

void create_gps_app(lv_obj_t * parent) {
    lv_obj_clean(parent);
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x000000), 0);

    if (active_route) {
        // --- NAVIGATION VIEW ---
        direction_label = lv_label_create(parent);
        lv_obj_set_style_text_font(direction_label, &lv_font_montserrat_48, 0);
        lv_obj_set_style_text_color(direction_label, lv_color_hex(0xFFFFFF), 0);
        lv_obj_align(direction_label, LV_ALIGN_TOP_MID, 0, 40);

        distance_label = lv_label_create(parent);
        lv_obj_set_style_text_font(distance_label, &lv_font_montserrat_32, 0);
        lv_obj_set_style_text_color(distance_label, lv_color_hex(0xFFFFFF), 0);
        lv_obj_align_to(distance_label, direction_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

        street_label = lv_label_create(parent);
        lv_obj_set_style_text_font(street_label, &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_color(street_label, lv_color_hex(0xDDDDDD), 0);
        lv_obj_align_to(street_label, distance_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

        update_navigation_display();

        if (navigation_timer == NULL) {
            navigation_timer = lv_timer_create(navigation_updater_task, 1000, NULL);
        }
    } else {
        // --- NO ROUTE VIEW ---
        lv_obj_t * set_dest_btn = lv_btn_create(parent);
        lv_obj_align(set_dest_btn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(set_dest_btn, set_destination_event_handler, LV_EVENT_CLICKED, NULL);
        
        lv_obj_t * btn_label = lv_label_create(set_dest_btn);
        lv_label_set_text(btn_label, "Set Destination");
    }
}
