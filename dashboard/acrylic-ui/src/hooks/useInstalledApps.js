import { useState, useEffect } from 'react';

export const useInstalledApps = () => {
    const [apps, setApps] = useState([]);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchAndFilterApps = async () => {
            setIsLoading(true);
            setError(null);
            try {
                const response = await fetch('http://localhost:3001/api/apps');
                if (!response.ok) {
                    throw new Error(`API Error: ${response.status}`);
                }
                const allApps = await response.json();
                const installedApps = allApps.filter(app => app.installed);
                setApps(installedApps);
            } catch (err) {
                setError(err.message);
                console.error("Failed to fetch installed apps:", err);
            } finally {
                setIsLoading(false);
            }
        };

        fetchAndFilterApps();
        
        // Optional: Poll for changes, or use a more advanced state management
        // library to automatically update when apps are installed/uninstalled.
        // For now, the user may need to re-open the app grid to see changes.

    }, []);

    return { apps, isLoading, error };
};
