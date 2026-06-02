import React, { useRef, useEffect } from 'react';
import { useKeyboard } from './KeyboardContext';

const TextInput = (props) => {
    const { showKeyboard, hideKeyboard } = useKeyboard();
    const inputRef = useRef(null);

    const handleFocus = () => {
        // Pass the requested layout, or default to 'qwerty'
        showKeyboard(inputRef.current, props.keyboardLayout || 'qwerty');
    };

    const handleBlur = () => {
        // We add a small delay to allow a click on the keyboard to register
        // before the keyboard is hidden.
        setTimeout(() => {
            if (document.activeElement !== inputRef.current) {
                hideKeyboard();
            }
        }, 100);
    };

    return (
        <input
            ref={inputRef}
            {...props}
            onFocus={handleFocus}
            onBlur={handleBlur}
        />
    );
};

export default TextInput;
