import React, { useState, useEffect } from 'react';
import './TopBar.css';
import QuickSettings from './QuickSettings';
import AppMenu from './AppMenu';
import { Grid } from 'react-feather';

const TopBar = ({ onActivitiesClick, onLock, onToggleNotifications, onAlyssaClick, onDashboardClick }) => {
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
                    <button className="status-button" onClick={onAlyssaClick}>
                        <img src="/dashboard/Alyssad.svg" alt="Alyssa" />
                    </button>
                    <button className="status-button" onClick={onDashboardClick}>
                        <Grid size={16} />
                    </button>
                    <button className="status-button" onClick={toggleQuickSettings}>
                        <img src="/dashboard/Wifid.svg" alt="Wi-Fi" />
                        <img src="/dashboard/Batteryd.svg" alt="Battery" />
                    </button>
                    <span className="time" onClick={onToggleNotifications}>{formatTime(currentTime)}</span>
                    <button className="activities-button" onClick={onLock}>Lock</button>
                </div>
            </div>
            {showAppMenu && <AppMenu items={fileMenuItems} />}
            <QuickSettings isVisible={showQuickSettings} />
        </>
    );
};
export default TopBar;
