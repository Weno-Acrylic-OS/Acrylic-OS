import React, { useState, useEffect } from 'react';
import './Car.css';

const Car = () => {
    const [message, setMessage] = useState('Waiting for connection...');
    const [isConnected, setIsConnected] = useState(false);

    const isInsideSimulator = window.parent !== window;

    useEffect(() => {
        if (isInsideSimulator) {
            // In the simulator, listen for messages from the parent shell
            const handleSimMessage = (event) => {
                if (event.data && event.data.type === 'weno-car-receive') {
                    console.log('Car Display (Sim): Received message via postMessage:', event.data.message);
                    setMessage(event.data.message);
                    setIsConnected(true); // Consider it connected when we get a message
                }
            };
            window.addEventListener('message', handleSimMessage);
            setMessage('Connected (Simulated)'); // Start with a connected message
            setIsConnected(true);
            return () => window.removeEventListener('message', handleSimMessage);
        }

        // In the real OS, use a real WebSocket
        const ws = new WebSocket('ws://localhost:5003/ws');
        ws.onopen = () => {
            setIsConnected(true);
            setMessage('Connected');
        };
        ws.onmessage = (event) => {
            setMessage(event.data);
        };
        ws.onclose = () => {
            setIsConnected(false);
            setMessage('Disconnected');
        };
        ws.onerror = () => {
            setIsConnected(false);
            setMessage('Connection Error');
        };
        return () => ws.close();

    }, [isInsideSimulator]);

    return (
        <div className="car-display">
            <div className={`connection-status ${isConnected ? 'connected' : ''}`}></div>
            <h1 className="car-message">{message}</h1>
        </div>
    );
};

export default Car;
