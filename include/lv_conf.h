#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_COLOR_DEPTH 16

#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (48U * 1024U)

#ifdef SIMULATOR_BUILD
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "emscripten.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (emscripten_get_now())
#else
#define LV_TICK_CUSTOM 0
#endif

#define LV_BUILD_EXAMPLES 0

#define LV_USE_TABVIEW 1
#define LV_USE_LIST 1
#define LV_USE_ARC 1
#define LV_USE_BAR 1
#define LV_USE_BTN 1
#define LV_USE_BTNMATRIX 1
#define LV_USE_CANVAS 0
#define LV_USE_CHECKBOX 1
#define LV_USE_DROPDOWN 1
#define LV_USE_IMG 1
#define LV_USE_LABEL 1
#define LV_USE_LINE 1
#define LV_USE_ROLLER 1
#define LV_USE_SLIDER 0
#define LV_USE_SWITCH 0
#define LV_USE_TEXTAREA 1
#define LV_USE_TABLE 0
#define LV_USE_LED 0

/*==================
 * EXTRA COMPONENTS
 *==================*/
#define LV_USE_CALENDAR 0
#define LV_USE_CHART 1
#define LV_USE_COLORWHEEL 1
#define LV_USE_IMGBTN 0
#define LV_USE_KEYBOARD 1
#define LV_USE_MENU 0
#define LV_USE_METER 0
#define LV_USE_MSGBOX 1
#define LV_USE_SPAN 0
#define LV_USE_SPINBOX 0
#define LV_USE_SPINNER 0
#define LV_USE_TILEVIEW 0
#define LV_USE_WIN 0
#define LV_USE_ANIMIMG 0

/*==================
 *   FONT USAGE
 *===================*/

/*Montserrat fonts with ASCII range and some symbols using bpp = 4
 *https://fonts.google.com/specimen/Montserrat*/
 #define LV_FONT_MONTSERRAT_12 1  /* Keep this as our "base" data */
 #define LV_FONT_MONTSERRAT_14 1
 #define LV_FONT_MONTSERRAT_16 1
 #define LV_FONT_MONTSERRAT_18 1
 #define LV_FONT_MONTSERRAT_20 1
 #define LV_FONT_MONTSERRAT_22 1
 #define LV_FONT_MONTSERRAT_24 1
 #define LV_FONT_MONTSERRAT_28 1
 #define LV_FONT_MONTSERRAT_32 1
 #define LV_FONT_MONTSERRAT_48 1
 
 /* Set the default font to the one we actually have data for */
 #define LV_FONT_DEFAULT &lv_font_montserrat_12

#define LV_USE_PNG 1

#endif
