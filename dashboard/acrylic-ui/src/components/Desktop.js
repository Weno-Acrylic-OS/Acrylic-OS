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
import AlyssaApps from '../apps/AlyssaApps';
import { useInstalledApps } from '../hooks/useInstalledApps';
import Alyssa from './common/Alyssa';
import DashboardView from './desktop/DashboardView';

const Desktop = ({ onLock, onPinChange, pin, appUITree, onWallpaperChange, onAccentColorChange }) => {
    const [windows, setWindows] = useState([]);
    const [showActivities, setShowActivities] = useState(false);
    const [topZIndex, setTopZIndex] = useState(100);
    const [showNotificationPanel, setShowNotificationPanel] = useState(false);
    const [activeToast, setActiveToast] = useState(null);
    const [showAlyssa, setShowAlyssa] = useState(false);
    const [showDashboard, setShowDashboard] = useState(false);

    // Create a registry of all possible app components
    const ALL_APPS_REGISTRY = {
        'Calculator': Calculator,
        'Browser': Browser,
        'Files': Files,
        'Weno Store': WenoStore,
        'Settings': Settings,
        'Messages': Messages,
        'Mail': Mail,
        'Photos': Photos,
        'Alyssa Apps': AlyssaApps,
        'HelloAcrylic': NativeAppWrapper,
    };

    // Fetch the list of installed apps
    const { apps: installedApps } = useInstalledApps();

    useEffect(() => {
        const unsubscribe = subscribe(() => {
            const newNotifications = getNotifications();
            if (newNotifications.length > 0) {
                setActiveToast(newNotifications[0]);
            }
        });
        setTimeout(() => addNotification('Welcome!', 'This is a test notification.'), 2000);
        return unsubscribe;
    }, []);

    // Define core apps that should always be present
    const coreApps = [
        'Settings', 'Browser', 'Files', 'Weno Store',
        'Photos', 'Messages', 'Mail', 'Calculator', 'Alyssa Apps',
    ];

    // Combine core apps with installed apps, removing duplicates
    const allAppNames = [...new Set([...coreApps, ...installedApps.map(app => app.name)])];

    // Dynamically generate the map of apps that can be rendered
    const appMap = allAppNames.reduce((acc, appName) => {
        const AppComponent = ALL_APPS_REGISTRY[appName];
        if (AppComponent) {
            if (appName === 'Settings') {
                acc[appName] = () => <AppComponent onPinChange={onPinChange} pin={pin} onWallpaperChange={onWallpaperChange} onAccentColorChange={onAccentColorChange} />;
            } else if (appName === 'HelloAcrylic') {
                acc[appName] = () => <AppComponent uiTree={appUITree} />;
            } else {
                acc[appName] = () => <AppComponent />;
            }
        }
        return acc;
    }, {});

    const appListForStartMenu = allAppNames;

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

    const toggleAlyssa = () => {
        setShowAlyssa(!showAlyssa);
    }

    const toggleDashboard = () => {
        setShowDashboard(!showDashboard);
    }

    const switchWindow = (id) => {
        focusWindow(id);
        setShowActivities(false);
    }

    return (
        <div className="desktop">
            <Alyssa isVisible={showAlyssa} onClose={toggleAlyssa} />
            <DashboardView isVisible={showDashboard} onClose={toggleDashboard} />
            <TopBar onActivitiesClick={toggleActivitiesView} onLock={onLock} onToggleNotifications={toggleNotificationPanel} onAlyssaClick={toggleAlyssa} onDashboardClick={toggleDashboard} />

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

            <Taskbar onAppClick={openWindow} windows={windows} onTaskbarAppClick={toggleMinimize} appList={appListForStartMenu} />
        </div>
    );
};

export default Desktop;
