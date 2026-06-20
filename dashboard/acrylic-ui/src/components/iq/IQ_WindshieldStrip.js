import React, { useState, useEffect } from 'react';
import './IQ.css';
import { CornerUpRight, Music } from 'react-feather';

const IQ_WindshieldStrip = () => {
    const [speed, setSpeed] = useState(55); // Mock data
    const [gear, setGear] = useState('D');   // Mock data
    const [time, setTime] = useState(new Date());
    const [context, setContext] = useState({ type: 'media', title: 'Song Name - Artist' }); // Default context

    useEffect(() => {
        const timerId = setInterval(() => setTime(new Date()), 1000);

        const handleMessage = (event) => {
            if (event.data && event.data.type === 'iq-strip-update') {
                setContext(event.data.payload);
            }
        };
        window.addEventListener('message', handleMessage);

        return () => {
            clearInterval(timerId);
            window.removeEventListener('message', handleMessage);
        };
    }, []);

    const renderContext = () => {
        if (context.type === 'navigation') {
            return (
                <div className="strip-context-item nav">
                    <CornerUpRight size={28} />
                    <p>{context.text}</p>
                </div>
            );
        }
        return (
            <div className="strip-context-item media">
                <Music size={24} />
                <p>{context.title}</p>
            </div>
        );
    };

    return (
        <div className="iq-component iq-strip">
            <div className="strip-left">
                <div className="speed-display">{speed}</div>
                <div className="gear-display">{gear}</div>
            </div>
            <div className="strip-center">
                {renderContext()}
            </div>
            <div className="strip-right">
                <div className="time-display">{time.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })}</div>
            </div>
        </div>
    );
};

export default IQ_WindshieldStrip;

