#define LV_USE_LIST       1

#define LV_USE_MENU       0

#define LV_USE_METER      0

#define LV_USE_MSGBOX     0

#define LV_USE_SPAN       0
#if LV_USE_SPAN
    /*A line text can contain maximum num of span descriptor */
    #define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif

#define LV_USE_SPINBOX    0

#define LV_USE_SPINNER    0

#define LV_USE_TABVIEW    1

#define LV_USE_TILEVIEW   0

#define LV_USE_WIN        0

#define LV_USE_PNG        1