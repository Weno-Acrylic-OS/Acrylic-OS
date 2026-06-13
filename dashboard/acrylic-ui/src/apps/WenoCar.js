import React, { useState, useEffect, useRef } from 'react';
import './WenoCar.css';
import TextInput from '../components/common/TextInput';

const WenoCar = () => {
    const [status, setStatus] = useState('Disconnected');
    const [message, setMessage] = useState('');
    const socketRef = useRef(null);

    const isInsideSimulator = window.parent !== window;

    useEffect(() => {
        if (isInsideSimulator) {
            // In the simulator, we don't need a real WebSocket.
            // We can just pretend to be connected.
            setStatus('Connected (Simulated)');
            return;
        }

        // Establish real WebSocket connection only in the OS
        socketRef.current = new WebSocket('ws://localhost:5003/ws');
        socketRef.current.onopen = () => setStatus('Connected');
        socketRef.current.onclose = () => setStatus('Disconnected');
        socketRef.current.onerror = () => setStatus('Error');

        return () => {
            if (socketRef.current) {
                socketRef.current.close();
            }
        };
    }, [isInsideSimulator]);

    const handleSendMessage = () => {
        if (!message) return;

        if (isInsideSimulator) {
            // Send message to the parent shell
            window.parent.postMessage({ type: 'weno-car-send', message: message }, '*');
            console.log('WenoCar App (Sim): Sent message via postMessage.');
            setMessage('');
        } else {
            // Send message via real WebSocket
            if (socketRef.current && socketRef.current.readyState === WebSocket.OPEN) {
                socketRef.current.send(message);
                setMessage('');
            } else {
                console.warn('Cannot send message, WebSocket is not open.');
            }
        }
    };

    return (
        <div className="weno-car-app">
            <div className="weno-car-header">
                <h1>Weno Car+</h1>
                <p>Connection Status: <span className={`status-${status.toLowerCase()}`}>{status}</span></p>
            </div>
            <div className="weno-car-controls">
                <p>Enter a message to display on the car screen:</p>
                <TextInput 
                    placeholder="Type here..."
                    value={message}
                    onChange={(e) => setMessage(e.target.value)}
                />
                <button onClick={handleSendMessage}>Send to Car</button>
            </div>
        </div>
    );
};

export default WenoCar;
