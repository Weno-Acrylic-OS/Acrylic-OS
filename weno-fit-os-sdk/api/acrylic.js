/**
 * @module Acrylic
 * @description The core module for building Acrylic OS apps for desktop, phone, and smarthome.
 */

let _root = null;

/**
 * Creates a UI element. This is a "virtual" element that will be translated into a native React component.
 * @param {string} type - The type of element to create (e.g., 'View', 'Text', 'Button').
 * @param {object} props - The properties of the element.
 * @param {...object} children - Child elements.
 * @returns {object} A representation of the UI element.
 */
function createElement(type, props, ...children) {
    return {
        type,
        props: {
            ...props,
            children: children.flat(),
        },
    };
}

/**
 * Renders the UI tree. This sends the virtual DOM to the Acrylic OS shell to be rendered as native React components.
 * @param {object} element - The root element of the app's UI.
 */
function render(element) {
    _root = element;
    window.parent.postMessage({
        type: 'render-app',
        ui: _root,
    }, '*');
}

export default { createElement, render };
