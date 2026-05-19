#include "lvgl.h"
#include "drivers/display.h" // For display_draw_pixel

// Native ARM implementation for LVGL display initialization
// This version uses the display_qemu.c driver for pixel drawing.

static void flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            display_draw_pixel(x, y, color_p->full);
            color_p++;
        }
    }

    lv_disp_flush_ready(disp_drv);
}

// Dummy read_cb for input device for now (no touch input in QEMU yet)
static void read_cb(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    (void)indev_drv; // Suppress unused warning
    data->point.x = 0; // No actual touch data
    data->point.y = 0;
    data->state = LV_INDEV_STATE_RELEASED; // Always released
}


void lvgl_display_init(void)
{
    static lv_disp_draw_buf_t draw_buf;
    // Assuming 240x320 resolution from display_qemu.c
    static lv_color_t buf1[240 * 10]; // A portion of the screen
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, 240 * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 320;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = read_cb; // Dummy read_cb
    indev_drv.gesture_limit = 20; // Use the same limit as web build
    lv_indev_drv_register(&indev_drv);
}
