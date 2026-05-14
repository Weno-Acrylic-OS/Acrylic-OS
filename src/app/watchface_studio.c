// src/app/watchface_studio.c
#include "app/watchface_studio.h"
#include "app/watchface.h"
#include "app/custom_watch_face.h"
#include "app/navigation_service.h"
#include "app/persistence.h"
#include "app/widget_registry.h"
#include "lvgl.h"

// --- Forward Declarations ---
static void add_widget_event_handler(lv_event_t * e);
static void clock_style_editor_event_handler(lv_event_t * e);
static void color_wheel_event_handler(lv_event_t * e);
static void font_dropdown_event_handler(lv_event_t * e);
static void save_event_handler(lv_event_t * e);
static void date_style_editor_event_handler(lv_event_t * e);
static void date_color_wheel_event_handler(lv_event_t * e);
static void widget_gallery_event_handler(lv_event_t * e);
static void select_wallpaper_event_handler(lv_event_t * e);
static void wallpaper_gallery_event_handler(lv_event_t * e);

// --- Module-level State ---
static lv_obj_t * clock_style_panel;
static int current_font_index = 0;
static lv_color_t current_color;

// --- Font Definitions ---
// We need to declare the fonts we want to use.
LV_FONT_DECLARE(lv_font_montserrat_24);
LV_FONT_DECLARE(lv_font_montserrat_32);
LV_FONT_DECLARE(lv_font_montserrat_48);

static const lv_font_t * fonts[] = {
    &lv_font_montserrat_24,
    &lv_font_montserrat_32,
    &lv_font_montserrat_48,
};
static const char * font_names = "Montserrat 24\nMontserrat 32\nMontserrat 48";


static void color_wheel_event_handler(lv_event_t * e)
{
    lv_obj_t * color_wheel = lv_event_get_target(e);
    current_color = lv_colorwheel_get_rgb(color_wheel);
    watchface_set_time_color(current_color);
}

static void font_dropdown_event_handler(lv_event_t * e)
{
    lv_obj_t * dropdown = lv_event_get_target(e);
    current_font_index = lv_dropdown_get_selected(dropdown);
    watchface_set_time_font(fonts[current_font_index]);
}

static void save_event_handler(lv_event_t * e)
{
    (void)e;
    custom_watch_face_t custom_face = {
        .font_index = current_font_index,
        .time_color = current_color,
        .date_color = current_color, // For now, date color is the same as time color
        .widgets = {WIDGET_TYPE_NONE, WIDGET_TYPE_NONE, WIDGET_TYPE_NONE, WIDGET_TYPE_NONE}, // Initialize widgets
        .wallpaper_path = "" // Initialize wallpaper path
    };
    persistence_write("custom_watch_face_0", &custom_face, sizeof(custom_watch_face_t));
}

static void date_color_wheel_event_handler(lv_event_t * e)
{
    lv_obj_t * color_wheel = lv_event_get_target(e);
    lv_color_t color = lv_colorwheel_get_rgb(color_wheel);
    watchface_set_date_color(color);
}

static void date_style_editor_event_handler(lv_event_t * e)
{
    lv_obj_t * master_panel = lv_event_get_user_data(e);
    lv_obj_clean(master_panel); // Remove the "Tap to edit" message or previous editor

    // Re-create the save button as it was cleaned
    lv_obj_t * save_btn = lv_btn_create(master_panel);
    lv_obj_align(save_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(save_btn, save_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save");

    // Create the editor controls
    lv_obj_t * editor_container = lv_obj_create(master_panel);
    lv_obj_set_size(editor_container, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(editor_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(editor_container, 10, 0);

    // Color wheel for the date
    lv_obj_t * cw = lv_colorwheel_create(editor_container, true);
    lv_obj_set_size(cw, 100, 100);
    lv_obj_add_event_cb(cw, date_color_wheel_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}

static void widget_gallery_event_handler(lv_event_t * e)
{
    lv_obj_t * master_panel = lv_event_get_user_data(e);
    lv_obj_clean(master_panel);

    // Re-create the save button as it was cleaned
    lv_obj_t * save_btn = lv_btn_create(master_panel);
    lv_obj_align(save_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(save_btn, save_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save");

    lv_obj_t * list = lv_list_create(master_panel);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    extern const widget_descriptor_t available_widgets[];
    extern const int num_available_widgets;

    for (int i = 0; i < num_available_widgets; i++) {
        lv_obj_t * btn = lv_list_add_btn(list, available_widgets[i].icon, available_widgets[i].name);
        lv_obj_add_event_cb(btn, add_widget_event_handler, LV_EVENT_CLICKED, (void*)&available_widgets[i]);
    }
}

static void wallpaper_gallery_event_handler(lv_event_t * e) {
    (void)e; // Prevent unused parameter warning
    // TODO: implement wallpaper gallery
}

static void add_widget_event_handler(lv_event_t * e)
{
    const widget_descriptor_t * widget_desc = lv_event_get_user_data(e);

    custom_watch_face_t custom_face;
    // Load existing custom face
    if (persistence_read("custom_watch_face_0", &custom_face, sizeof(custom_watch_face_t)) <= 0) {
        // If not found, initialize with defaults
        custom_face.font_index = 0;
        custom_face.time_color = lv_color_white();
        custom_face.date_color = lv_color_white();
        for (int i = 0; i < MAX_WIDGETS; i++) {
            custom_face.widgets[i] = WIDGET_TYPE_NONE;
        }
    }

    // Add the new widget if there's space
    for (int i = 0; i < MAX_WIDGETS; i++) {
        if (custom_face.widgets[i] == WIDGET_TYPE_NONE) {
            custom_face.widgets[i] = widget_desc->id;
            break;
        }
    }

    persistence_write("custom_watch_face_0", &custom_face, sizeof(custom_watch_face_t));

    // Re-create the watchface studio to reset the view and show the new widget
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * list = lv_obj_get_parent(btn);
    lv_obj_t * master_panel = lv_obj_get_parent(list);

    if (master_panel) {
        lv_obj_t * parent_screen = lv_obj_get_parent(lv_obj_get_parent(master_panel)); // Get the screen (parent of main_container)
        if (parent_screen) {
            create_watchface_studio(parent_screen);
        }
    }
}

void create_watchface_studio(lv_obj_t * parent)
{
    // Main container
    lv_obj_t * main_container = lv_obj_create(parent);
    lv_obj_set_size(main_container, lv_pct(100), lv_pct(100));
    lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(main_container, LV_FLEX_FLOW_COLUMN);

    // Live preview area
    lv_obj_t * preview_container = lv_obj_create(main_container);
    lv_obj_set_size(preview_container, lv_pct(100), lv_pct(60));
    create_watchface(preview_container);

    // Master panel for controls
    lv_obj_t * master_panel = lv_obj_create(main_container);
    lv_obj_set_size(master_panel, lv_pct(100), lv_pct(40));
    lv_obj_align(master_panel, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t * initial_message = lv_label_create(master_panel);
    lv_label_set_text(initial_message, "Tap an element to edit");
    lv_obj_center(initial_message);

    lv_obj_t * widget_btn = lv_btn_create(master_panel);
    lv_obj_align(widget_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(widget_btn, widget_gallery_event_handler, LV_EVENT_CLICKED, master_panel);
    lv_obj_t * widget_label = lv_label_create(widget_btn);
    lv_label_set_text(widget_label, "Widgets");

    lv_obj_t * wallpaper_btn = lv_btn_create(master_panel);
    lv_obj_align(wallpaper_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(wallpaper_btn, wallpaper_gallery_event_handler, LV_EVENT_CLICKED, master_panel);
    lv_obj_t * wallpaper_label = lv_label_create(wallpaper_btn);
    lv_label_set_text(wallpaper_label, "Wallpaper");

    // Tappable area for the clock
    lv_obj_t * time_label = watchface_get_time_label();
    if (time_label) {
        lv_obj_add_flag(time_label, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(time_label, clock_style_editor_event_handler, LV_EVENT_CLICKED, master_panel);
        lv_obj_set_style_border_width(time_label, 1, 0);
        lv_obj_set_style_border_color(time_label, lv_color_hex(0x00FFFF), 0);
        lv_obj_set_style_border_opa(time_label, LV_OPA_50, 0);
    }

    lv_obj_t * date_label = watchface_get_date_label();
    if (date_label) {
        lv_obj_add_flag(date_label, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(date_label, date_style_editor_event_handler, LV_EVENT_CLICKED, master_panel);
        lv_obj_set_style_border_width(date_label, 1, 0);
        lv_obj_set_style_border_color(date_label, lv_color_hex(0x00FFFF), 0);
        lv_obj_set_style_border_opa(date_label, LV_OPA_50, 0);
    }
}

// This function is now responsible for showing the editor inside the master_panel
static void clock_style_editor_event_handler(lv_event_t * e)
{
    lv_obj_t * master_panel = lv_event_get_user_data(e);
    lv_obj_clean(master_panel); // Remove the "Tap to edit" message or previous editor

    // Re-create the save button as it was cleaned
    lv_obj_t * save_btn = lv_btn_create(master_panel);
    lv_obj_align(save_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(save_btn, save_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save");

    // Create the editor controls
    lv_obj_t * editor_container = lv_obj_create(master_panel);
    lv_obj_set_size(editor_container, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(editor_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(editor_container, 10, 0);

    // Color wheel
    lv_obj_t * cw = lv_colorwheel_create(editor_container, true);
    lv_obj_set_size(cw, 100, 100);
    lv_obj_add_event_cb(cw, color_wheel_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // Font dropdown
    lv_obj_t * font_dropdown = lv_dropdown_create(editor_container);
    lv_dropdown_set_options(font_dropdown, font_names);
    lv_obj_add_event_cb(font_dropdown, font_dropdown_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}
