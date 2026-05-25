/**
 * @module UI
 * @description Provides a unified API for creating user interfaces across all Acrylic OS devices.
 */

/**
 * Creates a UI element.
 * @param {string} type - The type of element to create (e.g., 'label', 'button', 'container').
 * @param {object} properties - The properties of the element.
 * @property {string} [properties.text] - The text content of the element.
 * @property {function} [properties.onClick] - The function to call when the element is clicked.
 * @property {Array<object>} [properties.children] - An array of child elements.
 * @returns {object} A representation of the UI element.
 */
function createElement(type, properties) {
    // To be implemented by the native side
    return WenoFitOS.ui.createElement(type, properties);
}

/**
 * Renders a UI element to the screen.
 * @param {object} element - The element to render, created by `createElement`.
 */
function render(element) {
    // To be implemented by the native side
    WenoFitOS.ui.render(element);
}

/**
 * Shows a platform-specific notification.
 * @param {string} title - The title of the notification.
 * @param {string} message - The body of the notification.
 */
function showNotification(title, message) {
    // To be implemented by the native side
    WenoFitOS.ui.showNotification(title, message);
}

export { createElement, render, showNotification };
