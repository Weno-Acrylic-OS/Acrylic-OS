import React from 'react';
import './AppGrid.css';

const apps = [
    { name: 'Messages' },
    { name: 'Mail' },
    { name: 'Photos' },
    { name: 'Browser' },
    { name: 'Files' },
    { name: 'Weno Store' },
    { name: 'Settings' },
    { name: 'Calculator' },
    { name: 'HelloAcrylic' },
];

const AppGrid = ({ onAppClick }) => {
    return (
        <div className="app-grid">
            {apps.map(app => (
                <div key={app.name} className="app-icon" onClick={() => onAppClick(app.name)}>
                    <img src={`../../dashboard/${app.name.replace(/ /g, '')}.png`} alt={app.name} />
                    <span>{app.name}</span>
                </div>
            ))}
        </div>
    );
};

export default AppGrid;
