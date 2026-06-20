import React, { useState, useEffect } from 'react';
import './IQ.css';
import { CornerUpRight, Music } from 'react-feather';

// --- AR View Renderers ---
const AR_NavigationView = ({ payload }) => (
    <div className="ar-view navigation-view">
        <CornerUpRight size={80} />
        <h1>{payload.text}</h1>
        <p>{payload.distance || ''}</p>
    </div>
);

const AR_MediaView = ({ payload }) => (
    <div className="ar-view media-view">
        <Music size={60} />
        <h2>{payload.title}</h2>
        <p>{payload.artist}</p>
    </div>
);

const viewRegistry = {
    'navigation': AR_NavigationView,
    'media': AR_MediaView,
};

const IQ_AR_Overlay = () => {
    const [activeView, setActiveView] = useState(null); // e.g., { type: 'navigation', payload: {...} }

    useEffect(() => {
        let timeoutId = null;

        const handleMessage = (event) => {
            if (event.data && event.data.type === 'iq-ar-update') {
                setActiveView(event.data.payload);
                
                // Clear any existing timeout
                if (timeoutId) clearTimeout(timeoutId);

                // Set a new timeout to clear the view
                timeoutId = setTimeout(() => {
                    setActiveView(null);
                    timeoutId = null;
                }, 5000); // AR views last for 5 seconds
            }
        };
        window.addEventListener('message', handleMessage);
        
        return () => {
            window.removeEventListener('message', handleMessage);
            if (timeoutId) clearTimeout(timeoutId);
        };
    }, []);

    if (!activeView) {
        return <div className="iq-component iq-ar"></div>; // Render empty when inactive
    }

    const ViewComponent = viewRegistry[activeView.type];

    return (
        <div className="iq-component iq-ar">
            {ViewComponent ? <ViewComponent payload={activeView.payload} /> : null}
        </div>
    );
};

export default IQ_AR_Overlay;
