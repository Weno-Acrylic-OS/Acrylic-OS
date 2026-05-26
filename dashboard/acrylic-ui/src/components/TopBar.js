import React, { useState, useEffect } from 'react';
import './TopBar.css';
import QuickSettings from './QuickSettings';
import AppMenu from './AppMenu';

const TopBar = ({ onActivitiesClick, onLock, onToggleNotifications }) => {
    const [currentTime, setCurrentTime] = useState(new Date());
    const [showAppMenu, setShowAppMenu] = useState(false);
    const [showQuickSettings, setShowQuickSettings] = useState(false);

    useEffect(() => {
        const timerId = setInterval(() => setCurrentTime(new Date()), 1000);
        return () => clearInterval(timerId);
    }, []);

    const formatTime = (date) => {
        return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    };

    const toggleAppMenu = () => {
        setShowAppMenu(!showAppMenu);
    };

    const toggleQuickSettings = () => {
        setShowQuickSettings(!showQuickSettings);
    };

    const fileMenuItems = ["New", "Open", "Save", "Save As...", "Exit"];

    return (
        <>
            <div className="top-bar">
                <div className="top-bar-left">
                    <button className="app-menu-button" onClick={toggleAppMenu}>
                        <strong>App</strong>
                    </button>
                    <button className="activities-button" onClick={onActivitiesClick}>Activities</button>
                </div>
                <div className="top-bar-right">
                    <button className="status-button" onClick={onToggleNotifications}>
                        <svg width="20" height="20" viewBox="0 0 24 24"><path fill="#333" d="M12 22c1.1 0 2-.9 2-2h-4c0 1.1.9 2 2 2zm6-6v-5c0-3.07-1.63-5.64-4.5-6.32V4c0-.83-.67-1.5-1.5-1.5s-1.5.67-1.5 1.5v.68C7.63 5.36 6 7.92 6 11v5l-2 2v1h16v-1l-2-2z"/></svg>
                    </button>
                    <button className="status-button" onClick={toggleQuickSettings}>
                        <img src="/dashboard/Wifi.svg" alt="Wi-Fi" />
                        <img src="/dashboard/Battery.svg" alt="Battery" />
                    </button>
                    <span className="time">{formatTime(currentTime)}</span>
                    <button className="activities-button" onClick={onLock}>Lock</button>
                </div>
            </div>
            {showAppMenu && <AppMenu items={fileMenuItems} />}
            <QuickSettings isVisible={showQuickSettings} />
        </>
    );
};
export default TopBar;
