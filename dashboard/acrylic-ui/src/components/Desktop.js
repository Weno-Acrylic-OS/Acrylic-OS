import React, { useState, useEffect } from 'react';
import './Desktop.css';
import Taskbar from './Taskbar';
import TopBar from './TopBar';
import Window from './Window';
import ActivitiesView from './ActivitiesView';
import { addNotification, getNotifications, subscribe } from './NotificationManager';
import NotificationPanel from './NotificationPanel';
import Notification from './Notification';
// App Components
import Calculator from '../apps/Calculator';
import Browser from '../apps/Browser';
import Files from '../apps/Files';
import WenoStore from '../apps/WenoStore';
import Settings from '../apps/Settings';
import Messages from '../apps/Messages';
import Mail from '../apps/Mail';
import NativeAppWrapper from './NativeAppWrapper';
import Photos from '../apps/Photos';

const Desktop = ({ onLock, onPinChange, pin, appUITree }) => {
    const [windows, setWindows] = useState([]);
    const [showActivities, setShowActivities] = useState(false);
    const [topZIndex, setTopZIndex] = useState(100);
    const [showNotificationPanel, setShowNotificationPanel] = useState(false);
    const [activeToast, setActiveToast] = useState(null);

    useEffect(() => {
        const unsubscribe = subscribe(() => {
            const newNotifications = getNotifications();
            if (newNotifications.length > 0) {
                // For simplicity, just show the latest as a toast
                setActiveToast(newNotifications[0]);
            }
        });

        // Demo notification
        setTimeout(() => addNotification('Welcome!', 'This is a test notification.'), 2000);

        return unsubscribe;
    }, []);

    const appMap = {
        'Calculator': () => <Calculator />,
        'Browser': () => <Browser />,
        'Files': () => <Files />,
        'Weno Store': () => <WenoStore />,
        'Settings': () => <Settings onPinChange={onPinChange} pin={pin} />,
        'Messages': () => <Messages />,
        'Mail': () => <Mail />,
        'Photos': () => <Photos />,
        'HelloAcrylic': () => <NativeAppWrapper uiTree={appUITree} />,
    };

    const renderApp = (appName) => {
        const appRenderer = appMap[appName];
        return appRenderer ? appRenderer() : <p>{appName} Application not found.</p>;
    }

    const openWindow = (appName) => {
        const existingWindow = windows.find(w => w.title === appName);
        if (existingWindow) {
            if (existingWindow.isMinimized) {
                toggleMinimize(existingWindow.id);
            } else {
                focusWindow(existingWindow.id);
            }
            return;
        }

        const newPosition = {
            x: 150 + windows.length * 20,
            y: 60 + windows.length * 20,
        };
        const newZIndex = topZIndex + 1;

        const newWindow = {
            id: Date.now(),
            title: appName,
            app: renderApp(appName),
            position: newPosition,
            zIndex: newZIndex,
            isMinimized: false,
        };
        setTopZIndex(newZIndex);
        setWindows(prev => [...prev, newWindow]);
    };

    const closeWindow = (id) => {
        setWindows(windows.filter(w => w.id !== id));
    };

    const focusWindow = (id) => {
        const newZIndex = topZIndex + 1;
        setTopZIndex(newZIndex);
        setWindows(windows.map(win => 
            win.id === id ? { ...win, zIndex: newZIndex, isMinimized: false } : win
        ));
    };

    const minimizeWindow = (id) => {
        setWindows(windows.map(win => 
            win.id === id ? { ...win, isMinimized: true } : win
        ));
    };

    const toggleMinimize = (id) => {
        const win = windows.find(w => w.id === id);
        if (!win) return;

        if (win.isMinimized) {
            focusWindow(id); // This will also un-minimize and bring to front
        } else {
            minimizeWindow(id);
        }
    };

    const toggleActivitiesView = () => {
        setShowActivities(!showActivities);
    };

    const toggleNotificationPanel = () => {
        setShowNotificationPanel(!showNotificationPanel);
    }

    const switchWindow = (id) => {
        focusWindow(id);
        setShowActivities(false);
    }

    return (
        <div className="desktop">
            <TopBar onActivitiesClick={toggleActivitiesView} onLock={onLock} onToggleNotifications={toggleNotificationPanel} />

            {windows.filter(w => !w.isMinimized).map(win => (
                <Window 
                    key={win.id} 
                    title={win.title} 
                    onClose={() => closeWindow(win.id)}
                    onMinimize={() => minimizeWindow(win.id)}
                    onFocus={() => focusWindow(win.id)}
                    initialPosition={win.position}
                    zIndex={win.zIndex}
                >
                    {win.app}
                </Window>
            ))}

            {showActivities && (
                <ActivitiesView 
                    windows={windows} 
                    onCloseWindow={closeWindow}
                    onSwitchWindow={switchWindow}
                />
            )}

            <NotificationPanel isVisible={showNotificationPanel} onClose={() => setShowNotificationPanel(false)} />
            {activeToast && <Notification notification={activeToast} onDismiss={() => setActiveToast(null)} />}

            <Taskbar onAppClick={openWindow} windows={windows} onTaskbarAppClick={toggleMinimize} />
        </div>
    );
};

export default Desktop;
