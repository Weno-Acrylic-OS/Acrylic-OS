import React, { useState, useEffect } from 'react';
import './App.css';
import Desktop from './components/Desktop';

const PhoneUI = () => <div className="phone-ui">Phone UI</div>;
const SmartHomeUI = () => <div className="smarthome-ui">Smart Home UI</div>;

function App() {
  const [personality, setPersonality] = useState('desktop');

  useEffect(() => {
    const channel = new BroadcastChannel('weno-fit-os');
    const handleMessage = (event) => {
      if (event.data.personality) {
        setPersonality(event.data.personality);
      }
      console.log('Received data from Weno Fit OS:', event.data);
    };
    channel.addEventListener('message', handleMessage);

    return () => {
      channel.removeEventListener('message', handleMessage);
      channel.close();
    };
  }, []);

  const renderPersonality = () => {
    switch (personality) {
      case 'desktop':
        return <Desktop />;
      case 'phone':
        return <PhoneUI />;
      case 'smarthome':
        return <SmartHomeUI />;
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
