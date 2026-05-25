import React, { useState } from 'react';
import './SmartHome.css';
import { useDrag } from '@use-gesture/react';
import CarouselWidget from './CarouselWidget';
import SmartHomeDock from './SmartHomeDock';
import SmartHomeAppGrid from './SmartHomeAppGrid';
import SmartHomeStatusBar from './SmartHomeStatusBar';

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
import NativeAppWrapper from '../NativeAppWrapper';
import Music from '../../apps/Music';

const SmartHome = ({ onLock, onPinChange, pin, appUITree }) => {
    const [showAppDrawer, setShowAppDrawer] = useState(false);
    const [openedApp, setOpenedApp] = useState(null);
    const [appViewPos, setAppViewPos] = useState({ x: 0 });

    const appMap = {
        'Calculator': () => <Calculator />,
        'Browser': () => <Browser />,
        'Files': () => <Files />,
        'Weno Store': () => <WenoStore />,
        'Settings': () => <Settings onPinChange={onPinChange} pin={pin} />,
        'Messages': () => <Messages />,
        'Mail': () => <Mail />,
        'Photos': () => <Photos />,
        'Home': () => <Home />,
        'Music': () => <Music />,
        'HelloAcrylic': () => <NativeAppWrapper uiTree={appUITree} />,
    };

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
                <SmartHomeAppGrid onAppClick={handleAppLaunch} />
            </div>

            <SmartHomeDock onAppClick={handleAppLaunch} />
        </div>
    );
};

export default SmartHome;
