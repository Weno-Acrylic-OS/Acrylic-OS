import React, { useState, useEffect, useRef } from 'react';
import './Alyssa.css';
import TextInput from './TextInput';
import { X, Send } from 'react-feather';

// This function maps the intent received from the AI server to a user-friendly text response.
const getResponseForIntent = (intent) => {
    switch (intent) {
        case 'get_weather':
            return "It's always sunny in Acrylic OS!";
        case 'get_time':
            return `The current time is ${new Date().toLocaleTimeString()}.`;
        case 'tell_joke':
            return "Why don't scientists trust atoms? Because they make up everything!";
        case 'open_calculator':
            // In a future version, this could trigger a callback to open the app.
            return "I can't open apps just yet, but I've noted that you'd like to open the calculator.";
        case 'unknown':
        default:
            return "I'm sorry, I don't quite understand that. I'm still learning!";
    }
};

const Alyssa = ({ isVisible, onClose }) => {
    const [messages, setMessages] = useState([
        { sender: 'alyssa', text: 'Hello! How can I help you today?' }
    ]);
    const [inputText, setInputText] = useState('');
    const [isLoading, setIsLoading] = useState(false);
    const messagesEndRef = useRef(null);

    useEffect(() => {
        messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
    }, [messages]);

    const handleSendMessage = async (e) => {
        e.preventDefault();
        if (!inputText.trim() || isLoading) return;

        const userMessage = { sender: 'user', text: inputText };
        setMessages(prev => [...prev, userMessage]);
        setInputText('');
        setIsLoading(true);

        try {
            const response = await fetch('http://localhost:5004/api/ask', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ prompt: inputText }),
            });
            if (!response.ok) {
                throw new Error('AI service is not available.');
            }
            const data = await response.json();
            const responseText = getResponseForIntent(data.intent);
            const alyssaMessage = { sender: 'alyssa', text: responseText };
            setMessages(prev => [...prev, alyssaMessage]);

        } catch (error) {
            console.error("Alyssa API error:", error);
            const errorMessage = { sender: 'alyssa', text: "I'm having trouble connecting to my brain right now." };
            setMessages(prev => [...prev, errorMessage]);
        } finally {
            setIsLoading(false);
        }
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
                {isLoading && (
                    <div className="alyssa-message-bubble alyssa typing-indicator">
                        <div className="message-sender-avatar">A</div>
                        <div className="message-text">...</div>
                    </div>
                )}
                <div ref={messagesEndRef} />
            </div>
            <form className="alyssa-input-form" onSubmit={handleSendMessage}>
                <TextInput 
                    placeholder="Ask Alyssa..."
                    value={inputText}
                    onChange={(e) => setInputText(e.target.value)}
                    disabled={isLoading}
                />
                <button type="submit" disabled={isLoading}><Send size={20} /></button>
            </form>
        </div>
    );
};

export default Alyssa;
