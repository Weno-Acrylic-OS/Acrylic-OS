import React from 'react';
import './SmartHomeDock.css';

const AppIcon = ({ name, icon, onClick }) => (
    <div className="smarthome-dock-icon" onClick={onClick}>
        <img src={`/dashboard/${icon}sm.png`} alt={name} />
    </div>
);

const iconMap = {
    'Home': 'home',
    'Music': 'music',
    'Weno Store': 'wenostore',
};

const SmartHomeDock = ({ onAppClick, appList = [] }) => {
    // Filter the incoming appList to only show the specific apps intended for the dock
    const dockApps = appList
        .filter(appName => ['Home', 'Music', 'Weno Store'].includes(appName))
        .map(appName => ({ name: appName, icon: iconMap[appName] }));

    return (
        <div className="smarthome-dock">
            {dockApps.map(app => (
                <AppIcon key={app.name} name={app.name} icon={app.icon} onClick={() => onAppClick(app.name)} />
            ))}
        </div>
    );
};

export default SmartHomeDock;
