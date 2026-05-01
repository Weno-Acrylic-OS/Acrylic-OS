# Weno Fit OS SDK - API Reference

This document outlines the conceptual APIs available to Weno Fit OS applications developed using JavaScript and the Elk embedded engine. These APIs provide a bridge between the JavaScript application logic and the underlying C-based operating system and hardware.

## Global Objects and Modules

Applications will have access to a global `WenoFitOS` object and various modules for specific functionalities.

---

### `WenoFitOS` Object - System APIs

Provides access to core operating system functionalities.

#### Methods:

*   **`WenoFitOS.log(message: string)`**
    *   Logs a message to the system's debug output.
*   **`WenoFitOS.exitApp()`**
    *   Gracefully terminates the current application.
*   **`WenoFitOS.getBatteryLevel(): number`**
    *   Returns the current battery level as a percentage (0-100).
*   **`WenoFitOS.getOSVersion(): string`**
    *   Returns the current version of the Weno Fit OS.
*   **`WenoFitOS.on(eventName: string, callback: Function)`**
    *   Registers a callback for system-level events (e.g., `'lowBattery'`, `'displayOff'`).

---

### `LVGL` Module - UI APIs

Provides bindings to the LVGL (Light and Versatile Graphics Library) for creating rich user interfaces. All LVGL object creation and manipulation should be done through this module.

#### Methods:

*   **`LVGL.createScreen(): object`**
    *   Creates and returns a new LVGL screen object.
*   **`LVGL.createLabel(parent: object): object`**
    *   Creates and returns an LVGL label object.
*   **`LVGL.setLabelText(label: object, text: string)`**
    *   Sets the text content of an LVGL label.
*   **`LVGL.createButton(parent: object): object`**
    *   Creates and returns an LVGL button object.
*   **`LVGL.setObjectSize(obj: object, width: number, height: number)`**
    *   Sets the size of an LVGL object (can use `LVGL.pct(percentage)` for relative sizing).
*   **`LVGL.alignObject(obj: object, alignment: number, x_ofs: number, y_ofs: number)`**
    *   Aligns an LVGL object (e.g., `LVGL.ALIGN_CENTER`).
*   **`LVGL.onEvent(obj: object, eventType: number, callback: Function)`**
    *   Registers an event handler for an LVGL object (e.g., `LVGL.EVENT_CLICKED`).

#### Constants:

*   **`LVGL.ALIGN_CENTER`**, **`LVGL.ALIGN_TOP_LEFT`**, etc.: Alignment constants.
*   **`LVGL.EVENT_CLICKED`**, **`LVGL.EVENT_VALUE_CHANGED`**, etc.: Event type constants.
*   **`LVGL.pct(percentage: number): number`**: Helper function for percentage sizing.

---

### `Sensors` Module - Sensor APIs

Provides access to various hardware sensors on the Weno Fit OS device.

#### Methods:

*   **`Sensors.getHeartRate(): Promise<number>`**
    *   Returns the current heart rate. Requires `access_heart_rate_sensor` permission.
*   **`Sensors.onHeartRateChange(callback: (hr: number) => void)`**
    *   Registers a callback to be called when the heart rate changes. Requires `access_heart_rate_sensor` permission.
*   **`Sensors.getStepCount(): Promise<number>`**
    *   Returns the total step count. Requires `access_step_sensor` permission.
*   **`Sensors.onStepCountChange(callback: (steps: number) => void)`**
    *   Registers a callback for step count changes. Requires `access_step_sensor` permission.
*   **`Sensors.getLocation(): Promise<{latitude: number, longitude: number}>`**
    *   Returns the current GPS location. Requires `access_location` permission.

---

### `Notifications` Module - Notification APIs

Allows applications to send and manage system notifications.

#### Methods:

*   **`Notifications.send(title: string, message: string, options?: object)`**
    *   Sends a new notification to the system. `options` might include `icon`, `vibrate`, etc. Requires `send_notifications` permission.
*   **`Notifications.clear(notificationId: string)`**
    *   Clears a specific notification.
*   **`Notifications.on(eventName: string, callback: Function)`**
    *   Registers callbacks for notification-related events (e.g., `'clicked'`, `'dismissed'`).
