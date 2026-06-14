import React, { useState } from 'react';
import './DashboardView.css';
import { useWidgetLayout, WIDGET_REGISTRY } from '../../hooks/WidgetLayoutContext';
import WidgetLibrary from '../common/WidgetLibrary';
import { X, Edit, Plus, ArrowUp, ArrowDown } from 'react-feather';

const DashboardView = ({ isVisible, onClose }) => {
    const { getWidgetsFor, removeWidget, moveWidget } = useWidgetLayout();
    const desktopWidgets = getWidgetsFor('desktop');
    const [isEditing, setIsEditing] = useState(false);
    const [showLibrary, setShowLibrary] = useState(false); // New state for modal

    if (!isVisible) {
        return null;
    }

    const handleRemove = (index) => {
        removeWidget('desktop', index);
    };

    const handleMove = (fromIndex, toIndex) => {
        moveWidget('desktop', fromIndex, toIndex);
    };

    return (
        <>
            <WidgetLibrary 
                isVisible={showLibrary} 
                onClose={() => setShowLibrary(false)} 
                personality="desktop" 
            />
            <div className="dashboard-view-overlay" onClick={onClose}>
                <div className="dashboard-view-panel" onClick={(e) => e.stopPropagation()}>
                    <div className="dashboard-header">
                        <h2>Dashboard</h2>
                        <div className="dashboard-header-actions">
                            <button onClick={() => setIsEditing(!isEditing)}>
                                {isEditing ? 'Done' : <Edit size={20} />}
                            </button>
                            <button onClick={onClose}><X size={20} /></button>
                        </div>
                    </div>
                    <div className="dashboard-widget-list">
                        {desktopWidgets.map((widgetName, index) => {
                            const WidgetComponent = WIDGET_REGISTRY[widgetName];
                            if (!WidgetComponent) {
                                return <div key={widgetName}>Widget '{widgetName}' not found.</div>;
                            }
                            return (
                                <div key={widgetName} className="widget-container">
                                    {isEditing && (
                                        <div className="widget-edit-controls">
                                            <button onClick={() => handleMove(index, index - 1)} disabled={index === 0}><ArrowUp size={16} /></button>
                                            <button onClick={() => handleMove(index, index + 1)} disabled={index === desktopWidgets.length - 1}><ArrowDown size={16} /></button>
                                            <button onClick={() => handleRemove(index)}><X size={16} /></button>
                                        </div>
                                    )}
                                    <div className="widget-wrapper">
                                        <WidgetComponent />
                                    </div>
                                </div>
                            );
                        })}
                        {isEditing && (
                            <button className="add-widget-btn" onClick={() => setShowLibrary(true)}>
                                <Plus size={20} /> Add Widget
                            </button>
                        )}
                    </div>
                </div>
            </div>
        </>
    );
};

export default DashboardView;

