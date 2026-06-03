import React, { useState, useEffect } from 'react';
import './WenoStore.css';

const WenoStore = () => {
    const [apps, setApps] = useState([]);
    const [error, setError] = useState(null);

    const fetchApps = async () => {
        try {
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

    useEffect(() => {
        fetchApps();
    }, []);

    const handleInstall = async (appId) => {
        try {
            const response = await fetch(`http://localhost:3001/api/apps/${appId}/install`, { method: 'POST' });
            if (!response.ok) throw new Error('Failed to install');
            // Refresh the app list to show the new state
            fetchApps();
        } catch (e) {
            console.error('Install error:', e);
        }
    };

    const handleUninstall = async (appId) => {
        try {
            const response = await fetch(`http://localhost:3001/api/apps/${appId}/uninstall`, { method: 'POST' });
            if (!response.ok) throw new Error('Failed to uninstall');
            // Refresh the app list
            fetchApps();
        } catch (e) {
            console.error('Uninstall error:', e);
        }
    };

    return (
        <div className="weno-store-app">
            <div className="store-header">
                <h1>Weno App Store</h1>
            </div>
            <div className="app-list">
                {error && <p className="error-message">Failed to load apps: {error}</p>}
                {apps.length === 0 && !error && <p>No apps available in the store.</p>}
                {apps.map(app => (
                    <div key={app.id} className="app-item">
                        <div className="app-icon-placeholder"></div>
                        <div className="app-details">
                            <h3>{app.name}</h3>
                            <p>{app.description}</p>
                        </div>
                        <div className="app-version">
                            <p>v{app.version}</p>
                            {app.installed ? (
                                <button className="uninstall-btn" onClick={() => handleUninstall(app.id)}>Uninstall</button>
                            ) : (
                                <button className="install-btn" onClick={() => handleInstall(app.id)}>Install</button>
                            )}
                        </div>
                    </div>
                ))}
            </div>
        </div>
    );
};

export default WenoStore;
