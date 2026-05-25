import React, { useState, useEffect } from 'react';
import './Settings.css';

const Settings = ({ onPinChange, pin: currentPin }) => {
    const [isDndActive, setIsDndActive] = useState(false);
    const [oldPin, setOldPin] = useState('');
    const [newPin, setNewPin] = useState('');
    const [confirmPin, setConfirmPin] = useState('');
    const [pinMessage, setPinMessage] = useState({ text: '', type: '' });

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

    const handlePinSave = () => {
        if (oldPin !== currentPin) {
            setPinMessage({ text: 'Incorrect old PIN.', type: 'error' });
            return;
        }
        if (newPin.length !== 4) {
            setPinMessage({ text: 'New PIN must be 4 digits.', type: 'error' });
            return;
        }
        if (newPin !== confirmPin) {
            setPinMessage({ text: 'New PINs do not match.', type: 'error' });
            return;
        }

        onPinChange(newPin);
        setPinMessage({ text: 'PIN updated successfully!', type: 'success' });
        setOldPin('');
        setNewPin('');
        setConfirmPin('');
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
                <div className="settings-item-column">
                    <span>Change PIN</span>
                    <div className="pin-change-form">
                        <input type="password" placeholder="Old PIN" value={oldPin} onChange={e => setOldPin(e.target.value)} maxLength="4" />
                        <input type="password" placeholder="New PIN" value={newPin} onChange={e => setNewPin(e.target.value)} maxLength="4" />
                        <input type="password" placeholder="Confirm New PIN" value={confirmPin} onChange={e => setConfirmPin(e.target.value)} maxLength="4" />
                        <button onClick={handlePinSave}>Save PIN</button>
                        {pinMessage.text && <p className={`pin-message ${pinMessage.type}`}>{pinMessage.text}</p>}
                    </div>
                </div>
            </div>
        </div>
    );
};

export default Settings;
