import React, { useState, useEffect } from 'react';
import './Settings.css';

const Settings = () => {
    const [isDndActive, setIsDndActive] = useState(false);

    useEffect(() => {
        const handleMessage = (event) => {
            // Security check and ensure it's the right type of message
            if (event.origin !== 'http://localhost:8000' || !event.data || event.data.type !== 'dnd_update') {
                return;
            }
            console.log('Settings.js: DND status updated via postMessage:', event.data.status);
            setIsDndActive(event.data.status);
        };

        window.addEventListener('message', handleMessage);

        // Request initial state from the parent
        console.log('Settings.js: Requesting initial DND status...');
        window.parent.postMessage({ command: 'get_dnd_status' }, '*');

        return () => {
            window.removeEventListener('message', handleMessage);
        };
    }, []);

    const handleToggle = () => {
        console.log('Settings.js: Requesting DND toggle...');
        window.parent.postMessage({ command: 'toggle_dnd_status' }, '*');
    };

    return (
        <div className="settings-app">
            <h1>Settings</h1>
            <div className="settings-list">
                <div className="settings-item">
                    <span>Do Not Disturb</span>
                    <label className="switch">
                        <input 
                            type="checkbox" 
                            checked={isDndActive}
                            onChange={handleToggle}
                        />
                        <span className="slider round"></span>
                    </label>
                </div>
                {/* More settings will go here */}
            </div>
        </div>
    );
};

export default Settings;
