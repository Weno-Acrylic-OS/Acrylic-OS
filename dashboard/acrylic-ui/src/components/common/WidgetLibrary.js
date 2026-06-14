import React from 'react';
import './WidgetLibrary.css';
import { useWidgetLayout, WIDGET_REGISTRY } from '../../hooks/WidgetLayoutContext';
import { X, PlusCircle } from 'react-feather';

const WidgetLibrary = ({ isVisible, onClose, personality }) => {
    const { getWidgetsFor, addWidget } = useWidgetLayout();
    const currentWidgets = getWidgetsFor(personality);
    
    // Find which widgets are available to be added
    const availableWidgets = Object.keys(WIDGET_REGISTRY).filter(
        widgetName => !currentWidgets.includes(widgetName)
    );

    if (!isVisible) {
        return null;
    }

    const handleAddWidget = (widgetName) => {
        addWidget(personality, widgetName);
        // Optional: close the library after adding a widget
        // onClose(); 
    };

    return (
        <div className="widget-library-overlay" onClick={onClose}>
            <div className="widget-library-panel" onClick={(e) => e.stopPropagation()}>
                <div className="widget-library-header">
                    <h2>Add a Widget</h2>
                    <button onClick={onClose}><X size={20} /></button>
                </div>
                <div className="available-widgets-list">
                    {availableWidgets.length > 0 ? (
                        availableWidgets.map(widgetName => (
                            <div key={widgetName} className="available-widget-item">
                                <span>{widgetName}</span>
                                <button onClick={() => handleAddWidget(widgetName)}>
                                    <PlusCircle size={18} /> Add
                                </button>
                            </div>
                        ))
                    ) : (
                        <p className="no-widgets-message">All available widgets have been added.</p>
                    )}
                </div>
            </div>
        </div>
    );
};

export default WidgetLibrary;
