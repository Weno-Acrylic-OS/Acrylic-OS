import React, { useState, useEffect } from 'react';
import './SmartHomeStatusBar.css';
import QuickSettings from '../QuickSettings'; // Re-using the desktop one

const SmartHomeStatusBar = ({ onLock }) => {
    const [currentTime, setCurrentTime] = useState(new Date());
    const [showQuickSettings, setShowQuickSettings] = useState(false);

    useEffect(() => {
        const timerId = setInterval(() => setCurrentTime(new Date()), 1000);
        return () => clearInterval(timerId);
    }, []);

    const formatTime = (date) => {
        return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    };

    const toggleQuickSettings = () => {
        setShowQuickSettings(!showQuickSettings);
    }

    return (
        <>
            <div className="smarthome-status-bar">
                <span className="time">{formatTime(currentTime)}</span>
                <div className="status-icons">
                    <button className="status-bar-button" onClick={toggleQuickSettings}>
                        <img src="/dashboard/Wifi.svg" alt="Wi-Fi" />
                        <img src="/dashboard/Battery.svg" alt="Battery" />
                    </button>
                    <button className="status-bar-button" onClick={onLock}>
                        <svg width="20" height="20" viewBox="0 0 24 24"><path fill="white" d="M12,17c1.1,0,2-0.9,2-2s-0.9-2-2-2s-2,0.9-2,2S10.9,17,12,17z M18,8h-1V6c0-2.76-2.24-5-5-5S7,3.24,7,6v2H6c-1.1,0-2,0.9-2,2v10c0,1.1,0.9,2,2,2h12c1.1,0,2-0.9,2-2V10C20,8.9,19.1,8,18,8z M9,6c0-1.66,1.34-3,3-3s3,1.34,3,3v2H9V6z"/></svg>
                    </button>
                </div>
            </div>
            <QuickSettings isVisible={showQuickSettings} />
        </>
    );
};

export default SmartHomeStatusBar;
