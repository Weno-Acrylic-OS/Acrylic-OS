import React, { useState } from 'react';
import { useKeyboard } from './KeyboardContext';
import './Keyboard.css';

const Keyboard = () => {
    const { sendKeyPress, isKeyboardVisible, layout } = useKeyboard();
    const [isShifted, setIsShifted] = useState(false);

    if (!isKeyboardVisible) {
        return null;
    }

    const handleKeyPress = (key) => {
        if (key === '⇧') {
            setIsShifted(!isShifted);
        } else {
            sendKeyPress(isShifted ? key.toUpperCase() : key);
            if (isShifted) {
                setIsShifted(false);
            }
        }
    };

    const renderQwerty = () => {
        const rows = [
            ['q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'],
            ['a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'],
            ['⇧', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '⌫'],
            ['Space']
        ];
        return rows.map((row, rowIndex) => (
            <div key={rowIndex} className="keyboard-row">
                {row.map((key) => {
                    const keyClass = `keyboard-key ${key.length > 1 ? 'keyboard-key' : ''} ${key === 'Space' ? 'space' : ''}`;
                    return (
                        <button key={key} className={keyClass} onClick={() => handleKeyPress(key)}>
                            {isShifted ? key.toUpperCase() : key}
                        </button>
                    );
                })}
            </div>
        ));
    };

    const renderNumeric = () => {
        const rows = [
            ['1', '2', '3'],
            ['4', '5', '6'],
            ['7', '8', '9'],
            ['', '0', '⌫']
        ];
        return rows.map((row, rowIndex) => (
            <div key={rowIndex} className="keyboard-row numeric">
                {row.map((key) => {
                    if (key === '') return <div key="placeholder" className="keyboard-key placeholder"></div>;
                    const keyClass = `keyboard-key ${key.length > 1 ? 'special' : ''}`;
                    return (
                        <button key={key} className={keyClass} onClick={() => sendKeyPress(key)}>
                            {key}
                        </button>
                    );
                })}
            </div>
        ));
    };

    return (
        <div className="keyboard-container" onMouseDown={(e) => e.preventDefault()}>
            {layout === 'numeric' ? renderNumeric() : renderQwerty()}
        </div>
    );
};

export default Keyboard;
