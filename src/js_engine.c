#include "js_engine.h"
#include "elk.h"
#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include "app/watchface_service.h"
#include "app/today_service.h"
#include "app/time_service.h"
#include "app/gamification_service.h"
#include "drivers/heart_rate.h"

#define JS_MEM_SIZE 4096
static char js_mem[JS_MEM_SIZE];
static struct js *js;

// --- C <-> JS Bridges ---

// We can only have one active JS watchface and one active JS widget at a time
// due to the limitations of this C->JS bridge design.
static jsval_t js_watchface_create_func = 0;
static jsval_t js_widget_create_func = 0;
static jsval_t js_watchface_timer_func = 0; // For the watchface update timer

static void js_watchface_create_bridge(lv_obj_t *parent) {
    if (js_watchface_create_func != 0) {
        char script[128];
        snprintf(script, sizeof(script), "WenoFitOS._watchface_create(%lu)", (unsigned long)(uintptr_t)parent);
        js_eval(js, script, ~0);
    }
}

static void js_widget_create_bridge(lv_obj_t *parent) {
    if (js_widget_create_func != 0) {
        char script[128];
        snprintf(script, sizeof(script), "WenoFitOS._widget_create(%lu)", (unsigned long)(uintptr_t)parent);
        js_eval(js, script, ~0);
    }
}

static void js_watchface_timer_bridge(lv_timer_t *timer) {
    LV_UNUSED(timer);
    if (js_watchface_timer_func != 0) {
        js_eval(js, "WenoFitOS._watchface_timer()", ~0);
    }
}


// --- WenoFitOS API ---

static jsval_t js_log(struct js *js, jsval_t *args, int nargs) {
    for (int i = 0; i < nargs; i++) {
        printf("%s ", js_str(js, args[i]));
    }
    printf("\n");
    return js_mkundef();
}

static jsval_t js_create_timer(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2 || js_type(args[1]) != JS_NUM) {
        return js_mkerr(js, "Usage: createTimer(callback_func, period_ms)");
    }
    js_watchface_timer_func = args[0];
    uint32_t period = (uint32_t)js_getnum(args[1]);

    // Store the callback globally so the bridge can find it
    js_set(js, js_glob(js), "WenoFitOS._watchface_timer", js_watchface_timer_func);

    lv_timer_create(js_watchface_timer_bridge, period, NULL);
    return js_mkundef();
}

static jsval_t js_register_watchface(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2 || js_type(args[0]) != JS_STR) {
        return js_mkerr(js, "Usage: registerWatchface(name, create_func)");
    }
    const char *name = js_str(js, args[0]);
    js_watchface_create_func = args[1];

    // The JS function needs to be stored on a globally accessible object
    js_set(js, js_glob(js), "WenoFitOS._watchface_create", js_watchface_create_func);
    
    static watchface_descriptor_t desc;
    desc.name = name;
    desc.create_func = js_watchface_create_bridge;
    watchface_service_register(&desc);

    return js_mkundef();
}

static jsval_t js_register_widget(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2 || js_type(args[0]) != JS_STR) {
        return js_mkerr(js, "Usage: registerWidget(name, create_func)");
    }
    const char *name = js_str(js, args[0]);
    js_widget_create_func = args[1];

    js_set(js, js_glob(js), "WenoFitOS._widget_create", js_widget_create_func);

    static today_widget_descriptor_t desc;
    desc.name = name;
    desc.create_func = js_widget_create_bridge;
    today_service_register_widget(&desc);

    return js_mkundef();
}

// --- Data API ---

static jsval_t js_get_hours(struct js *js, jsval_t *args, int nargs) {
    (void)js; (void)args; (void)nargs;
    return js_mknum(time_service_get_hours());
}

static jsval_t js_get_minutes(struct js *js, jsval_t *args, int nargs) {
    (void)js; (void)args; (void)nargs;
    return js_mknum(time_service_get_minutes());
}

static jsval_t js_get_seconds(struct js *js, jsval_t *args, int nargs) {
    (void)js; (void)args; (void)nargs;
    return js_mknum(time_service_get_seconds());
}

static jsval_t js_get_steps(struct js *js, jsval_t *args, int nargs) {
    (void)js; (void)args; (void)nargs;
    int count = 0;
    const goal_t *goals = gamification_get_goals(&count);
    for(int i = 0; i < count; i++) {
        if (goals[i].type == GOAL_TYPE_DAILY_STEPS) {
            return js_mknum(goals[i].current_value);
        }
    }
    return js_mknum(0);
}

static jsval_t js_get_heart_rate(struct js *js, jsval_t *args, int nargs) {
    (void)js; (void)args; (void)nargs;
    return js_mknum(heart_rate_get_bpm());
}


// --- LVGL API ---

static jsval_t js_lvgl_create_screen(struct js *js, jsval_t *args, int nargs) {
    (void)js; (void)args; (void)nargs;
    return js_mknum((uintptr_t)lv_scr_act());
}

static jsval_t js_lvgl_create_label(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 1) return js_mkerr(js, "createLabel requires a parent object address");
    lv_obj_t *parent = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_obj_t *label = lv_label_create(parent);
    return js_mknum((uintptr_t)label);
}

static jsval_t js_lvgl_create_object(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 1) return js_mkerr(js, "createObject requires a parent object address");
    lv_obj_t *parent = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_obj_t *obj = lv_obj_create(parent);
    return js_mknum((uintptr_t)obj);
}

static jsval_t js_lvgl_set_label_text(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2) return js_mkerr(js, "setLabelText requires an object address and a string");
    lv_obj_t *label = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    const char *text = js_str(js, args[1]);
    lv_label_set_text(label, text);
    return js_mkundef();
}

static jsval_t js_lvgl_align_object(struct js *js, jsval_t *args, int nargs) {
    if (nargs < 2) return js_mkerr(js, "alignObject requires an object address and an alignment");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_align_t align = (lv_align_t)js_getnum(args[1]);
    lv_coord_t x_ofs = (nargs > 2) ? (lv_coord_t)js_getnum(args[2]) : 0;
    lv_coord_t y_ofs = (nargs > 3) ? (lv_coord_t)js_getnum(args[3]) : 0;
    lv_obj_align(obj, align, x_ofs, y_ofs);
    return js_mkundef();
}

static jsval_t js_lvgl_set_bg_color(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2) return js_mkerr(js, "setBgColor requires objAddr, colorHex");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    uint32_t color_hex = (uint32_t)js_getnum(args[1]);
    lv_obj_set_style_bg_color(obj, lv_color_hex(color_hex), 0);
    return js_mkundef();
}

static jsval_t js_lvgl_set_radius(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2) return js_mkerr(js, "setRadius requires objAddr, radius");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_coord_t radius = (lv_coord_t)js_getnum(args[1]);
    lv_obj_set_style_radius(obj, radius, 0);
    return js_mkundef();
}

static jsval_t js_lvgl_set_size(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 3) return js_mkerr(js, "setSize requires objAddr, width, and height");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_coord_t w = (lv_coord_t)js_getnum(args[1]);
    lv_coord_t h = (lv_coord_t)js_getnum(args[2]);
    // Use negative numbers to signify percentage
    if (w < 0 && w >= -100) w = lv_pct(w * -1);
    if (h < 0 && h >= -100) h = lv_pct(h * -1);
    lv_obj_set_size(obj, w, h);
    return js_mkundef();
}

static jsval_t js_lvgl_set_pad_all(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2) return js_mkerr(js, "setPadAll requires objAddr, padding");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_coord_t pad = (lv_coord_t)js_getnum(args[1]);
    lv_obj_set_style_pad_all(obj, pad, 0);
    return js_mkundef();
}

static jsval_t js_lvgl_set_border_width(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2) return js_mkerr(js, "setBorderWidth requires objAddr, width");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_coord_t width = (lv_coord_t)js_getnum(args[1]);
    lv_obj_set_style_border_width(obj, width, 0);
    return js_mkundef();
}

static jsval_t js_lvgl_set_flex_flow(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2) return js_mkerr(js, "setFlexFlow requires objAddr, flow");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    lv_flex_flow_t flow = (lv_flex_flow_t)js_getnum(args[1]);
    lv_obj_set_flex_flow(obj, flow);
    return js_mkundef();
}

static jsval_t js_lvgl_set_text_color(struct js *js, jsval_t *args, int nargs) {
    if (nargs != 2) return js_mkerr(js, "setTextColor requires objAddr, colorHex");
    lv_obj_t *obj = (lv_obj_t *)(uintptr_t)js_getnum(args[0]);
    uint32_t color_hex = (uint32_t)js_getnum(args[1]);
    lv_obj_set_style_text_color(obj, lv_color_hex(color_hex), 0);
    return js_mkundef();
}

void js_engine_init() {
    js = js_create(js_mem, sizeof(js_mem));
    
    jsval_t weno_fit_os_obj = js_mkobj(js);
    js_set(js, js_glob(js), "WenoFitOS", weno_fit_os_obj);
    js_set(js, weno_fit_os_obj, "log", js_mkfun(js_log));
    js_set(js, weno_fit_os_obj, "registerWatchface", js_mkfun(js_register_watchface));
    js_set(js, weno_fit_os_obj, "registerWidget", js_mkfun(js_register_widget));
    js_set(js, weno_fit_os_obj, "createTimer", js_mkfun(js_create_timer));

    // Data API
    js_set(js, weno_fit_os_obj, "getHours", js_mkfun(js_get_hours));
    js_set(js, weno_fit_os_obj, "getMinutes", js_mkfun(js_get_minutes));
    js_set(js, weno_fit_os_obj, "getSeconds", js_mkfun(js_get_seconds));
    js_set(js, weno_fit_os_obj, "getSteps", js_mkfun(js_get_steps));
    js_set(js, weno_fit_os_obj, "getHeartRate", js_mkfun(js_get_heart_rate));

    jsval_t lvgl_obj = js_mkobj(js);
    js_set(js, js_glob(js), "LVGL", lvgl_obj);
    js_set(js, lvgl_obj, "createScreen", js_mkfun(js_lvgl_create_screen));
    js_set(js, lvgl_obj, "createLabel", js_mkfun(js_lvgl_create_label));
    js_set(js, lvgl_obj, "createObject", js_mkfun(js_lvgl_create_object));
    js_set(js, lvgl_obj, "setLabelText", js_mkfun(js_lvgl_set_label_text));
    js_set(js, lvgl_obj, "alignObject", js_mkfun(js_lvgl_align_object));
    js_set(js, lvgl_obj, "setBgColor", js_mkfun(js_lvgl_set_bg_color));
    js_set(js, lvgl_obj, "setRadius", js_mkfun(js_lvgl_set_radius));
    js_set(js, lvgl_obj, "setSize", js_mkfun(js_lvgl_set_size));
    js_set(js, lvgl_obj, "setPadAll", js_mkfun(js_lvgl_set_pad_all));
    js_set(js, lvgl_obj, "setBorderWidth", js_mkfun(js_lvgl_set_border_width));
    js_set(js, lvgl_obj, "setFlexFlow", js_mkfun(js_lvgl_set_flex_flow));
    js_set(js, lvgl_obj, "setTextColor", js_mkfun(js_lvgl_set_text_color));

    // Constants
    js_set(js, lvgl_obj, "ALIGN_CENTER", js_mknum(LV_ALIGN_CENTER));
    js_set(js, lvgl_obj, "FLEX_FLOW_ROW", js_mknum(LV_FLEX_FLOW_ROW));
    js_set(js, lvgl_obj, "FLEX_FLOW_COLUMN", js_mknum(LV_FLEX_FLOW_COLUMN));
}

void js_engine_run(const char * js_code) {
    js_eval(js, js_code, ~0);
}
