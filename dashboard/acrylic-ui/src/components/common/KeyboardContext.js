import React, { createContext, useState, useContext } from 'react';

const KeyboardContext = createContext();

export const useKeyboard = () => useContext(KeyboardContext);

export const KeyboardProvider = ({ children }) => {
    const [activeInput, setActiveInput] = useState(null);
    const [layout, setLayout] = useState('qwerty'); // Add layout state

    const showKeyboard = (inputTarget, layoutType = 'qwerty') => {
        setActiveInput(inputTarget);
        setLayout(layoutType);
    };

    const hideKeyboard = () => {
        setActiveInput(null);
        setLayout('qwerty'); // Reset layout on hide
    };

    const sendKeyPress = (key) => {
        if (!activeInput) return;

        // This is a simplified way to handle key presses.
        // It directly manipulates the value of the target input element
        // and dispatches an input event to ensure React state updates.
        const target = activeInput;
        const currentValue = target.value;
        let newValue;

        switch (key) {
            case '⌫': // Backspace
                newValue = currentValue.slice(0, -1);
                break;
            case 'Space':
                newValue = currentValue + ' ';
                break;
            default:
                newValue = currentValue + key;
        }

        // To make this work with React controlled components, we need to
        // set the property and then dispatch an event.
        Object.getOwnPropertyDescriptor(window.HTMLInputElement.prototype, 'value').set.call(target, newValue);
        target.dispatchEvent(new Event('input', { bubbles: true }));
    };

    const value = {
        isKeyboardVisible: !!activeInput,
        layout, // Expose current layout
        showKeyboard,
        hideKeyboard,
        sendKeyPress,
    };

    return (
        <KeyboardContext.Provider value={value}>
            {children}
        </KeyboardContext.Provider>
    );
};
