// src/drivers/lvgl_display.c

#include "lvgl.h"
#include "display.h"
#include <emscripten.h>

EM_JS(int, get_touch_x, (), {
  return lvgl_touch_x;
});

EM_JS(int, get_touch_y, (), {
  return lvgl_touch_y;
});

EM_JS(int, get_touch_down, (), {
  return lvgl_touch_down;
});

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

static void read_cb(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    data->point.x = get_touch_x();
    data->point.y = get_touch_y();
    data->state = get_touch_down() ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}


void lvgl_display_init(void)
{
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[240 * 10];
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
    indev_drv.read_cb = read_cb;
    lv_indev_drv_register(&indev_drv);
}
