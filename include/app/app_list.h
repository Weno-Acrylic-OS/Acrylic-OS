// include/app/app_list.h
#ifndef APP_LIST_H
#define APP_LIST_H

#include "lvgl.h"

void create_app_list(lv_obj_t * parent);
void add_to_app_list(const char * app_name, const char * icon_path);

#endif // APP_LIST_H
