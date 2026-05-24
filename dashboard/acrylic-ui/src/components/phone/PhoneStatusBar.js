import React, { useState, useEffect } from 'react';
import './PhoneStatusBar.css';

const PhoneStatusBar = () => {
    const [currentTime, setCurrentTime] = useState(new Date());

    useEffect(() => {
        const timerId = setInterval(() => setCurrentTime(new Date()), 1000);
        return () => clearInterval(timerId);
    }, []);

    const formatTime = (date) => {
        return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    };

    return (
        <div className="phone-status-bar">
            <span className="time">{formatTime(currentTime)}</span>
            <div className="status-icons">
                <img src="/dashboard/wifi.svg" alt="Wi-Fi" />
                <img src="/dashboard/battery.svg" alt="Battery" />
            </div>
        </div>
    );
};

export default PhoneStatusBar;
