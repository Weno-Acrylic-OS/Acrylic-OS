import React, { useState, useEffect } from 'react';
import './Files.css';

const Files = () => {
    const [path, setPath] = useState('/');
    const [items, setItems] = useState([]);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchFiles = async () => {
            setIsLoading(true);
            setError(null);
            try {
                // The server runs on the same device, so we can use localhost
                const response = await fetch(`http://localhost:5001/api/fs?path=${encodeURIComponent(path)}`);
                if (!response.ok) {
                    throw new Error(`Failed to fetch: ${response.status} ${response.statusText}`);
                }
                const data = await response.json();
                if (data.error) {
                    throw new Error(data.error);
                }
                // Sort with directories first
                const sortedItems = data.items.sort((a, b) => {
                    if (a.type === b.type) return a.name.localeCompare(b.name);
                    return a.type === 'directory' ? -1 : 1;
                });
                setItems(sortedItems);
            } catch (err) {
                setError(err.message);
            } finally {
                setIsLoading(false);
            }
        };

        fetchFiles();
    }, [path]);

    const handleItemClick = (item) => {
        if (item.type === 'directory') {
            // A simple way to join paths
            const newPath = path === '/' ? `/${item.name}` : `${path}/${item.name}`;
            setPath(newPath);
        }
        // In a real app, clicking a file would open it
    };
    
    const goUp = () => {
        if (path === '/') return;
        const parentPath = path.substring(0, path.lastIndexOf('/')) || '/';
        setPath(parentPath);
    };

    return (
        <div className="files-app">
            <div className="files-toolbar">
                <button onClick={goUp} disabled={path === '/'}>↑ Up</button>
                <div className="path-display">{path}</div>
            </div>
            <div className="files-list">
                {isLoading && <div className="loading-indicator">Loading...</div>}
                {error && <div className="error-indicator">Error: {error}</div>}
                {!isLoading && !error && items.map((item, index) => (
                    <div key={index} className="file-item" onDoubleClick={() => handleItemClick(item)}>
                        <span className="file-icon">{item.type === 'directory' ? '📁' : '📄'}</span>
                        <span className="file-name">{item.name}</span>
                    </div>
                ))}
            </div>
        </div>
    );
};

export default Files;

