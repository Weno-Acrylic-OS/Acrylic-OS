import React, { useState, useEffect } from 'react';
import { getNotifications, clearAll, subscribe } from '../NotificationManager';
import './PhoneQuickSettings.css';

const PhoneQuickSettings = ({ isVisible, onClose }) => {
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

    let className = 'phone-quick-settings';
    if (isVisible) {
        className += ' visible';
    }

    return (
        <div className={className} onClick={onClose}>
            <div className="phone-qs-panel" onClick={e => e.stopPropagation()}>
                <div className="phone-qs-grid">
                    <div 
                        className={`phone-qs-tile ${wifiOn ? 'active' : ''}`}
                        onClick={() => setWifiOn(!wifiOn)}
                    >
                        <span>Wi-Fi</span>
                    </div>
                    <div 
                        className={`phone-qs-tile ${bluetoothOn ? 'active' : ''}`}
                        onClick={() => setBluetoothOn(!bluetoothOn)}
                    >
                        <span>Bluetooth</span>
                    </div>
                    <div 
                        className={`phone-qs-tile ${dndOn ? 'active' : ''}`}
                        onClick={() => setDndOn(!dndOn)}
                    >
                        <span>Do Not Disturb</span>
                    </div>
                    {/* Add more tiles here */}
                </div>
                <div className="phone-notification-list">
                    <div className="phone-notification-header">
                        <h3>Notifications</h3>
                        <button onClick={clearAll}>Clear All</button>
                    </div>
                    {notifications.length === 0 ? (
                        <p className="no-notifications">No new notifications</p>
                    ) : (
                        notifications.map(n => (
                            <div key={n.id} className={`phone-notification-item ${n.read ? 'read' : ''}`}>
                                <h4>{n.title}</h4>
                                <p>{n.message}</p>
                            </div>
                        ))
                    )}
                </div>
            </div>
        </div>
    );
};

export default PhoneQuickSettings;
