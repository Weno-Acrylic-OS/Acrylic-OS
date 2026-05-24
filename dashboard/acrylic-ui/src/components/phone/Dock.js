import React from 'react';
import './Dock.css';

// Using the same AppIcon component structure as AppGrid for consistency
const AppIcon = ({ name, onAppClick }) => (
    <div className="dock-app-icon" onClick={() => onAppClick(name)}>
        <img src={`../../dashboard/${name.replace(/ /g, '')}.png`} alt={name} />
    </div>
);

const Dock = ({ onAppClick }) => {
    const dockApps = ['Browser', 'Messages', 'Mail', 'Photos'];

    return (
        <div className="dock">
            <div className="dock-inner">
                {dockApps.map(appName => (
                    <AppIcon key={appName} name={appName} onAppClick={onAppClick} />
                ))}
            </div>
        </div>
    );
};

export default Dock;
