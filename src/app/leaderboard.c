#include "app/leaderboard.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include "lvgl.h" // Required for LVGL file system

void create_leaderboard_app(lv_obj_t * parent) {
    lv_obj_set_style_bg_color(parent, lv_color_black(), 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, 10, 0);

    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "Global Leaderboard");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_pad_bottom(title, 10, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(85));
    lv_obj_align(list, LV_ALIGN_BOTTOM_MID, 0, 0);

    // --- File Reading Logic ---
    lv_fs_file_t f;
    lv_fs_res_t res;
    res = lv_fs_open(&f, "A:assets/data/leaderboard.json", LV_FS_MODE_RD);

    if(res != LV_FS_RES_OK) {
        lv_list_add_text(list, "Failed to open leaderboard data");
        return;
    }

    uint32_t read_bytes;
    char buf[1024];
    res = lv_fs_read(&f, buf, sizeof(buf) - 1, &read_bytes);
    
    if(res != LV_FS_RES_OK || read_bytes == 0) {
        lv_list_add_text(list, "Failed to read leaderboard data");
        lv_fs_close(&f);
        return;
    }

    // Null-terminate the buffer
    buf[read_bytes] = '\0';
    lv_fs_close(&f);

    // --- JSON Parsing and UI Creation ---
    cJSON *root = cJSON_Parse(buf);
    if (root == NULL || !cJSON_IsArray(root)) {
        lv_list_add_text(list, "Could not parse data");
        if (root != NULL) cJSON_Delete(root);
        return;
    }

    cJSON *entry = NULL;
    cJSON_ArrayForEach(entry, root) {
        cJSON *rank = cJSON_GetObjectItem(entry, "rank");
        cJSON *name = cJSON_GetObjectItem(entry, "name");
        cJSON *steps = cJSON_GetObjectItem(entry, "steps");

        if (cJSON_IsNumber(rank) && cJSON_IsString(name) && cJSON_IsNumber(steps)) {
            char text_buf[100];
            snprintf(text_buf, sizeof(text_buf), "#%d %s - %d steps", rank->valueint, name->valuestring, steps->valueint);
            lv_obj_t* btn = lv_list_add_btn(list, NULL, text_buf);

            // Highlight the "You" entry
            if (strcmp(name->valuestring, "You") == 0) {
                lv_obj_set_style_bg_color(btn, lv_color_hex(0x003060), LV_PART_MAIN);
            }
        }
    }

    cJSON_Delete(root);
}
