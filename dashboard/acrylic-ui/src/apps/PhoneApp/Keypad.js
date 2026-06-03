import React from 'react';
import { Phone } from 'react-feather';
import '../PhoneApp.css';

const Keypad = ({ number, setNumber, onCall }) => {
    const handleDigitClick = (digit) => {
        setNumber(number + digit);
    };

    const handleDelete = () => {
        setNumber(number.slice(0, -1));
    };

    return (
        <div className="keypad-container">
            <div className="keypad-display">{number || <span className="placeholder">Enter number</span>}</div>
            <div className="keypad">
                <div className="numpad-row">
                    {['1', '2', '3'].map(d => <button key={d} onClick={() => handleDigitClick(d)}>{d}</button>)}
                </div>
                <div className="numpad-row">
                    {['4', '5', '6'].map(d => <button key={d} onClick={() => handleDigitClick(d)}>{d}</button>)}
                </div>
                <div className="numpad-row">
                    {['7', '8', '9'].map(d => <button key={d} onClick={() => handleDigitClick(d)}>{d}</button>)}
                </div>
                <div className="numpad-row">
                    <button onClick={() => handleDigitClick('*')}>*</button>
                    <button onClick={() => handleDigitClick('0')}>0</button>
                    <button onClick={() => handleDigitClick('#')}>#</button>
                </div>
            </div>
            <div className="keypad-actions">
                <button className="call-button" onClick={onCall}>
                    <Phone size={28} />
                </button>
                {number.length > 0 && (
                    <button className="delete-button" onClick={handleDelete}>
                        ⌫
                    </button>
                )}
            </div>
        </div>
    );
};

export default Keypad;
