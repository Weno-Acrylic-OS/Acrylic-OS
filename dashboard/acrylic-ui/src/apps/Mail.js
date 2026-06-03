import React, { useState } from 'react';
import './Mail.css';

const mockEmails = {
    'Inbox': [
        { id: 1, from: 'launch@example.com', subject: 'Your Launch Confirmation', body: 'Thank you for your order. It will be shipped shortly.' },
        { id: 2, from: 'support@example.com', subject: 'Re: Support Ticket #12345', body: 'We have received your request and a support agent will be in touch.' },
    ],
    'Sent': [
        { id: 3, to: 'jane@example.com', subject: 'Lunch plans', body: 'Hey Jane, are we still on for lunch tomorrow at 1 PM?' },
    ],
    'Drafts': [],
    'Trash': [],
};

const Mail = () => {
    const [mailboxes] = useState(['Inbox', 'Sent', 'Drafts', 'Trash']);
    const [selectedMailbox, setSelectedMailbox] = useState('Inbox');
    const [selectedEmail, setSelectedEmail] = useState(mockEmails['Inbox'][0]);

    const handleSelectEmail = (email) => {
        setSelectedEmail(email);
    };

    return (
        <div className="mail-app">
            <div className="mailbox-panel">
                <div className="mailbox-header">
                    <h2>Mail</h2>
                </div>
                <ul>
                    {mailboxes.map(box => (
                        <li 
                            key={box} 
                            className={selectedMailbox === box ? 'active' : ''}
                            onClick={() => setSelectedMailbox(box)}
                        >
                            {box} <span>{mockEmails[box]?.length || 0}</span>
                        </li>
                    ))}
                </ul>
            </div>
            <div className="message-list-panel">
                <div className="message-list-header">
                    <h3>{selectedMailbox}</h3>
                </div>
                <div className="message-list">
                    {(mockEmails[selectedMailbox] || []).map(email => (
                        <div 
                            key={email.id} 
                            className={`message-summary ${selectedEmail?.id === email.id ? 'active' : ''}`}
                            onClick={() => handleSelectEmail(email)}
                        >
                            <div className="message-from">{email.from || email.to}</div>
                            <div className="message-subject">{email.subject}</div>
                        </div>
                    ))}
                </div>
            </div>
            <div className="message-detail-panel">
                {selectedEmail ? (
                    <>
                        <div className="detail-header">
                            <h2>{selectedEmail.subject}</h2>
                            <p>From: {selectedEmail.from || 'Me'}</p>
                            {selectedEmail.to && <p>To: {selectedEmail.to}</p>}
                        </div>
                        <div className="detail-body">
                            <p>{selectedEmail.body}</p>
                        </div>
                    </>
                ) : (
                    <div className="no-message-selected">
                        <p>Select a message to read</p>
                    </div>
                )}
            </div>
        </div>
    );
};

export default Mail;

