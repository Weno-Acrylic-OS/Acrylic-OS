import React, { useState } from 'react';
import './Taskbar.css';
import StartMenu from './StartMenu';
const Taskbar = ({ onAppClick, windows, onTaskbarAppClick }) => {
    const [showStartMenu, setShowStartMenu] = useState(false);

    const toggleStartMenu = () => {
        setShowStartMenu(!showStartMenu);
    };

    return (
        <>
            {showStartMenu && <StartMenu onAppClick={onAppClick} />}
            <div className="taskbar">
                <button className="start-button" onClick={toggleStartMenu}>
                    <img src="all_apps.svg" alt="All Apps" style={{ width: '20px', height: '20px' }} />
                </button>

                <div className="taskbar-apps">
                    {windows.map(win => (
                        <button 
                            key={win.id} 
                            className={`taskbar-app-button ${win.isMinimized ? 'minimized' : ''}`}
                            onClick={() => onTaskbarAppClick(win.id)}
                        >
                            {win.title.substring(0, 10)}
                        </button>
                    ))}
                </div>

            </div>
        </>
    );
};

export default Taskbar;
