import React, { useState } from 'react';
import './SmartHome.css';
import { useDrag } from '@use-gesture/react';
import CarouselWidget from './CarouselWidget';
import SmartHomeDock from './SmartHomeDock';
import AppGrid from '../phone/AppGrid'; // Re-use the phone's AppGrid

const SmartHome = () => {
    const [showAppDrawer, setShowAppDrawer] = useState(false);

    const homeGestureBind = useDrag(({ down, movement: [, my] }) => {
        if (down && my < -20) setShowAppDrawer(true); // Swipe up to open
        if (down && my > 20) setShowAppDrawer(false); // Swipe down to close
    }, { axis: 'y', filterTaps: true });

    return (
        <div className="smarthome-screen" {...homeGestureBind()}>
            <div className="smarthome-header">
                <h1>My Home</h1>
                <p>Welcome back</p>
            </div>
            
            <CarouselWidget />
            
            <div className={`smarthome-app-drawer ${showAppDrawer ? 'visible' : ''}`}>
                 <div className="drawer-handle-area" onClick={() => setShowAppDrawer(false)}>
                    <div className="drawer-handle"></div>
                </div>
                <AppGrid onAppClick={(appName) => console.log(`Open ${appName}`)} />
            </div>

            <SmartHomeDock />
        </div>
    );
};

export default SmartHome;
