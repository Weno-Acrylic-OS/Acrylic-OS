import React from 'react';
import './SmartHomeAppGrid.css';

const SmartHomeAppGrid = ({ onAppClick, appList = [] }) => {
    return (
        <div className="smarthome-app-grid">
            {appList.map(appName => (
                <div key={appName} className="smarthome-app-icon" onClick={() => onAppClick(appName)}>
                    <img src={`/dashboard/${appName.toLowerCase().replace(/ /g, '')}sm.png`} alt={appName} />
                    <span>{appName}</span>
                </div>
            ))}
        </div>
    );
};

export default SmartHomeAppGrid;
