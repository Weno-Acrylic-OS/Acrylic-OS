import React, { useState, useRef, useEffect } from 'react';
import './Messages.css';
import TextInput from '../components/common/TextInput';

const initialConversations = {
    '1': { name: 'Jane Doe', messages: [{id: 1, text: 'Hey, are we still on for lunch?', sender: 'Jane Doe'}, {id: 2, text: 'Yep, see you at 1!', sender: 'me'}] },
    '2': { name: 'Acme Corp', messages: [{id: 1, text: 'Your package has been dispatched.', sender: 'Acme Corp'}] },
};

const Messages = () => {
    const [conversations, setConversations] = useState(initialConversations);
    const [selectedId, setSelectedId] = useState('1');
    const [inputText, setInputText] = useState('');
    const messageEndRef = useRef(null);

    const selectedConversation = conversations[selectedId];

    useEffect(() => {
        // Scroll to the latest message when the conversation changes
        messageEndRef.current?.scrollIntoView({ behavior: 'smooth' });
    }, [selectedConversation]);

    const handleSendMessage = (e) => {
        e.preventDefault();
        if (!inputText.trim()) return;

        const newMessage = {
            id: Date.now(),
            text: inputText,
            sender: 'me',
        };

        const updatedConversation = {
            ...selectedConversation,
            messages: [...selectedConversation.messages, newMessage],
        };

        setConversations({
            ...conversations,
            [selectedId]: updatedConversation,
        });

        setInputText('');
    };

    return (
        <div className="messages-app">
            <div className="conversation-list">
                <div className="list-header">
                    <h2>Chats</h2>
                </div>
                {Object.keys(conversations).map(convId => (
                    <div 
                        key={convId} 
                        className={`conversation-item ${selectedId === convId ? 'active' : ''}`}
                        onClick={() => setSelectedId(convId)}
                    >
                        <div className="convo-avatar">{conversations[convId].name.charAt(0)}</div>
                        <div className="convo-details">
                            <div className="convo-name">{conversations[convId].name}</div>
                            <div className="convo-preview">
                                {conversations[convId].messages.slice(-1)[0].text}
                            </div>
                        </div>
                    </div>
                ))}
            </div>
            <div className="message-view">
                {selectedConversation ? (
                    <>
                        <div className="message-header">
                            <h3>{selectedConversation.name}</h3>
                        </div>
                        <div className="message-list">
                            {selectedConversation.messages.map(msg => (
                                <div key={msg.id} className={`message-bubble ${msg.sender === 'me' ? 'sent' : 'received'}`}>
                                    {msg.text}
                                </div>
                            ))}
                            <div ref={messageEndRef} />
                        </div>
                        <form className="message-input-form" onSubmit={handleSendMessage}>
                            <TextInput 
                                type="text"
                                placeholder="Type a message..."
                                value={inputText}
                                onChange={e => setInputText(e.target.value)}
                            />
                            <button type="submit">Send</button>
                        </form>
                    </>
                ) : (
                    <div className="no-conversation-selected">
                        <p>Select a conversation to start messaging.</p>
                    </div>
                )}
            </div>
        </div>
    );
};

export default Messages;

