import React, { useState, useEffect } from 'react';
import './Settings.css';
import TextInput from '../components/common/TextInput';

const Settings = ({ onPinChange, pin: currentPin }) => {
    const [isDndActive, setIsDndActive] = useState(false);
    const [oldPin, setOldPin] = useState('');
    const [newPin, setNewPin] = useState('');
    const [confirmPin, setConfirmPin] = useState('');
    const [pinMessage, setPinMessage] = useState({ text: '', type: '' });

    // State for OTA Update feature
    const [updateCheckStatus, setUpdateCheckStatus] = useState(''); // e.g., 'checking', 'error', 'updated', 'available'
    const [latestVersionInfo, setLatestVersionInfo] = useState(null);
    const currentOSVersion = '21.7.0'; // This would ideally come from the OS

    const handleCheckForUpdate = async () => {
        setUpdateCheckStatus('checking');
        setLatestVersionInfo(null);
        try {
            const response = await fetch('http://localhost:3001/api/os/latest');
            if (!response.ok) throw new Error('Could not connect to update server.');
            const data = await response.json();
            setLatestVersionInfo(data);
            if (data.version === currentOSVersion) {
                setUpdateCheckStatus('updated');
            } else {
                setUpdateCheckStatus('available');
            }
        } catch (error) {
            setUpdateCheckStatus('error');
            console.error('Update check failed:', error);
        }
    };


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

    const UpdateModal = () => {
        if (!updateCheckStatus || updateCheckStatus === 'checking') return null;

        const closeModal = () => {
            // Don't close if installing
            if (updateCheckStatus === 'installing') return;
            setUpdateCheckStatus('');
        }

        const handleInstallUpdate = async () => {
            setUpdateCheckStatus('installing');
            try {
                // In a real app, the bundle URL would come from the version info
                const fakeBundleUrl = 'http://localhost:3001/updates/acrylic-os-v1.1.0.raucb';
                const response = await fetch('http://localhost:5001/api/ota/install', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ url: fakeBundleUrl }),
                });
                if (!response.ok) throw new Error('Failed to start update');
                // The UI doesn't get a "success" here. The device will reboot on its own
                // if the update succeeds. We just show a message.
            } catch (err) {
                console.error('Install trigger failed', err);
                setUpdateCheckStatus('error'); // Show an error in the modal
            }
        };
        
        let title = '';
        let content = '';

        switch(updateCheckStatus) {
            case 'updated':
                title = 'Up to Date';
                content = `Acrylic OS version ${currentOSVersion} is the latest version.`;
                break;
            case 'available':
                title = `Version ${latestVersionInfo.version} Available`;
                content = (
                    <div>
                        <p>A new version of Acrylic OS is available.</p>
                        <strong>Release Notes:</strong>
                        <pre>{latestVersionInfo.notes}</pre>
                    </div>
                );
                break;
            case 'installing':
                title = 'Installation in Progress';
                content = 'The new version is being installed in the background. The device will reboot automatically when it is complete.';
                break;
            case 'error':
                title = 'Error';
                content = 'Could not check for or start the update. Please check your connection and try again.';
                break;
            default:
                return null;
        }

        return (
            <div className="update-modal-overlay">
                <div className="update-modal">
                    <h3>{title}</h3>
                    <div className="update-modal-content">{content}</div>
                    <div className="update-modal-actions">
                        {updateCheckStatus === 'available' && (
                            <button onClick={handleInstallUpdate} className="install-btn">Install Now</button>
                        )}
                        {updateCheckStatus !== 'installing' && (
                            <button onClick={closeModal}>Close</button>
                        )}
                    </div>
                </div>
            </div>
        );
    };

    return (
        <div className="settings-app">
            <UpdateModal />
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
                        <TextInput type="password" placeholder="Old PIN" value={oldPin} onChange={e => setOldPin(e.target.value)} maxLength="4" keyboardLayout="numeric" />
                        <TextInput type="password" placeholder="New PIN" value={newPin} onChange={e => setNewPin(e.target.value)} maxLength="4" keyboardLayout="numeric" />
                        <TextInput type="password" placeholder="Confirm New PIN" value={confirmPin} onChange={e => setConfirmPin(e.target.value)} maxLength="4" keyboardLayout="numeric" />
                        <button onClick={handlePinSave}>Save PIN</button>
                        {pinMessage.text && <p className={`pin-message ${pinMessage.type}`}>{pinMessage.text}</p>}
                    </div>
                </div>
                <div className="settings-item-column">
                    <span>System Update</span>
                    <div className="update-info">
                        <p>Current Version: {currentOSVersion}</p>
                        <button onClick={handleCheckForUpdate} disabled={updateCheckStatus === 'checking'}>
                            {updateCheckStatus === 'checking' ? 'Checking...' : 'Check for Updates'}
                        </button>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default Settings;
