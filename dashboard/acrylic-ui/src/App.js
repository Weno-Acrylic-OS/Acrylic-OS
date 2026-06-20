import React, { useState, useEffect, useCallback } from 'react';
import './App.css';
import Desktop from './components/Desktop';
import Phone from './components/phone/Phone';
import SmartHome from './components/smarthome/SmartHome';
import Car from './components/Car';
import IQ_WindshieldStrip from './components/iq/IQ_WindshieldStrip';
import IQ_AR_Overlay from './components/iq/IQ_AR_Overlay';
import IQ_ConsoleTablet from './components/iq/IQ_ConsoleTablet';
import LockScreen from './components/lockscreen/LockScreen';
import OOBE from './components/oobe/OOBE';

import { KeyboardProvider } from './components/common/KeyboardContext';
import Keyboard from './components/common/Keyboard';
import { WidgetLayoutProvider } from './hooks/WidgetLayoutContext';

function App() {
  const [personality, setPersonality] = useState('desktop');
  const [display, setDisplay] = useState(null); // For IQ multi-display
  const [isLocked, setIsLocked] = useState(true);
  const [showPinScreen, setShowPinScreen] = useState(false);
  const [pin, setPin] = useState(localStorage.getItem('acrylic-os-pin') || '0000');
  const [appUITree, setAppUITree] = useState(null);
  const [showOOBE, setShowOOBE] = useState(!localStorage.getItem('oobe_completed'));
  const [selectedWallpaper, setSelectedWallpaper] = useState('url("/dashboard/wallpaper.png")');
  const [selectedAccentColor, setSelectedAccentColor] = useState('#007aff');

  const handleWallpaperChange = (newWallpaper) => { setSelectedWallpaper(newWallpaper); };
  const handleAccentColorChange = (newColor) => { setSelectedAccentColor(newColor); };

  useEffect(() => {
    document.documentElement.style.setProperty('--background-primary', selectedWallpaper);
    document.documentElement.style.setProperty('--primary-accent', selectedAccentColor);
  }, [selectedWallpaper, selectedAccentColor]);

  const handleLock = useCallback(() => {
      setIsLocked(true);
      if (personality !== 'phone') { setShowPinScreen(true); }
  }, [personality]);

  useEffect(() => {
    const urlParams = new URLSearchParams(window.location.search);
    const personalityFromUrl = urlParams.get('personality');
    if (personalityFromUrl) {
        setPersonality(personalityFromUrl);
        // If the personality is 'iq', also determine which display it is
        if (personalityFromUrl === 'iq') {
            const displayFromUrl = urlParams.get('display');
            if (displayFromUrl) {
                setDisplay(displayFromUrl);
            }
        }
    }

    const handleMessage = (event) => {
      if (!event.data) return;
      if (event.data.personality) { setPersonality(event.data.personality); }
      if (event.data.lock) { handleLock(); }
      if (event.data.type === 'render-app') { setAppUITree(event.data.ui); }
    };
    window.addEventListener('message', handleMessage);
    window.parent.postMessage({ status: 'ready' }, '*');
    return () => window.removeEventListener('message', handleMessage);
  }, [handleLock]);

  const handleUnlock = (enteredPin) => {
      if (enteredPin === pin) {
          setIsLocked(false);
          setShowPinScreen(false);
      } else {
          console.error('Incorrect PIN');
      }
  };
  
  const handlePinScreenRequest = () => {
      setShowPinScreen(true);
  };

  const handlePinChange = (newPin) => {
      setPin(newPin);
      localStorage.setItem('acrylic-os-pin', newPin);
  };

  const handleOOBEComplete = () => {
      localStorage.setItem('oobe_completed', 'true');
      setShowOOBE(false);
  };

  const renderPersonality = () => {
    switch (personality) {
      case 'desktop':
        return <Desktop onLock={handleLock} onPinChange={handlePinChange} pin={pin} appUITree={appUITree} onWallpaperChange={handleWallpaperChange} onAccentColorChange={handleAccentColorChange} />;
      case 'phone':
        return <Phone isLocked={isLocked} onLock={handleLock} requestPinScreen={handlePinScreenRequest} onPinChange={handlePinChange} pin={pin} appUITree={appUITree} onWallpaperChange={handleWallpaperChange} onAccentColorChange={handleAccentColorChange} />;
      case 'smarthome':
        return <SmartHome onLock={handleLock} onPinChange={handlePinChange} pin={pin} appUITree={appUITree} onWallpaperChange={handleWallpaperChange} onAccentColorChange={handleAccentColorChange} />;
      case 'car':
        return <Car />;
      case 'iq':
        switch(display) {
            case 'strip': return <IQ_WindshieldStrip />;
            case 'ar': return <IQ_AR_Overlay />;
            case 'console': return <IQ_ConsoleTablet />;
            default: return <div>IQ Display not specified</div>;
        }
      default:
        return <div>Unknown Personality</div>;
    }
  };

  const renderContent = () => {
    if (showOOBE) {
      return <OOBE onComplete={handleOOBEComplete} onPinChange={handlePinChange} />;
    }
    
    // For desktop/smarthome, if locked, show the PIN screen and nothing else.
    if ((personality === 'desktop' || personality === 'smarthome') && isLocked) {
      return <LockScreen onUnlock={handleUnlock} />;
    }

    // For the phone, and for unlocked non-phone UIs
    return (
      <>
        {renderPersonality()}
        {/* The PIN screen is an overlay only for the phone personality */}
        {personality === 'phone' && showPinScreen && <LockScreen onUnlock={handleUnlock} />}
      </>
    );
  };

  return (
    <WidgetLayoutProvider>
      <KeyboardProvider>
        <div className="App">
          {renderContent()}
          {personality !== 'desktop' && <Keyboard />}
        </div>
      </KeyboardProvider>
    </WidgetLayoutProvider>
  );
}

export default App;
