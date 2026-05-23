import React from 'react';
import './ActivitiesView.css';

const ActivitiesView = ({ windows, onCloseWindow, onSwitchWindow }) => {
    return (
        <div className="activities-view">
            <h2>Activities</h2>
            <div className="window-previews">
                {windows.map(win => (
                    <div key={win.id} className="window-preview" onClick={() => onSwitchWindow(win.id)}>
                        <div className="preview-title-bar">
                            <span>{win.title}</span>
                            <button className="preview-close-button" onClick={(e) => {
                                e.stopPropagation(); // Prevent onSwitchWindow from firing
                                onCloseWindow(win.id);
                            }}>×</button>
                        </div>
                        <div className="preview-content">
                            {/* A more advanced version could render a thumbnail here */}
                        </div>
                    </div>
                ))}
            </div>
        </div>
    );
};

export default ActivitiesView;
