#include "app/navigation_service.h"
#include <stdlib.h> // For NULL

#define MAX_NAV_STACK_SIZE 10

// --- Module-level State ---
static lv_obj_t * app_container_ref;
static lv_obj_t * tab_container_ref;
static lv_obj_t * nav_bar;

static screen_create_func_t screen_stack[MAX_NAV_STACK_SIZE];
static int stack_top = -1;

// --- Forward Declarations ---
static void back_event_handler(lv_event_t * e);
static void home_event_handler(lv_event_t * e);
static void show_nav_bar(bool show);
static void navigate_back();
static void push_screen_to_stack(screen_create_func_t create_func);

// --- UI Management ---
static void show_nav_bar(bool show) {
    if (!nav_bar) return;
    if (show) {
        lv_obj_clear_flag(nav_bar, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(nav_bar, LV_OBJ_FLAG_HIDDEN);
    }
}

// --- Event Handlers ---
static void back_event_handler(lv_event_t * e) { (void)e; navigate_back(); }
static void home_event_handler(lv_event_t * e) {
    (void)e;
    stack_top = -1;
    lv_obj_add_flag(app_container_ref, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(tab_container_ref, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clean(app_container_ref);
    show_nav_bar(false);
}

// --- Navigation Logic ---
static void push_screen_to_stack(screen_create_func_t create_func) {
    if (stack_top < MAX_NAV_STACK_SIZE - 1) {
        stack_top++;
        screen_stack[stack_top] = create_func;
    }
}

static void navigate_back() {
    if (stack_top <= 0) {
        home_event_handler(NULL);
        return;
    }
    stack_top--;
    screen_create_func_t prev_screen_func = screen_stack[stack_top];
    lv_obj_clean(app_container_ref);
    prev_screen_func(app_container_ref);
}

// --- Public API ---
void navigation_service_init(lv_obj_t * app_container, lv_obj_t * tab_container) {
    app_container_ref = app_container;
    tab_container_ref = tab_container;
    stack_top = -1;

    nav_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(nav_bar, lv_pct(100), 50);
    lv_obj_align(nav_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(nav_bar, lv_color_hex(0x222222), 0);
    lv_obj_set_style_radius(nav_bar, 0, 0);
    lv_obj_set_style_border_width(nav_bar, 0, 0);
    lv_obj_set_flex_flow(nav_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(nav_bar, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * back_btn = lv_btn_create(nav_bar);
    lv_obj_add_event_cb(back_btn, back_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);

    lv_obj_t * home_btn = lv_btn_create(nav_bar);
    lv_obj_add_event_cb(home_btn, home_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * home_label = lv_label_create(home_btn);
    lv_label_set_text(home_label, LV_SYMBOL_HOME);

    show_nav_bar(false);
}

void navigation_service_navigate_to(screen_create_func_t create_func) {
    if (stack_top >= MAX_NAV_STACK_SIZE - 1) return;

    if (stack_top == -1) {
        extern void create_app_list(lv_obj_t * parent);
        push_screen_to_stack(create_app_list);
    }
    
    push_screen_to_stack(create_func);

    lv_obj_add_flag(tab_container_ref, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(app_container_ref, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clean(app_container_ref);
    create_func(app_container_ref);

    show_nav_bar(true);
}
