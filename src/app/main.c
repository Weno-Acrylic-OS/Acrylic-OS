// src/app/main.c
#include <stdio.h>
#include <string.h>
#include "lvgl.h"
#include "lvgl_display.h"
#ifdef SIMULATOR_BUILD
#include <emscripten.h>
#endif
#include <stdlib.h>

// App-specific includes
#include "app/datalock.h"
#include "app/status_bar.h"
#include "app/today.h"
#include "app/shortcuts.h"
#include "app/quick_settings.h"
#include "app/notifications.h"
#include "app/voice_assistant.h"
#include "app/fitness.h"
#include "app/time_service.h"
#include "app/persistence.h"
#include "app/persistence_service.h"
#include "app/oobe.h"
#include "app/sdk_service.h"
#include "app/today_service.h"
#include "js_engine.h"
#include "app/privacy_service.h"
#include "app/gamification_service.h"
#include "app/breathing_exercise.h"
#include "app/navigation_service.h"
#include "app/spo2_service.h"
#include "app/temperature_service.h"
#include "app/notification_service.h"
#include "app/dnd_service.h"
#include "app/routines_service.h"
#include "app/activity_service.h"
#include "app/profile_service.h"
#include "drivers/heart_rate.h"
#include "drivers/sim_heart_rate.h"
#include "app/ble_service.h"

#include "src/extra/themes/default/lv_theme_default.h"

// New includes for UI variants
#include "weno_config.h"
#include "ui/ui_hal.h"
#include "ui/wearable.h"
#include "ui/desktop.h"
#include "ui/phone.h"
#include "ui/smarthome.h"


// --- Main Loop ---
void main_loop() {
    lv_timer_handler();
}

// --- Main Entry Point ---
int main() {
    #ifndef __EMSCRIPTEN__
    volatile uint32_t *uart0_dr = (uint32_t *)0x40004000;
    *uart0_dr = 'W'; *uart0_dr = 'E'; *uart0_dr = 'N'; *uart0_dr = 'O';
    display_init(); 
    #else
    printf("Weno Fit OS starting in Web Simulator...\\n");
    #endif

    lv_init();
    lvgl_display_init();
    persistence_service_init();
    quick_settings_init();
    // ... all other service inits from the original main.c ...
    time_service_init();
    persistence_init();
    sdk_service_init();
    today_service_init();
    today_register_native_widgets();
    js_engine_init();
    privacy_service_init();
    gamification_service_init();
    notification_service_init();
    dnd_service_init();
    routines_service_init();
    activity_service_init();
    profile_service_init();
    spo2_service_init();
    temperature_service_init();
    datalock_init();
    heart_rate_init(&sim_heart_rate_driver);
    ble_service_init();


    lv_disp_t * disp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_lighten(LV_PALETTE_GREY, 3), true, &lv_font_montserrat_14);
    lv_disp_set_theme(disp, theme);

    lv_obj_t* screen = lv_scr_act();

    // --- Personality Selection ---
    ui_personality_t* current_personality;
    if (strcmp(ACRYLIC_OS_PERSONALITY, "desktop") == 0) {
        current_personality = get_desktop_personality();
    } else if (strcmp(ACRYLIC_OS_PERSONALITY, "phone") == 0) {
        current_personality = get_phone_personality();
    } else if (strcmp(ACRYLIC_OS_PERSONALITY, "smarthome") == 0) {
        current_personality = get_smarthome_personality();
    } else {
        // Default to wearable
        current_personality = get_wearable_personality();
    }
    
    current_personality->init_ui(screen);


#ifdef SIMULATOR_BUILD
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (1) {
        main_loop();
        // platform_sleep_ms(5);
    }
#endif

    return 0;
}
