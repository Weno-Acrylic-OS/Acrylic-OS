#include "js_engine.h"
#include "elk.h"
#include "lvgl.h"
#include <stdio.h>

#define JS_MEM_SIZE 4096
static char js_mem[JS_MEM_SIZE];
static struct js *js;

// --- WenoFitOS API ---
static jsval_t js_log(struct js *js, jsval_t *args, int nargs) {
    for (int i = 0; i < nargs; i++) {
        printf("%s ", js_str(js, args[i]));
    }
    printf("\\n");
    return js_mkundef();
}

// --- LVGL API ---
static jsval_t js_lvgl_create_screen(struct js *js, jsval_t *args, int nargs) {
    return js_mkobj(js); // Dummy implementation
}

static jsval_t js_lvgl_create_label(struct js *js, jsval_t *args, int nargs) {
    return js_mkobj(js); // Dummy implementation
}

static jsval_t js_lvgl_set_label_text(struct js *js, jsval_t *args, int nargs) {
    return js_mkundef(); // Dummy implementation
}

static jsval_t js_lvgl_align_object(struct js *js, jsval_t *args, int nargs) {
    return js_mkundef(); // Dummy implementation
}


void js_engine_init() {
    js = js_create(js_mem, sizeof(js_mem));
    
    // Create the WenoFitOS object
    jsval_t weno_fit_os_obj = js_mkobj(js);
    js_set(js, js_glob(js), "WenoFitOS", weno_fit_os_obj);
    js_set(js, weno_fit_os_obj, "log", js_mkfun(js_log));

    // Create the LVGL object
    jsval_t lvgl_obj = js_mkobj(js);
    js_set(js, js_glob(js), "LVGL", lvgl_obj);
    js_set(js, lvgl_obj, "createScreen", js_mkfun(js_lvgl_create_screen));
    js_set(js, lvgl_obj, "createLabel", js_mkfun(js_lvgl_create_label));
    js_set(js, lvgl_obj, "setLabelText", js_mkfun(js_lvgl_set_label_text));
    js_set(js, lvgl_obj, "alignObject", js_mkfun(js_lvgl_align_object));

    // Add LVGL constants
    js_set(js, lvgl_obj, "ALIGN_CENTER", js_mknum(LV_ALIGN_CENTER));
}

void js_engine_run(const char * js_code) {
    js_eval(js, js_code, ~0);
}
