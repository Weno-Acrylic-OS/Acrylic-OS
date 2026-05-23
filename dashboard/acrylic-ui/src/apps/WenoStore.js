import React, { useState, useEffect } from 'react';
import './WenoStore.css';

const WenoStore = () => {
    const [apps, setApps] = useState([]);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchApps = async () => {
            try {
                // The store backend is running on port 3001
                const response = await fetch('http://localhost:3001/api/apps');
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                const data = await response.json();
                setApps(data);
            } catch (e) {
                console.error("Failed to fetch apps:", e);
                setError(e.message);
            }
        };

        fetchApps();
    }, []);

    return (
        <div className="weno-store-app">
            <div className="store-header">
                <h1>Weno App Store</h1>
            </div>
            <div className="app-list">
                {error && <p className="error-message">Failed to load apps: {error}</p>}
                {apps.length === 0 && !error && <p>No apps available.</p>}
                {apps.map(app => (
                    <div key={app.id} className="app-item">
                        <div className="app-icon-placeholder"></div>
                        <div className="app-details">
                            <h3>{app.name}</h3>
                            <p>{app.description}</p>
                        </div>
                        <div className="app-version">
                            <p>v{app.version}</p>
                            <button>Install</button>
                        </div>
                    </div>
                ))}
            </div>
        </div>
    );
};

export default WenoStore;
