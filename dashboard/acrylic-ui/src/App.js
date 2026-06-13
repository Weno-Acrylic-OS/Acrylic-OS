import React, { useState, useEffect, useCallback } from 'react';
import './App.css';
import Desktop from './components/Desktop';
import Phone from './components/phone/Phone';
import SmartHome from './components/smarthome/SmartHome';
import Car from './components/Car';
import LockScreen from './components/lockscreen/LockScreen';
import OOBE from './components/oobe/OOBE';

import { KeyboardProvider } from './components/common/KeyboardContext';
import Keyboard from './components/common/Keyboard';

function App() {
  const [personality, setPersonality] = useState('desktop');
  const [isLocked, setIsLocked] = useState(true);
  const [showPinScreen, setShowPinScreen] = useState(false); // Control the PIN overlay
  const [pin, setPin] = useState(localStorage.getItem('acrylic-os-pin') || '0000');
  const [appUITree, setAppUITree] = useState(null);
  const [showOOBE, setShowOOBE] = useState(!localStorage.getItem('oobe_completed'));

  const handleLock = useCallback(() => {
      setIsLocked(true);
      if (personality !== 'phone') {
        setShowPinScreen(true);
      }
  }, [personality]);

  useEffect(() => {
    // Check for personality from URL parameter on initial load
    const urlParams = new URLSearchParams(window.location.search);
    const personalityFromUrl = urlParams.get('personality');
    if (personalityFromUrl) {
        setPersonality(personalityFromUrl);
    }

    const handleMessage = (event) => {
      if (!event.data) return;
      // Allow personality to be changed dynamically later if needed
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
      console.log("App.js: Received request to show PIN screen. Setting state.");
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
        return <Desktop onLock={handleLock} onPinChange={handlePinChange} pin={pin} appUITree={appUITree} />;
      case 'phone':
        return <Phone isLocked={isLocked} onLock={handleLock} requestPinScreen={handlePinScreenRequest} onPinChange={handlePinChange} pin={pin} appUITree={appUITree} />;
      case 'smarthome':
        return <SmartHome onLock={handleLock} onPinChange={handlePinChange} pin={pin} appUITree={appUITree} />;
      case 'car':
        return <Car />;
      default:
        return <div>Unknown Personality</div>;
    }
  };

  const renderContent = () => {
    if (showOOBE) {
      return <OOBE onComplete={handleOOBEComplete} onPinChange={handlePinChange} />;
    }
    
    // For non-phone personalities, if locked, show the PIN screen and nothing else.
    if (personality !== 'phone' && isLocked) {
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
    <KeyboardProvider>
      <div className="App">
        {renderContent()}
        {personality !== 'desktop' && <Keyboard />}
      </div>
    </KeyboardProvider>
  );
}

export default App;
