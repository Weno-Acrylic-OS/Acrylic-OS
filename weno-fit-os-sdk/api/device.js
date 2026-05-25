/**
 * @module Device
 * @description Provides access to device-specific hardware and features.
 */

/**
 * Returns the form factor of the device.
 * @returns {string} The form factor: "desktop", "phone", "smarthome", or "wearable".
 */
function getFormFactor() {
    // To be implemented by the native side
    return WenoFitOS.device.getFormFactor();
}

/**
 * Returns the current heart rate. (Wearable only)
 * @returns {number|null} The current heart rate in beats per minute, or null if not available.
 */
function getHeartRate() {
    if (getFormFactor() === 'wearable') {
        // To be implemented by the native side
        return WenoFitOS.device.getHeartRate();
    }
    return null;
}

/**
 * Minimizes a window. (Desktop only)
 * @param {number} id - The ID of the window to minimize.
 */
function minimizeWindow(id) {
    if (getFormFactor() === 'desktop') {
        // To be implemented by the native side
        WenoFitOS.device.minimizeWindow(id);
    }
}

export { getFormFactor, getHeartRate, minimizeWindow };
