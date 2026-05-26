import React, { useEffect } from 'react';
import './Notification.css';

const Notification = ({ notification, onDismiss }) => {
    useEffect(() => {
        const timer = setTimeout(() => {
            onDismiss(notification.id);
        }, 5000); // Auto-dismiss after 5 seconds

        return () => clearTimeout(timer);
    }, [notification, onDismiss]);

    return (
        <div className="notification-toast" onClick={() => onDismiss(notification.id)}>
            <h3>{notification.title}</h3>
            <p>{notification.message}</p>
        </div>
    );
};

export default Notification;
