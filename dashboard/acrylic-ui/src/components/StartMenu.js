import React from 'react';
import './StartMenu.css';

// A local map to find icons for the dynamic apps
const iconMap = {
    'Browser': '/dashboard/Browser.png',
    'Calculator': '/dashboard/Calculator.png',
    'Files': '/dashboard/Files.png',
    'Mail': '/dashboard/Mail.png',
    'Messages': '/dashboard/Messages.png',
    'Photos': '/dashboard/Photos.png',
    'Settings': '/dashboard/Settings.png',
    'Weno Store': '/dashboard/Wenostore.png',
    'HelloAcrylic': '/dashboard/logo192.png',
    'Phone': '/dashboard/Phone.png', // Assuming an icon exists
    'Camera': '/dashboard/Camera.png', // Assuming an icon exists
    'Calendar': '/dashboard/Calendar.png', // Assuming an icon exists
    'Weno Car+': '/dashboard/Car.png', // Assuming an icon exists
};
const defaultIcon = '/dashboard/logo192.png';

const StartMenu = ({ onAppClick, appList = [] }) => {
    const handleAppClick = (appName) => {
        if (onAppClick) {
            onAppClick(appName);
        }
    };

    return (
        <div className="start-menu">
            <div className="start-menu-side">
                {/* User profile or OS logo could go here */}
            </div>
            <div className="start-menu-main">
                <div className="start-menu-app-grid">
                    {appList.map(appName => (
                        <div key={appName} className="start-menu-app-icon" onClick={() => handleAppClick(appName)}>
                            <img src={iconMap[appName] || defaultIcon} alt={appName} />
                            <span>{appName}</span>
                        </div>
                    ))}
                </div>
                <div className="start-menu-footer">
                    <div className="start-menu-divider"></div>
                    <div className="start-menu-footer-item">Shut Down...</div>
                </div>
            </div>
        </div>
    );
};

export default StartMenu;
