import React, { useState, useEffect, useRef } from 'react';
import './Alyssa.css';
import TextInput from './TextInput';
import { X, Send } from 'react-feather';

const Alyssa = ({ isVisible, onClose }) => {
    const [messages, setMessages] = useState([
        { sender: 'alyssa', text: 'Hello! How can I help you today?' }
    ]);
    const [inputText, setInputText] = useState('');
    const messagesEndRef = useRef(null);

    useEffect(() => {
        // Scroll to the latest message
        messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
    }, [messages]);

    const getAlyssaResponse = (userInput) => {
        const lowerInput = userInput.toLowerCase();
        if (lowerInput.includes('weather')) {
            return "It's always sunny in Acrylic OS!";
        }
        if (lowerInput.includes('hello') || lowerInput.includes('hi')) {
            return "Hello there! What can I do for you?";
        }
        if (lowerInput.includes('open')) {
            return "I can't open apps just yet, but that's a great feature for the future!";
        }
        return "I'm sorry, I'm just a demo and I don't understand that yet.";
    };

    const handleSendMessage = (e) => {
        e.preventDefault();
        if (!inputText.trim()) return;

        const userMessage = { sender: 'user', text: inputText };
        const newMessages = [...messages, userMessage];
        setMessages(newMessages);
        setInputText('');

        // Simulate Alyssa thinking and responding
        setTimeout(() => {
            const alyssaResponseText = getAlyssaResponse(inputText);
            const alyssaMessage = { sender: 'alyssa', text: alyssaResponseText };
            setMessages(prev => [...prev, alyssaMessage]);
        }, 800);
    };

    if (!isVisible) {
        return null;
    }

    return (
        <div className="alyssa-overlay">
            <div className="alyssa-header">
                <h2>Alyssa</h2>
                <button className="alyssa-close-btn" onClick={onClose}><X size={24} /></button>
            </div>
            <div className="alyssa-message-list">
                {messages.map((msg, index) => (
                    <div key={index} className={`alyssa-message-bubble ${msg.sender}`}>
                        <div className="message-sender-avatar">
                            {msg.sender === 'alyssa' ? 'A' : 'U'}
                        </div>
                        <div className="message-text">{msg.text}</div>
                    </div>
                ))}
                <div ref={messagesEndRef} />
            </div>
            <form className="alyssa-input-form" onSubmit={handleSendMessage}>
                <TextInput 
                    placeholder="Ask Alyssa..."
                    value={inputText}
                    onChange={(e) => setInputText(e.target.value)}
                />
                <button type="submit"><Send size={20} /></button>
            </form>
        </div>
    );
};

export default Alyssa;
