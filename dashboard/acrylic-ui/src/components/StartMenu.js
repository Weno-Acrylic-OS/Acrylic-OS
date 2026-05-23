import React from 'react';
import './StartMenu.css';

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
                <ul>
                    <li onClick={() => handleAppClick('Browser')}>Browser</li>
                    <li onClick={() => handleAppClick('Files')}>Files</li>
                    <li onClick={() => handleAppClick('Weno Store')}>Weno Store</li>
                    <li onClick={() => handleAppClick('Settings')}>Settings</li>
                    <li onClick={() => handleAppClick('Calculator')}>Calculator</li>
                    <li className="start-menu-divider"></li>
                    <li>Shut Down...</li>
                </ul>
            </div>
        </div>
    );
};

export default StartMenu;
