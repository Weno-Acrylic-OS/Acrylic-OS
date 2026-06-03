import React from 'react';
import '../PhoneApp.css';

const mockContacts = [
    { id: 1, name: 'Acme Corp' },
    { id: 2, name: 'Jane Doe' },
    { id: 3, name: 'John Smith' },
    { id: 4, name: 'Mom' },
    { id: 5, name: 'Work' },
].sort((a, b) => a.name.localeCompare(b.name));

const Contacts = ({ onCall }) => {
    return (
        <div className="contacts-container">
            {mockContacts.map(contact => (
                <div key={contact.id} className="contact-item" onClick={() => onCall(contact.name)}>
                    <div className="contact-avatar">{contact.name.charAt(0)}</div>
                    <div className="contact-name">{contact.name}</div>
                </div>
            ))}
        </div>
    );
};

export default Contacts;
