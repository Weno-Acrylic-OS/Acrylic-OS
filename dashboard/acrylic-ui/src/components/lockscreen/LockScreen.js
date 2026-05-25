import React, { useState, useEffect } from 'react';
import './LockScreen.css';

const LockScreen = ({ onUnlock }) => {
    const [pin, setPin] = useState('');
    const [currentTime, setCurrentTime] = useState(new Date());

    useEffect(() => {
        const timerId = setInterval(() => setCurrentTime(new Date()), 1000);
        return () => clearInterval(timerId);
    }, []);

    useEffect(() => {
        if (pin.length === 4) {
            onUnlock(pin);
            setPin(''); // Reset after attempt
        }
    }, [pin, onUnlock]);

    const handlePinClick = (num) => {
        if (pin.length < 4) {
            setPin(pin + num);
        }
    };

    const handleDelete = () => {
        setPin(pin.slice(0, -1));
    };

    const formatTime = (date) => {
        return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    };

    const formatDate = (date) => {
        return date.toLocaleDateString([], { weekday: 'long', month: 'long', day: 'numeric' });
    };

    return (
        <div className="lockscreen">
            <div className="lockscreen-time-date">
                <div className="lockscreen-time">{formatTime(currentTime)}</div>
                <div className="lockscreen-date">{formatDate(currentTime)}</div>
            </div>

            <div className="pin-input-area">
                <div className="pin-dots">
                    {[...Array(4)].map((_, i) => (
                        <div key={i} className={`pin-dot ${i < pin.length ? 'filled' : ''}`}></div>
                    ))}
                </div>
            </div>

            <div className="numpad">
                <div className="numpad-row">
                    {[1, 2, 3].map(num => <button key={num} onClick={() => handlePinClick(num)}>{num}</button>)}
                </div>
                <div className="numpad-row">
                    {[4, 5, 6].map(num => <button key={num} onClick={() => handlePinClick(num)}>{num}</button>)}
                </div>
                <div className="numpad-row">
                    {[7, 8, 9].map(num => <button key={num} onClick={() => handlePinClick(num)}>{num}</button>)}
                </div>
                <div className="numpad-row">
                    <button style={{ visibility: 'hidden' }}></button> {/* Placeholder */}
                    <button onClick={() => handlePinClick(0)}>0</button>
                    <button onClick={handleDelete}>&lt;</button>
                </div>
            </div>
        </div>
    );
};

export default LockScreen;
