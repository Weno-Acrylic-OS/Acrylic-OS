import React, { useState, useEffect } from 'react';
import './App.css';
import Desktop from './components/Desktop';
import Phone from './components/phone/Phone';
import SmartHome from './components/smarthome/SmartHome';


function App() {
  const [personality, setPersonality] = useState('desktop');

  useEffect(() => {
    const handleMessage = (event) => {
      // For security, you might check event.origin here if the parent origin is known
      if (!event.data) return;

      if (event.data.personality) {
        console.log(`App.js: Received personality -> ${event.data.personality}`);
        setPersonality(event.data.personality);
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

  const renderPersonality = () => {
    switch (personality) {
      case 'desktop':
        return <Desktop />;
      case 'phone':
        return <Phone />;
      case 'smarthome':
        return <SmartHome />;
      default:
        return <div>Unknown Personality</div>;
    }
  };

  return (
    <div className="App">
      {renderPersonality()}
    </div>
  );
}

export default App;
