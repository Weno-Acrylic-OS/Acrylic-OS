// src/app/watchface_studio.c
#include "app/watchface_studio.h"
#include "app/watchface.h"

static bool show_steps_state = true;
static bool show_hr_state = true;
static bool show_date_state = true;

static lv_obj_t * cb_steps;
static lv_obj_t * cb_hr;
static lv_obj_t * cb_date;

static void style_dropdown_event_handler(lv_event_t * e)
{
    lv_obj_t * dropdown = lv_event_get_target(e);
    int selected_option = lv_dropdown_get_selected(dropdown);
    update_watchface_style(selected_option);
}

static void widget_checkbox_event_handler(lv_event_t * e)
{
    show_steps_state = lv_obj_has_state(cb_steps, LV_STATE_CHECKED);
    show_hr_state = lv_obj_has_state(cb_hr, LV_STATE_CHECKED);
    show_date_state = lv_obj_has_state(cb_date, LV_STATE_CHECKED);
    
    update_watchface_widgets(show_steps_state, show_hr_state, show_date_state);
}

void create_watchface_studio(lv_obj_t * parent)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_size(panel, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 20, 0);

    lv_obj_t * title = lv_label_create(panel);
    lv_label_set_text(title, "Watch Face Studio");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_pad_bottom(title, 20, 0);

    // Style dropdown
    lv_obj_t * style_label = lv_label_create(panel);
    lv_label_set_text(style_label, "Watch Face Style");
    lv_obj_t * style_dropdown = lv_dropdown_create(panel);
    lv_dropdown_set_options(style_dropdown, "Default\nMinimal\nAnalog");
    lv_obj_add_event_cb(style_dropdown, style_dropdown_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // Widgets checkboxes
    lv_obj_t * widgets_label = lv_label_create(panel);
    lv_label_set_text(widgets_label, "Widgets");
    cb_steps = lv_checkbox_create(panel);
    lv_checkbox_set_text(cb_steps, "Show Steps");
    lv_obj_add_state(cb_steps, LV_STATE_CHECKED);
    lv_obj_add_event_cb(cb_steps, widget_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    cb_hr = lv_checkbox_create(panel);
    lv_checkbox_set_text(cb_hr, "Show Heart Rate");
    lv_obj_add_state(cb_hr, LV_STATE_CHECKED);
    lv_obj_add_event_cb(cb_hr, widget_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    cb_date = lv_checkbox_create(panel);
    lv_checkbox_set_text(cb_date, "Show Date");
    lv_obj_add_state(cb_date, LV_STATE_CHECKED);
    lv_obj_add_event_cb(cb_date, widget_checkbox_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}