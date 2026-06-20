// dashboard/acrylic-ui/src/components/iq/IQ_HomeGrid.js
import React from 'react';
import './IQ.css';
import { Map, Phone, Music, Settings, Wind } from 'react-feather';

const coreApps = [
    { name: 'Navigation', icon: <Map size={48} /> },
    { name: 'Media', icon: <Music size={48} /> },
    { name: 'Phone', icon: <Phone size={48} /> },
    { name: 'Climate', icon: <Wind size={48} /> },
    { name: 'Settings', icon: <Settings size={48} /> },
];

const IQ_HomeGrid = ({ onAppClick }) => {
    return (
        <div className="iq-home-grid">
            {coreApps.map(app => (
                <button key={app.name} className="app-icon-btn" onClick={() => onAppClick(app.name)}>
                    {app.icon}
                    <span>{app.name}</span>
                </button>
            ))}
        </div>
    );
};

export default IQ_HomeGrid;
