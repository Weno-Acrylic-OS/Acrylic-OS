import React, { useState, useEffect } from 'react';
import { getNotifications, clearAll, subscribe } from './NotificationManager';
import './NotificationPanel.css';

const NotificationPanel = ({ isVisible, onClose }) => {
    const [notifications, setNotifications] = useState(getNotifications());

    useEffect(() => {
        const unsubscribe = subscribe(() => {
            setNotifications(getNotifications());
        });
        return unsubscribe;
    }, []);

    if (!isVisible) return null;

    return (
        <div className="notification-panel-overlay" onClick={onClose}>
            <div className="notification-panel" onClick={e => e.stopPropagation()}>
                <div className="notification-panel-header">
                    <h2>Notifications</h2>
                    <button onClick={clearAll}>Clear All</button>
                </div>
                <div className="notification-list">
                    {notifications.length === 0 ? (
                        <p className="no-notifications">No new notifications</p>
                    ) : (
                        notifications.map(n => (
                            <div key={n.id} className={`notification-item ${n.read ? 'read' : ''}`}>
                                <h3>{n.title}</h3>
                                <p>{n.message}</p>
                                <span className="timestamp">{n.timestamp.toLocaleTimeString()}</span>
                            </div>
                        ))
                    )}
                </div>
            </div>
        </div>
    );
};

export default NotificationPanel;
