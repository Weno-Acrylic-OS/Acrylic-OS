import React, { useState, useEffect } from 'react';
import './SmartHomeStatusBar.css';

const SmartHomeStatusBar = () => {
    const [currentTime, setCurrentTime] = useState(new Date());

    useEffect(() => {
        const timerId = setInterval(() => setCurrentTime(new Date()), 1000);
        return () => clearInterval(timerId);
    }, []);

    const formatTime = (date) => {
        return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    };

    return (
        <div className="smarthome-status-bar">
            <span className="time">{formatTime(currentTime)}</span>
            <div className="status-icons">
                <img src="/dashboard/Wifi.svg" alt="Wi-Fi" />
                <img src="/dashboard/Battery.svg" alt="Battery" />
            </div>
        </div>
    );
};

export default SmartHomeStatusBar;
