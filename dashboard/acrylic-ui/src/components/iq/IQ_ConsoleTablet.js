import React, { useState } from 'react';
import './IQ.css';
import IQ_HomeGrid from './IQ_HomeGrid';
import IQ_MusicWidget from './IQ_MusicWidget';
import { Wind, Wifi, Home, Grid } from 'react-feather';

// Import all separated app components
import { MediaProvider } from './apps/MediaContext';
import IQ_MediaApp from './apps/IQ_MediaApp';
import IQ_NavigationApp from './apps/IQ_NavigationApp';
import IQ_PhoneApp from './apps/IQ_PhoneApp';
import IQ_SettingsApp from './apps/IQ_SettingsApp';
import IQ_ClimateApp from './apps/IQ_ClimateApp';


// The app registry is now a clean, direct map.
const appRegistry = {
    'Navigation': IQ_NavigationApp,
    'Media': IQ_MediaApp,
    'Phone': IQ_PhoneApp,
    'Settings': IQ_SettingsApp,
    'Climate': IQ_ClimateApp,
};

const IQ_ConsoleTablet = () => {
    const [activeView, setActiveView] = useState('Home'); 

    const renderMainContent = () => {
        if (activeView === 'Home') {
            return <IQ_HomeGrid onAppClick={setActiveView} />;
        }
        const ActiveAppComponent = appRegistry[activeView];
        return ActiveAppComponent ? <ActiveAppComponent /> : <IQ_HomeGrid onAppClick={setActiveView} />;
    };

    return (
        <MediaProvider>
            <div className="iq-component iq-console">
                <div className="iq-console-statusbar">
                    <div className="status-left"><Wind size={20} /> 72°</div>
                    <div className="status-right"><Wifi size={20} /> 5G</div>
                </div>

                <div className="iq-console-main-content">
                    {renderMainContent()}
                </div>

                <div className="iq-console-dock">
                    <div className="dock-nav-buttons">
                        <button className={activeView === 'Home' ? 'active' : ''} onClick={() => setActiveView('Home')}>
                            <Home size={28} />
                        </button>
                        <button><Grid size={28} /></button>
                    </div>
                    <div className="dock-music-widget-container">
                        {/* This component will get its state from the context now */}
                        <IQ_MusicWidget />
                    </div>
                </div>
            </div>
        </MediaProvider>
    );
};

export default IQ_ConsoleTablet;
