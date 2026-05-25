import React, { useState, useEffect } from 'react';
import './App.css';
import Desktop from './components/Desktop';
import Phone from './components/phone/Phone';
import SmartHome from './components/smarthome/SmartHome';
import LockScreen from './components/lockscreen/LockScreen';

function App() {
  const [personality, setPersonality] = useState('desktop');
  const [isLocked, setIsLocked] = useState(true); // Start locked
  const [pin, setPin] = useState(localStorage.getItem('acrylic-os-pin') || '0000');

  useEffect(() => {
    const handleMessage = (event) => {
      // For security, you might check event.origin here if the parent origin is known
      if (!event.data) return;

      if (event.data.personality) {
        console.log(`App.js: Received personality -> ${event.data.personality}`);
        setPersonality(event.data.personality);
      }
      if (event.data.lock) {
          setIsLocked(true);
      }
    };

    window.addEventListener('message', handleMessage);

    // Notify the parent frame that the React app is ready to receive messages
    console.log('App.js: React app is ready, sending "ready" status.');
    window.parent.postMessage({ status: 'ready' }, '*'); // Send to any parent origin

    return () => {
      window.removeEventListener('message', handleMessage);
    };
  }, []);

  const handleUnlock = (enteredPin) => {
      if (enteredPin === pin) {
          setIsLocked(false);
      } else {
          console.error('Incorrect PIN');
      }
  };

  const handleLock = () => {
      setIsLocked(true);
  }

  const handlePinChange = (newPin) => {
      setPin(newPin);
      localStorage.setItem('acrylic-os-pin', newPin);
      // Maybe show a confirmation message here
  };

  const renderPersonality = () => {
    switch (personality) {
      case 'desktop':
        return <Desktop onLock={handleLock} onPinChange={handlePinChange} pin={pin} />;
      case 'phone':
        return <Phone onLock={handleLock} onPinChange={handlePinChange} pin={pin} />;
      case 'smarthome':
        return <SmartHome onLock={handleLock} onPinChange={handlePinChange} pin={pin} />;
      default:
        return <div>Unknown Personality</div>;
    }
  };

  return (
    <div className="App">
        {isLocked ? <LockScreen onUnlock={handleUnlock} /> : renderPersonality()}
    </div>
  );
}

export default App;
