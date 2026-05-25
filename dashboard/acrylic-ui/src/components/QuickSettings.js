import React, { useState } from 'react';
import './QuickSettings.css';

const QuickSettings = ({ isVisible }) => {
    const [wifiOn, setWifiOn] = useState(true);
    const [bluetoothOn, setBluetoothOn] = useState(true);
    const [dndOn, setDndOn] = useState(false);

    if (!isVisible) return null;

    return (
        <div className="quick-settings-panel">
            <div className="quick-settings-grid">
                <div 
                    className={`qs-tile ${wifiOn ? 'active' : ''}`}
                    onClick={() => setWifiOn(!wifiOn)}
                >
                    <span>Wi-Fi</span>
                </div>
                <div 
                    className={`qs-tile ${bluetoothOn ? 'active' : ''}`}
                    onClick={() => setBluetoothOn(!bluetoothOn)}
                >
                    <span>Bluetooth</span>
                </div>
                <div 
                    className={`qs-tile ${dndOn ? 'active' : ''}`}
                    onClick={() => setDndOn(!dndOn)}
                >
                    <span>Do Not Disturb</span>
                </div>
                {/* Add more tiles here */}
            </div>
        </div>
    );
};

export default QuickSettings;
