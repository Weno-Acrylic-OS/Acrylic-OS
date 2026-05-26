/**
 * @module NotificationManager
 * @description A simple in-memory manager for notifications.
 */

let notifications = [];
let nextId = 0;
let listeners = [];

function subscribe(listener) {
    listeners.push(listener);
    return function unsubscribe() {
        listeners = listeners.filter(l => l !== listener);
    };
}

function notify() {
    listeners.forEach(listener => listener());
}

function addNotification(title, message) {
    const newNotification = {
        id: nextId++,
        title,
        message,
        timestamp: new Date(),
        read: false,
    };
    notifications = [newNotification, ...notifications];
    notify();
    return newNotification;
}

function getNotifications() {
    return notifications;
}

function markAsRead(id) {
    notifications = notifications.map(n => 
        n.id === id ? { ...n, read: true } : n
    );
    notify();
}

function clearAll() {
    notifications = [];
    notify();
}

export { addNotification, getNotifications, markAsRead, clearAll, subscribe };
