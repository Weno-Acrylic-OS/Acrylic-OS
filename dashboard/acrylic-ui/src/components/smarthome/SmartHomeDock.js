import React from 'react';
import './SmartHomeDock.css';

const AppIcon = ({ name, icon, onClick }) => (
    <div className="smarthome-dock-icon" onClick={onClick}>
        <img src={`/dashboard/${icon}sm.png`} alt={name} />
    </div>
);

const SmartHomeDock = ({ onAppClick }) => {
    // The user requested Home, Music, and Weno Store
    const dockApps = [
        { name: 'Home', icon: 'home' },
        { name: 'Music', icon: 'music' },
        { name: 'Weno Store', icon: 'wenostore' },
    ];

    return (
        <div className="smarthome-dock">
            {dockApps.map(app => (
                <AppIcon key={app.name} name={app.name} icon={app.icon} onClick={() => onAppClick(app.name)} />
            ))}
        </div>
    );
};

export default SmartHomeDock;
