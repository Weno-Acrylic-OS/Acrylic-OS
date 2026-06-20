// dashboard/acrylic-ui/src/components/iq/apps/IQ_PhoneApp.js
import React, { useState } from 'react';
import '../IQ.css';
import { Star, Clock, Phone, Delete } from 'react-feather';

const mockFavorites = [
    { name: 'Home', photo: 'https://i.pravatar.cc/100?u=home' },
    { name: 'Jane Doe', photo: 'https://i.pravatar.cc/100?u=jane' },
    { name: 'John Smith', photo: 'https://i.pravatar.cc/100?u=john' },
];

const mockRecents = [
    { name: 'Jane Doe', type: 'incoming', time: '10:32 AM' },
    { name: 'Unknown', type: 'missed', time: '9:15 AM' },
    { name: 'Work', type: 'outgoing', time: 'Yesterday' },
];

const FavoritesView = () => (
    <div className="phone-view favorites-view">
        {mockFavorites.map(fav => (
            <button key={fav.name} className="favorite-contact">
                <img src={fav.photo} alt={fav.name} />
                <span>{fav.name}</span>
            </button>
        ))}
    </div>
);

const RecentsView = () => (
    <div className="phone-view recents-view">
        {mockRecents.map((call, i) => (
            <div key={i} className="recent-call-item">
                <div className="call-info">
                    <p className="caller-name">{call.name}</p>
                    <p className="call-time">{call.time}</p>
                </div>
                <p className={`call-type ${call.type}`}>{call.type}</p>
            </div>
        ))}
    </div>
);

const DialerView = () => {
    const [number, setNumber] = useState('');
    const dial = (digit) => setNumber(prev => prev + digit);
    const clear = () => setNumber(prev => prev.slice(0, -1));

    const dialpad = ['1','2','3','4','5','6','7','8','9','*','0','#'];

    return (
        <div className="phone-view dialer-view">
            <div className="dialer-display">{number || 'Enter number'}</div>
            <div className="dialpad">
                {dialpad.map(d => <button key={d} onClick={() => dial(d)}>{d}</button>)}
            </div>
            <div className="dialer-actions">
                <button className="call-btn"><Phone size={32} /></button>
                {number.length > 0 && <button className="clear-btn" onClick={clear}><Delete size={32} /></button>}
            </div>
        </div>
    );
};

const IQ_PhoneApp = () => {
    const [activeTab, setActiveTab] = useState('Favorites');

    const renderTabContent = () => {
        switch (activeTab) {
            case 'Recents': return <RecentsView />;
            case 'Dialer': return <DialerView />;
            case 'Favorites':
            default:
                return <FavoritesView />;
        }
    };

    return (
        <div className="iq-app iq-phone-app">
            <div className="phone-app-tabs">
                <button className={activeTab === 'Favorites' ? 'active' : ''} onClick={() => setActiveTab('Favorites')}><Star /> Favorites</button>
                <button className={activeTab === 'Recents' ? 'active' : ''} onClick={() => setActiveTab('Recents')}><Clock /> Recents</button>
                <button className={activeTab === 'Dialer' ? 'active' : ''} onClick={() => setActiveTab('Dialer')}><Phone /> Dialer</button>
            </div>
            <div className="phone-app-content">
                {renderTabContent()}
            </div>
        </div>
    );
};

export default IQ_PhoneApp;
