import React, { useState } from 'react';
import './PhoneQuickSettings.css';

const PhoneQuickSettings = ({ isVisible, onClose }) => {
    const [wifiOn, setWifiOn] = useState(true);
    const [bluetoothOn, setBluetoothOn] = useState(true);
    const [dndOn, setDndOn] = useState(false);

    let className = 'phone-quick-settings';
    if (isVisible) {
        className += ' visible';
    }

    return (
        <div className={className} onClick={onClose}>
            <div className="phone-qs-panel">
                <div className="phone-qs-grid">
                    <div 
                        className={`phone-qs-tile ${wifiOn ? 'active' : ''}`}
                        onClick={(e) => { e.stopPropagation(); setWifiOn(!wifiOn); }}
                    >
                        <span>Wi-Fi</span>
                    </div>
                    <div 
                        className={`phone-qs-tile ${bluetoothOn ? 'active' : ''}`}
                        onClick={(e) => { e.stopPropagation(); setBluetoothOn(!bluetoothOn); }}
                    >
                        <span>Bluetooth</span>
                    </div>
                    <div 
                        className={`phone-qs-tile ${dndOn ? 'active' : ''}`}
                        onClick={(e) => { e.stopPropagation(); setDndOn(!dndOn); }}
                    >
                        <span>Do Not Disturb</span>
                    </div>
                    {/* Add more tiles here */}
                </div>
            </div>
        </div>
    );
};

export default PhoneQuickSettings;
