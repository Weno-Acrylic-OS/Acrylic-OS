import React, { useState, useEffect } from 'react';
import './TopBar.css';
import QuickSettings from './QuickSettings';
import AppMenu from './AppMenu';

const TopBar = ({ onActivitiesClick, onLock }) => {
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
                    <button className="status-button" onClick={toggleQuickSettings}>
                    <span className="time">{formatTime(currentTime)}</span>
                        <img src="/dashboard/Wifi.svg" alt="Wi-Fi" />
                        <img src="/dashboard/Battery.svg" alt="Battery" />
                    </button>
                    <button className="activities-button" onClick={onLock}>Lock</button>
                </div>
            </div>
            {showAppMenu && <AppMenu items={fileMenuItems} />}
            <QuickSettings isVisible={showQuickSettings} />
        </>
    );
};
export default TopBar;
