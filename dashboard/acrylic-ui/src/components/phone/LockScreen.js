import React, { useState, useEffect } from 'react';
import './LockScreen.css';
import { Lock } from 'react-feather';

const LockScreen = ({ onUnlockRequest }) => {
    const [time, setTime] = useState(new Date());

    useEffect(() => {
        const timerId = setInterval(() => {
            setTime(new Date());
        }, 1000);
        return () => clearInterval(timerId);
    }, []);

    const formatTime = (date) => {
        const hours = date.getHours().toString().padStart(2, '0');
        const minutes = date.getMinutes().toString().padStart(2, '0');
        return `${hours}:${minutes}`;
    };

    const formatDate = (date) => {
        return date.toLocaleDateString(undefined, {
            weekday: 'long',
            month: 'long',
            day: 'numeric',
        });
    };

    return (
        <div className="phone-lock-screen">
            <div className="lock-screen-time-date">
                <div className="lock-screen-time">{formatTime(time)}</div>
                <div className="lock-screen-date">{formatDate(time)}</div>
            </div>
            <div className="lock-screen-unlock-prompt">
                <p>Tap to unlock</p>
                <div className="unlock-icon-container" onClick={onUnlockRequest}>
                    <Lock size={24} color="#204876" />
                </div>
            </div>
        </div>
    );
};

export default LockScreen;
