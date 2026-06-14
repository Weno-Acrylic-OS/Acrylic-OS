import React, { useState, useEffect } from 'react';
import './Phone.css';
import { useDrag } from '@use-gesture/react';
import PhoneStatusBar from './PhoneStatusBar';
import AppGrid from './AppGrid';
import Dock from './Dock';
import PhoneQuickSettings from './PhoneQuickSettings';
import ActivitiesView from '../ActivitiesView';
import LockScreen from './LockScreen'; // This line was missing!

// App Imports
import Browser from '../../apps/Browser';
import Files from '../../apps/Files';
import WenoStore from '../../apps/WenoStore';
import Settings from '../../apps/Settings';
import Calculator from '../../apps/Calculator';
import Messages from '../../apps/Messages';
import Mail from '../../apps/Mail';
import NativeAppWrapper from '../NativeAppWrapper';
import { getNotifications, subscribe } from '../NotificationManager';
import Notification from '../Notification';
import Photos from '../../apps/Photos';
import PhoneApp from '../../apps/PhoneApp'; // Add PhoneApp import here
import CameraApp from '../../apps/Camera';
import CalendarApp from '../../apps/Calendar';
import WenoCar from '../../apps/WenoCar';
import { useInstalledApps } from '../../hooks/useInstalledApps';
import { useWidgetLayout, WIDGET_REGISTRY } from '../../hooks/WidgetLayoutContext';
import WidgetLibrary from '../common/WidgetLibrary';
import { Edit, Plus, X } from 'react-feather';

const Phone = ({ isLocked, onLock, requestPinScreen, onPinChange, pin, appUITree, onWallpaperChange, onAccentColorChange }) => {
    const [activeApp, setActiveApp] = useState(null);
    const [showActivities, setShowActivities] = useState(false);
    const [showAppDrawer, setShowAppDrawer] = useState(false);
    const [showQuickSettings, setShowQuickSettings] = useState(false);
    const [appViewPos, setAppViewPos] = useState({ y: 0 });
    const [activeToast, setActiveToast] = useState(null);
    const [isEditing, setIsEditing] = useState(false);
    const [showLibrary, setShowLibrary] = useState(false);

    // App Registry
    const ALL_APPS_REGISTRY = { 'Browser': Browser, 'Files': Files, 'Weno Store': WenoStore, 'Settings': Settings, 'Calculator': Calculator, 'Messages': Messages, 'Mail': Mail, 'Photos': Photos, 'Phone': PhoneApp, 'Camera': CameraApp, 'Calendar': CalendarApp, 'Weno Car+': WenoCar, 'HelloAcrylic': NativeAppWrapper };
    
    // Fetch installed apps
    const { apps: installedApps } = useInstalledApps();

    // Core apps for the phone
    const coreApps = ['Phone', 'Messages', 'Browser', 'Camera'];
    
    // Combine and create the final list of apps to display
    const appList = [...new Set([...coreApps, ...installedApps.map(app => app.name)])];

    useEffect(() => {
        if (isLocked) setActiveApp(null);
    }, [isLocked]);

    useEffect(() => {
        const unsubscribe = subscribe(() => {
            const newNotifications = getNotifications();
            if (newNotifications.length > 0) setActiveToast(newNotifications[0]);
        });
        return unsubscribe;
    }, []);

    // Dynamically generate the map of apps that can be rendered
    const appMap = appList.reduce((acc, appName) => {
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

    const openApp = (appName) => {
        if (isLocked) return;
        if (appMap[appName]) {
            setActiveApp(appName);
            setAppViewPos({ y: 0 });
            setShowAppDrawer(false);
        } else {
            console.warn(`App "${appName}" not found.`);
        }
    };

    const closeApp = () => setActiveApp(null);

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

    const homeGestureBind = useDrag(({ down, movement: [, my] }) => {
        if (activeApp || showQuickSettings) return;
        if (down && my < 0) setShowAppDrawer(true);
        if (down && my > 0) setShowAppDrawer(false);
    }, { axis: 'y', filterTaps: true });

    const qsGestureBind = useDrag(({ down, movement: [, my] }) => {
        if (activeApp) return;
        if (down && my > 50) {
            setShowQuickSettings(true);
        }
    }, { axis: 'y' });

    const renderAppView = () => {
        if (!activeApp) return null;
        const appRenderer = appMap[activeApp];
        return (
            <div 
                {...appGestureBind()} 
                className="app-view-wrapper" 
                style={{ transform: `translateY(${appViewPos.y}px)` }}
            >
                <div className="gesture-handle-area">
                    <div className="gesture-handle"></div>
                </div>
                {appRenderer ? appRenderer() : <div>App not found</div>}
            </div>
        );
    };

    const { getWidgetsFor, removeWidget } = useWidgetLayout();
    const phoneWidgets = getWidgetsFor('phone');

    if (isLocked) {
        return <LockScreen onUnlockRequest={requestPinScreen} />;
    }

    return (
        <>
            <WidgetLibrary 
                isVisible={showLibrary} 
                onClose={() => setShowLibrary(false)} 
                personality="phone" 
            />
            <div className="phone-screen" {...homeGestureBind()}>
                <div className="phone-power-button" onClick={onLock}></div>
                <div className="phone-edit-button" onClick={() => setIsEditing(!isEditing)}>
                    {isEditing ? 'Done' : <Edit size={20} />}
                </div>

                <div {...qsGestureBind()} style={{ zIndex: 51, position: 'relative' }}>
                    <PhoneStatusBar />
                </div>

                <div className="phone-home-widgets">
                    {phoneWidgets.map((widgetName, index) => {
                        const WidgetComponent = WIDGET_REGISTRY[widgetName];
                        if (!WidgetComponent) return null;
                        return (
                            <div key={widgetName} className="widget-container-phone">
                                {isEditing && (
                                    <button className="remove-widget-btn" onClick={() => removeWidget('phone', index)}>
                                        <X size={16} />
                                    </button>
                                )}
                                <WidgetComponent />
                            </div>
                        );
                    })}
                    {isEditing && (
                        <button className="add-widget-placeholder" onClick={() => setShowLibrary(true)}>
                            <Plus size={24} />
                        </button>
                    )}
                </div>

                <div className={`app-drawer ${showAppDrawer ? 'visible' : ''}`}>
                    <div className="drawer-handle-area" onClick={() => setShowAppDrawer(false)}>
                        <div className="drawer-handle"></div>
                    </div>
                    <AppGrid onAppClick={openApp} appList={appList} />
                </div>

                {renderAppView()}
                
                {!activeApp && <Dock onAppClick={openApp} appList={appList.slice(0, 4)} />}

                <PhoneQuickSettings isVisible={showQuickSettings} onClose={() => setShowQuickSettings(false)} />
                
                {activeToast && <Notification notification={activeToast} onDismiss={() => setActiveToast(null)} />}

                {showActivities && 
                    <ActivitiesView 
                        windows={activeApp ? [{id: 1, title: activeApp}] : []} 
                        onCloseWindow={() => { setShowActivities(false); closeApp(); }}
                        onSwitchWindow={() => { setShowActivities(false); setAppViewPos({ y: 0 }); }}
                    />
                }
            </div>
        </>
    );
};

export default Phone;
