import React from 'react';
import './SmartHomeDock.css';

const AppIcon = ({ name, icon }) => (
    <div className="smarthome-dock-icon">
        <div className="smarthome-icon-placeholder">{icon}</div>
        <span>{name}</span>
    </div>
);

const SmartHomeDock = () => {
    // The user requested Home, Music, and Weno Store
    const dockApps = [
        { name: 'Home', icon: '🏠' },
        { name: 'Music', icon: '🎵' },
        { name: 'Weno Store', icon: '🛍️' },
    ];

    return (
        <div className="smarthome-dock">
            {dockApps.map(app => (
                <AppIcon key={app.name} name={app.name} icon={app.icon} />
            ))}
        </div>
    );
};

export default SmartHomeDock;
