// dashboard/acrylic-ui/src/components/iq/apps/IQ_NavigationApp.js
import React, { useState } from 'react';
import '../IQ.css';
import { Search, ArrowLeft, CornerUpRight } from 'react-feather';

const MAP_IMAGE_URL = 'https://images.unsplash.com/photo-1582842195828-5e2a63789122?q=80&w=2670&auto=format&fit=crop';

const IQ_NavigationApp = () => {
    const [isNavigating, setIsNavigating] = useState(false);
    const [searchQuery, setSearchQuery] = useState('');

    const handleSearch = (e) => {
        e.preventDefault();
        if (!searchQuery) return;
        // Simulate finding a destination and starting navigation
        setIsNavigating(true);

        // Send messages to the other displays
        window.parent.postMessage({
            iq_sender: 'console', type: 'iq-ar-update', payload: { type: 'navigation', text: 'Turn Right', distance: '150 FT' }
        }, '*');
        window.parent.postMessage({
            iq_sender: 'console', type: 'iq-strip-update', payload: { type: 'navigation', text: 'Turn Right' }
        }, '*');
    };

    const stopNavigation = () => {
        setIsNavigating(false);
        setSearchQuery('');
    };

    return (
        <div className="iq-app iq-nav-app" style={{ backgroundImage: `url(${MAP_IMAGE_URL})` }}>
            <div className="nav-overlay">
                {!isNavigating ? (
                    <form className="nav-search-bar" onSubmit={handleSearch}>
                        <Search size={24} />
                        <input 
                            type="text" 
                            placeholder="Search for a destination..." 
                            value={searchQuery}
                            onChange={(e) => setSearchQuery(e.target.value)}
                        />
                    </form>
                ) : (
                    <div className="nav-active-view">
                        <div className="nav-instruction-card">
                            <CornerUpRight size={48} />
                            <div className="instruction-text">
                                <h2>Turn Right</h2>
                                <p>150 FT</p>
                            </div>
                        </div>
                        <button className="nav-stop-btn" onClick={stopNavigation}>
                            <X size={24} /> End
                        </button>
                    </div>
                )}
            </div>
        </div>
    );
};

export default IQ_NavigationApp;
