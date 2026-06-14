import React, { useState } from 'react';
import './AlyssaApps.css';
import TextInput from '../components/common/TextInput';

const AlyssaApps = () => {
    const [prompt, setPrompt] = useState('');
    const [status, setStatus] = useState('');
    const [isLoading, setIsLoading] = useState(false);

    const handleCreateApp = async () => {
        if (!prompt) return;
        setIsLoading(true);
        setStatus('Alyssa is thinking...');

        try {
            const response = await fetch('http://localhost:3001/api/alyssa/create-app', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ prompt: prompt }),
            });

            const data = await response.json();

            if (!response.ok) {
                throw new Error(data.error || 'Failed to create app.');
            }
            
            setStatus(`Success! "${data.app.name}" is now available in the Weno Store.`);
            setPrompt(''); // Clear input on success
        } catch (err) {
            setStatus(`Error: ${err.message}`);
        } finally {
            setIsLoading(false);
        }
    };

    return (
        <div className="alyssa-apps">
            <div className="alyssa-apps-header">
                <h1>Alyssa Apps</h1>
                <p>Describe an app you want Alyssa to create.</p>
            </div>
            <div className="alyssa-apps-controls">
                <TextInput 
                    placeholder="e.g., A simple pomodoro timer"
                    value={prompt}
                    onChange={(e) => setPrompt(e.target.value)}
                    disabled={isLoading}
                />
                <button onClick={handleCreateApp} disabled={isLoading}>
                    {isLoading ? 'Creating...' : 'Create App'}
                </button>
            </div>
            {status && (
                <div className="alyssa-apps-status">
                    <p>{status}</p>
                </div>
            )}
        </div>
    );
};

export default AlyssaApps;
