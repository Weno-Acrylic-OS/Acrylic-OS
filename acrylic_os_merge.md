# Plan: Merging Weno Fit OS into Acrylic OS

This plan outlines the steps to evolve the existing Weno Fit OS into a multi-purpose RTOS called Acrylic OS, capable of running on different device types with different user interfaces.

## 1. Introduce a UI Hardware Abstraction Layer (HAL)

The first step is to decouple the core OS from the specific UI implementation. We will create a "UI HAL" that defines a common interface for different UI "personalities".

*   **Create `include/ui/ui_hal.h`:** This header will define a struct `ui_personality_t` that contains function pointers for initializing the UI, handling events, etc.
    ```c
    typedef struct {
        void (*init_ui)(lv_obj_t* screen);
        // Add more functions as needed, e.g., for handling specific events
    } ui_personality_t;
    ```
*   **Modify `src/app/main.c`:** The `main` function will be modified to:
    *   Include `ui_hal.h`.
    *   Have a pointer to the current `ui_personality_t`.
    *   Call the `init_ui` function from the current personality instead of directly calling `create_main_ui`.

## 2. Define UI Personalities

We will create two initial personalities: "Wearable" (the existing Weno Fit OS UI) and a new "Desktop" personality for larger screens.

### 2.1. Refactor the "Wearable" Personality

*   **Create `src/ui/wearable.c` and `include/ui/wearable.h`:**
    *   Move the `create_main_ui` function from `src/app/main.c` to `src/ui/wearable.c`.
    *   In `wearable.c`, define a `ui_personality_t` struct for the wearable personality, and point its `init_ui` to the `create_main_ui` function.
    *   Create a `get_wearable_personality()` function that returns a pointer to this struct.

### 2.2. Create a new "Desktop" Personality

*   **Create `src/ui/desktop.c` and `include/ui/desktop.h`:**
    *   Create a new `create_desktop_ui` function that will create a simple UI for larger screens. For now, it can just be a simple label that says "Acrylic OS - Desktop Mode".
    *   In `desktop.c`, define a `ui_personality_t` struct for the desktop personality and point its `init_ui` to `create_desktop_ui`.
    *   Create a `get_desktop_personality()` function that returns a pointer to this struct.

## 3. Select the UI Personality at Boot

We need a way to select which personality to use when the OS boots. We will use a compile-time flag in `weno_config.h` for this.

*   **Modify `include/weno_config.h`:** Add a new define `ACRYLIC_OS_PERSONALITY`.
    ```c
    // Valid values: "wearable", "desktop"
    #define ACRYLIC_OS_PERSONALITY "wearable"
    ```
*   **Modify `src/app/main.c`:** In the `main` function, use `if/else` statements on `ACRYLIC_OS_PERSONALITY` to select the correct personality.
    ```c
    #include "ui/wearable.h"
    #include "ui/desktop.h"

    // ... in main() ...
    ui_personality_t* current_personality;
    if (strcmp(ACRYLIC_OS_PERSONALITY, "desktop") == 0) {
        current_personality = get_desktop_personality();
    } else {
        // Default to wearable
        current_personality = get_wearable_personality();
    }

    // ... later, instead of create_main_ui(screen) ...
    current_personality->init_ui(screen);
    ```

## 4. Update Build System

*   **Modify `Makefile.arm` and `Makefile.web`** to include the new files (`src/ui/wearable.c`, `src/ui/desktop.c`) in the build.

This plan provides a clear path to evolving Weno Fit OS into Acrylic OS. It starts with a clean architectural separation of the UI, which will make it much easier to add new device types and experiences in the future.
