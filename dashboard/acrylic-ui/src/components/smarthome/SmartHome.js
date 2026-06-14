import React, { useState, useEffect } from 'react';
import './SmartHome.css';
import { useDrag } from '@use-gesture/react';
import CarouselWidget from './CarouselWidget';
import SmartHomeDock from './SmartHomeDock';
import SmartHomeAppGrid from './SmartHomeAppGrid';
import SmartHomeStatusBar from './SmartHomeStatusBar';
import NativeAppWrapper from '../NativeAppWrapper';

// App Components
import Calculator from '../../apps/Calculator';
import Browser from '../../apps/Browser';
import Files from '../../apps/Files';
import WenoStore from '../../apps/WenoStore';
import Settings from '../../apps/Settings';
import Messages from '../../apps/Messages';
import Mail from '../../apps/Mail';
import Photos from '../../apps/Photos';
import Home from '../../apps/Home';
import { getNotifications, subscribe } from '../NotificationManager';
import Notification from '../Notification';
import Music from '../../apps/Music';
import { useInstalledApps } from '../../hooks/useInstalledApps';

const SmartHome = ({ onLock, onPinChange, pin, appUITree, onWallpaperChange, onAccentColorChange }) => {
    const [showAppDrawer, setShowAppDrawer] = useState(false);
    const [openedApp, setOpenedApp] = useState(null);
    const [appViewPos, setAppViewPos] = useState({ x: 0 });
    const [activeToast, setActiveToast] = useState(null);

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
        'Home': Home,
        'Music': Music,
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
        return unsubscribe;
    }, []);

    // Define core apps that should always be present for the Smart Home
    const coreApps = [
        'Home', 'Music', 'Settings', 'Weno Store',
        'Browser', 'Calculator', 'Files', 'Mail', 'Messages', 'Phone', 'Photos',
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
    
    const appList = allAppNames;

    const renderApp = (appName) => {
        const appRenderer = appMap[appName];
        return appRenderer ? appRenderer() : <p>{appName} Application not found.</p>;
    }

    const homeGestureBind = useDrag(({ down, movement: [, my] }) => {
        if (openedApp) return;
        if (down && my < -20) setShowAppDrawer(true); // Swipe up to open
        if (down && my > 20) setShowAppDrawer(false); // Swipe down to close
    }, { axis: 'y', filterTaps: true });

    const appGestureBind = useDrag(({ down, movement: [mx], velocity: [vx], direction: [dx] }) => {
        if (!openedApp) return;

        const x = down ? Math.max(0, mx) : 0; // Only allow dragging to the right
        setAppViewPos({ x });

        if (!down) { // On release
            const screenWidth = window.innerWidth;
            if (mx > screenWidth / 3 && dx > 0 && vx > 0.2) {
                handleCloseApp();
            } else {
                setAppViewPos({ x: 0 }); // Snap back
            }
        }
    }, { axis: 'x', filterTaps: true, preventDefault: true });
    
    const handleAppLaunch = (appName) => {
        if (appMap[appName]) {
            setOpenedApp(appName);
            setShowAppDrawer(false);
            setAppViewPos({ x: 0 });
        } else {
            console.warn(`App component for "${appName}" not found.`);
        }
    };

    const handleCloseApp = () => {
        setOpenedApp(null);
        setAppViewPos({ x: 0 });
    };

    if (openedApp) {
        return (
            <div 
                className="smarthome-app-view"
                style={{ transform: `translateX(${appViewPos.x}px)` }}
            >
                <div {...appGestureBind()} className="drag-to-close-handle-area">
                    <div className="drag-to-close-handle" />
                </div>
                <div className="smarthome-app-content">
                    {renderApp(openedApp)}
                </div>
            </div>
        );
    }

    return (
        <div className="smarthome-screen" {...homeGestureBind()}>
            <SmartHomeStatusBar onLock={onLock} />
            <div className="smarthome-header">
                <h1>My Home</h1>
                <p>Welcome back</p>
            </div>
            
            <CarouselWidget />
            
            <div className={`smarthome-app-drawer ${showAppDrawer ? 'visible' : ''}`}>
                 <div className="drawer-handle-area" onClick={() => setShowAppDrawer(false)}>
                    <div className="drawer-handle"></div>
                </div>
                <SmartHomeAppGrid onAppClick={handleAppLaunch} appList={appList} />
            </div>

            <SmartHomeDock onAppClick={handleAppLaunch} appList={appList} />
            {activeToast && <Notification notification={activeToast} onDismiss={() => setActiveToast(null)} />}
        </div>
    );
};

export default SmartHome;
