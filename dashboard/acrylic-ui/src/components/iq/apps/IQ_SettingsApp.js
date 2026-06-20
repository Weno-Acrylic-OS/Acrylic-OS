// dashboard/acrylic-ui/src/components/iq/apps/IQ_SettingsApp.js
import React, { useState } from 'react';
import '../IQ.css';

const IQ_SettingsApp = () => {
    const [brightness, setBrightness] = useState(80);
    const [eqPreset, setEqPreset] = useState('Rock');

    return (
        <div className="iq-app iq-settings-app">
            <h1>Settings</h1>
            <div className="settings-category">
                <h2>Display</h2>
                <div className="setting-item">
                    <label>Brightness</label>
                    <div className="setting-control">
                        <input 
                            type="range" 
                            min="20" 
                            max="100" 
                            value={brightness} 
                            onChange={(e) => setBrightness(e.target.value)} 
                        />
                        <span>{brightness}%</span>
                    </div>
                </div>
            </div>

            <div className="settings-category">
                <h2>Sound</h2>
                <div className="setting-item">
                    <label>Equalizer Preset</label>
                    <div className="setting-control button-group">
                        <button className={eqPreset === 'Rock' ? 'active' : ''} onClick={() => setEqPreset('Rock')}>Rock</button>
                        <button className={eqPreset === 'Pop' ? 'active' : ''} onClick={() => setEqPreset('Pop')}>Pop</button>
                        <button className={eqPreset === 'Jazz' ? 'active' : ''} onClick={() => setEqPreset('Jazz')}>Jazz</button>
                    </div>
                </div>
            </div>

            <div className="settings-category">
                <h2>System</h2>
                <div className="setting-item">
                    <label>Software Version</label>
                    <div className="setting-control">
                        <span>IQ.2026.4.1</span>
                    </div>
                </div>
                <div className="setting-item">
                    <label>Software Updates</label>
                    <div className="setting-control">
                        <button className="system-btn">Check for Updates</button>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default IQ_SettingsApp;
