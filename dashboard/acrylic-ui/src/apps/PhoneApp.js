import React, { useState } from 'react';
import './PhoneApp.css';
import Keypad from './PhoneApp/Keypad';
import Recents from './PhoneApp/Recents';
import Contacts from './PhoneApp/Contacts';
import { Clock, User, Phone } from 'react-feather';

const PhoneApp = () => {
    const [activeTab, setActiveTab] = useState('keypad');
    const [number, setNumber] = useState('');
    const [callStatus, setCallStatus] = useState({ active: false, contact: '' });

    const handleCall = (contactName) => {
        setCallStatus({ active: true, contact: contactName || number || 'Unknown' });
        // Simulate call ending after 3 seconds
        setTimeout(() => {
            setCallStatus({ active: false, contact: '' });
            setNumber('');
        }, 3000);
    };

    const renderActiveTab = () => {
        switch (activeTab) {
            case 'recents':
                return <Recents onCall={handleCall} />;
            case 'contacts':
                return <Contacts onCall={handleCall} />;
            case 'keypad':
            default:
                return <Keypad number={number} setNumber={setNumber} onCall={handleCall} />;
        }
    };

    if (callStatus.active) {
        return (
            <div className="phone-app in-call-view">
                <h2>Calling...</h2>
                <h1>{callStatus.contact}</h1>
                <button className="end-call-button" onClick={() => setCallStatus({active: false, contact: ''})}>End Call</button>
            </div>
        )
    }

    return (
        <div className="phone-app">
            <div className="phone-app-header">
                <h1>Phone</h1>
            </div>
            <div className="phone-app-content">
                {renderActiveTab()}
            </div>
            <div className="phone-app-tabs">
                <button className={activeTab === 'recents' ? 'active' : ''} onClick={() => setActiveTab('recents')}>
                    <Clock size={24} /><p>Recents</p>
                </button>
                <button className={activeTab === 'contacts' ? 'active' : ''} onClick={() => setActiveTab('contacts')}>
                    <User size={24} /><p>Contacts</p>
                </button>
                <button className={activeTab === 'keypad' ? 'active' : ''} onClick={() => setActiveTab('keypad')}>
                    <div className="keypad-icon">#</div><p>Keypad</p>
                </button>
            </div>
        </div>
    );
};

export default PhoneApp;
