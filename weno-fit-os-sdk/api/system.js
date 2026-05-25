/**
 * @module System
 * @description Provides access to system-level information and utilities.
 */

/**
 * Returns information about the Acrylic OS device.
 * @returns {object} An object containing system information.
 * @property {string} osVersion - The version of Acrylic OS.
 * @property {string} deviceModel - The model of the device.
 */
function getInfo() {
    // To be implemented by the native side
    return WenoFitOS.system.getInfo();
}

/**
 * Returns the current UNIX timestamp.
 * @returns {number} The number of milliseconds since the UNIX epoch.
 */
function now() {
    return Date.now();
}

/**
 * Logs a message to the system console.
 * @param {string} message - The message to log.
 */
function log(message) {
    WenoFitOS.log(message);
}

export { getInfo, now, log };
