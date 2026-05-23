import React, { useState } from 'react';
import './Desktop.css';
import Taskbar from './Taskbar';
import TopBar from './TopBar';
import Window from './Window';
import ActivitiesView from './ActivitiesView';
// App Components
import Calculator from '../apps/Calculator';
import Browser from '../apps/Browser';
import Files from '../apps/Files';
import WenoStore from '../apps/WenoStore';
import Settings from '../apps/Settings';

const appMap = {
    'Calculator': <Calculator />,
    'Browser': <Browser />,
    'Files': <Files />,
    'Weno Store': <WenoStore />,
    'Settings': <Settings />,
};
const renderApp = (appName) => {
    return appMap[appName] || <p>{appName} Application not found.</p>;
}
const Desktop = () => {
    const [windows, setWindows] = useState([]);
    const [showActivities, setShowActivities] = useState(false);
    const [topZIndex, setTopZIndex] = useState(100); // Initial z-index

    const openWindow = (appName) => {
        if (windows.find(w => w.title === appName)) {
            // If window is already open, just focus it
            focusWindow(windows.find(w => w.title === appName).id);
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
            win.id === id ? { ...win, zIndex: newZIndex } : win
        ));
    };

    const toggleActivitiesView = () => {
        setShowActivities(!showActivities);
    };

    const switchWindow = (id) => {
        focusWindow(id);
        setShowActivities(false);
    }

    return (
        <div className="desktop">
            <TopBar onActivitiesClick={toggleActivitiesView} />

            {windows.map(win => (
                <Window 
                    key={win.id} 
                    title={win.title} 
                    onClose={() => closeWindow(win.id)}
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

            <Taskbar onAppClick={openWindow} windows={windows} onTaskbarAppClick={focusWindow} />
        </div>
    );
};

export default Desktop;
