import React from 'react';
import './SmartHomeAppGrid.css';

const apps = [
    { name: 'Browser' },
    { name: 'Calculator' },
    { name: 'Files' },
    { name: 'Mail' },
    { name: 'Messages' },
    { name: 'Phone' },
    { name: 'Photos' },
    { name: 'Settings' },
];

const SmartHomeAppGrid = ({ onAppClick }) => {
    return (
        <div className="smarthome-app-grid">
            {apps.map(app => (
                <div key={app.name} className="smarthome-app-icon" onClick={() => onAppClick(app.name)}>
                    <img src={`/dashboard/${app.name.toLowerCase().replace(/ /g, '')}sm.png`} alt={app.name} />
                    <span>{app.name}</span>
                </div>
            ))}
        </div>
    );
};

export default SmartHomeAppGrid;
