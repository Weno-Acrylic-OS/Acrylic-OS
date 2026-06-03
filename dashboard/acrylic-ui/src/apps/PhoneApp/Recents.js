import React from 'react';
import { PhoneIncoming, PhoneOutgoing, PhoneMissed } from 'react-feather';
import '../PhoneApp.css';

const mockRecents = [
    { id: 1, name: 'Jane Doe', type: 'incoming', time: 'Yesterday' },
    { id: 2, name: 'John Smith', type: 'missed', time: 'Yesterday' },
    { id: 3, name: 'Acme Corp', type: 'outgoing', time: '2 days ago' },
    { id: 4, name: 'Jane Doe', type: 'incoming', time: '3 days ago' },
];

const Recents = ({ onCall }) => {
    const renderIcon = (type) => {
        switch(type) {
            case 'incoming':
                return <PhoneIncoming size={16} />;
            case 'outgoing':
                return <PhoneOutgoing size={16} />;
            case 'missed':
                return <PhoneMissed size={16} color="red" />;
            default:
                return null;
        }
    };

    return (
        <div className="recents-container">
            {mockRecents.map(call => (
                <div key={call.id} className="recent-item" onClick={() => onCall(call.name)}>
                    <div className="recent-icon">{renderIcon(call.type)}</div>
                    <div className="recent-info">
                        <p className={`recent-name ${call.type === 'missed' ? 'missed' : ''}`}>{call.name}</p>
                        <p className="recent-type">{call.type}</p>
                    </div>
                    <div className="recent-time">{call.time}</div>
                </div>
            ))}
        </div>
    );
};

export default Recents;
