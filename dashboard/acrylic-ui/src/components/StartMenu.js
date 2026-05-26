import React from 'react';
import './StartMenu.css';

const apps = [
    { name: 'Browser', icon: '/dashboard/Browser.png' },
    { name: 'Calculator', icon: '/dashboard/Calculator.png' },
    { name: 'Files', icon: '/dashboard/Files.png' },
    { name: 'Mail', icon: '/dashboard/Mail.png' },
    { name: 'Messages', icon: '/dashboard/Messages.png' },
    { name: 'Photos', icon: '/dashboard/Photos.png' },
    { name: 'Settings', icon: '/dashboard/Settings.png' },
    { name: 'Weno Store', icon: '/dashboard/Wenostore.png' },
    { name: 'HelloAcrylic', icon: '/dashboard/logo192.png' }, // Using a generic icon for now
];

const StartMenu = ({ onAppClick }) => {
    // A simple guard
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
                    {apps.map(app => (
                        <div key={app.name} className="start-menu-app-icon" onClick={() => handleAppClick(app.name)}>
                            <img src={`/${app.icon}`} alt={app.name} />
                            <span>{app.name}</span>
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
