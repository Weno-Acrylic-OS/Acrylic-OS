import React, { useState } from 'react';
import './ControlWidget.css';

const ControlWidget = ({ icon, name, initialStatus = false }) => {
    const [isOn, setIsOn] = useState(initialStatus);

    const handleToggle = () => {
        setIsOn(!isOn);
        // In the future, this would call the bridge to the C backend
        console.log(`${name} toggled to ${!isOn}`);
    };

    return (
        <div className={`control-widget ${isOn ? 'on' : ''}`}>
            <div className="widget-icon">{icon}</div>
            <div className="widget-text">
                <span className="widget-name">{name}</span>
                <span className="widget-status">{isOn ? 'On' : 'Off'}</span>
            </div>
            <div className="widget-toggle">
                 <label className="switch">
                    <input 
                        type="checkbox" 
                        checked={isOn}
                        onChange={handleToggle}
                    />
                    <span className="slider round"></span>
                </label>
            </div>
        </div>
    );
};

export default ControlWidget;
