import React, { useState, useEffect } from 'react';
import './TopBar.css';
import AppMenu from './AppMenu';

const TopBar = ({ onActivitiesClick }) => {
    const [currentTime, setCurrentTime] = useState(new Date());
    const [showAppMenu, setShowAppMenu] = useState(false);

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

    const fileMenuItems = ["New", "Open", "Save", "Save As...", "Exit"];

    return (
        <>
            <div className="top-bar">
                <div className="top-bar-left">
                    <span className="time">{formatTime(currentTime)}</span>
                    {/* Status icons will go here */}
                </div>
                <div className="top-bar-center">
                    <button className="app-menu-button" onClick={toggleAppMenu}>
                        <strong>App</strong>
                    </button>
                </div>
                <div className="top-bar-right">
                    <button className="activities-button" onClick={onActivitiesClick}>Activities</button>
                </div>
            </div>
            {showAppMenu && <AppMenu items={fileMenuItems} />}
        </>
    );
};

export default TopBar;
