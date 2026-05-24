import React, { useState } from 'react';
import './Phone.css';
import { useDrag } from '@use-gesture/react';
import PhoneStatusBar from './PhoneStatusBar';
import AppGrid from './AppGrid';
import Dock from './Dock';
import ActivitiesView from '../ActivitiesView';

// App Imports
import Browser from '../../apps/Browser';
import Files from '../../apps/Files';
import WenoStore from '../../apps/WenoStore';
import Settings from '../../apps/Settings';
import Calculator from '../../apps/Calculator';
import Messages from '../../apps/Messages';
import Mail from '../../apps/Mail';
import Photos from '../../apps/Photos';

const appMap = {
    'Browser': <Browser />, 'Files': <Files />, 'Weno Store': <WenoStore />,
    'Settings': <Settings />, 'Calculator': <Calculator />, 'Messages': <Messages />,
    'Mail': <Mail />, 'Photos': <Photos />,
};

const Phone = () => {
    const [activeApp, setActiveApp] = useState(null);
    const [showActivities, setShowActivities] = useState(false);
    const [showAppDrawer, setShowAppDrawer] = useState(false);
    const [appViewPos, setAppViewPos] = useState({ y: 0 });

    const openApp = (appName) => {
        if (appMap[appName]) {
            setActiveApp(appName);
            setAppViewPos({ y: 0 });
            setShowAppDrawer(false); // Close drawer when app opens
        } else {
            console.warn(`App "${appName}" not found.`);
        }
    };

    const closeApp = () => setActiveApp(null);

    // Gesture for swiping up an open app
    const appGestureBind = useDrag(({ down, movement: [, my], velocity: [, vy], direction: [, dy] }) => {
        if (!activeApp) return;
        const y = down ? Math.min(my, 0) : 0;
        if (down) setAppViewPos({ y });
        else {
            const screenHeight = window.innerHeight;
            if (my < -screenHeight / 2 || (vy > 0.5 && dy < 0)) closeApp();
            else if (my < -screenHeight / 4) {
                setShowActivities(true);
                setAppViewPos({ y: -screenHeight + 100 });
            } else setAppViewPos({ y: 0 });
        }
    }, { axis: 'y', filterTaps: true, preventDefault: true });

    // Gesture for opening the app drawer from the home screen
    const homeGestureBind = useDrag(({ down, movement: [, my] }) => {
        if (activeApp) return;
        if (down && my < 0) setShowAppDrawer(true);
        if (down && my > 0) setShowAppDrawer(false);
    }, { axis: 'y', filterTaps: true });

    const renderAppView = () => {
        if (!activeApp) return null;
        return (
            <div 
                {...appGestureBind()} 
                className="app-view-wrapper" 
                style={{ transform: `translateY(${appViewPos.y}px)` }}
            >
                <div className="gesture-handle-area">
                    <div className="gesture-handle"></div>
                </div>
                {appMap[activeApp]}
            </div>
        );
    };

    return (
        <div className="phone-screen" {...homeGestureBind()}>
            <PhoneStatusBar />

            {/* App Drawer */}
            <div className={`app-drawer ${showAppDrawer ? 'visible' : ''}`}>
                <div className="drawer-handle-area" onClick={() => setShowAppDrawer(false)}>
                    <div className="drawer-handle"></div>
                </div>
                <AppGrid onAppClick={openApp} />
            </div>

            {/* Active App View */}
            {renderAppView()}
            
            {/* Dock */}
            {!activeApp && <Dock onAppClick={openApp} />}
            
            {/* Activities Overlay */}
            {showActivities && 
                <ActivitiesView 
                    windows={activeApp ? [{id: 1, title: activeApp}] : []} 
                    onCloseWindow={() => { setShowActivities(false); closeApp(); }}
                    onSwitchWindow={() => { setShowActivities(false); setAppViewPos({ y: 0 }); }}
                />
            }
        </div>
    );
};

export default Phone;
