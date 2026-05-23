import React, { useState, useEffect } from 'react';
import './Settings.css';

// Create a single channel instance for the app
const channel = new BroadcastChannel('weno-fit-os');

const Settings = () => {
    const [isDndActive, setIsDndActive] = useState(false);

    // Get initial state and listen for external changes
    useEffect(() => {
        const handleMessage = (event) => {
            if (event.data && event.data.type === 'dnd_update') {
                console.log('DND status updated via BroadcastChannel:', event.data.status);
                setIsDndActive(event.data.status);
            }
        };
        channel.addEventListener('message', handleMessage);

        // Request initial state
        console.log('Requesting initial DND status...');
        channel.postMessage({ command: 'get_dnd_status' });

        return () => {
            channel.removeEventListener('message', handleMessage);
            // Do not close the channel, it's shared
        };
    }, []);

    const handleToggle = () => {
        // Post a command to toggle the state in the C backend.
        // The backend will then broadcast the 'dnd_update' message.
        console.log('Requesting DND toggle...');
        channel.postMessage({ command: 'toggle_dnd_status' });
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
