import React, { useState, useEffect } from 'react';
import { getNotifications, clearAll, subscribe } from './NotificationManager';
import './QuickSettings.css';

const QuickSettings = ({ isVisible }) => {
    const [wifiOn, setWifiOn] = useState(true);
    const [bluetoothOn, setBluetoothOn] = useState(true);
    const [dndOn, setDndOn] = useState(false);
    const [notifications, setNotifications] = useState(getNotifications());

    useEffect(() => {
        const unsubscribe = subscribe(() => {
            setNotifications(getNotifications());
        });
        return unsubscribe;
    }, []);

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
            <div className="qs-notification-list">
                <div className="qs-notification-header">
                    <h3>Notifications</h3>
                    <button onClick={clearAll}>Clear All</button>
                </div>
                {notifications.length === 0 ? (
                    <p className="no-notifications">No new notifications</p>
                ) : (
                    notifications.map(n => (
                        <div key={n.id} className={`qs-notification-item ${n.read ? 'read' : ''}`}>
                            <h4>{n.title}</h4>
                            <p>{n.message}</p>
                        </div>
                    ))
                )}
            </div>
        </div>
    );
};

export default QuickSettings;
