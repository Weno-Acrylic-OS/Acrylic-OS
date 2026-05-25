import React, { useState, useEffect, useRef } from 'react';
import './Window.css';

const Window = ({ title, children, onClose, onMinimize, initialPosition, initialSize, zIndex, onFocus }) => {
    // Window State
    const [position, setPosition] = useState(initialPosition || { x: 100, y: 50 });
    const [size, setSize] = useState(initialSize || { w: 400, h: 300 });
    const [isMaximized, setIsMaximized] = useState(false);
    
    // Previous state before maximizing
    const [prevPosition, setPrevPosition] = useState(position);
    const [prevSize, setPrevSize] = useState(size);

    // Dragging State
    const [isDragging, setIsDragging] = useState(false);
    const [dragStart, setDragStart] = useState({ x: 0, y: 0 });

    // Resizing State
    const [isResizing, setIsResizing] = useState(false);
    
    const windowRef = useRef(null);

    // --- Dragging Handlers ---
    const handleDragMouseDown = (e) => {
        if (e.button !== 0 || isMaximized) return;
        onFocus(); // Bring window to front
        setIsDragging(true);
        setDragStart({ x: e.clientX - position.x, y: e.clientY - position.y });
        e.preventDefault();
    };

    // --- Resizing Handlers ---
    const handleResizeMouseDown = (e) => {
        if (e.button !== 0 || isMaximized) return;
        onFocus(); // Bring window to front
        setIsResizing(true);
        e.preventDefault();
    };

    // --- Maximize Handler ---
    const handleToggleMaximize = () => {
        if (isMaximized) {
            // Restore
            setPosition(prevPosition);
            setSize(prevSize);
        } else {
            // Maximize
            onFocus(); // Bring window to front
            setPrevPosition(position);
            setPrevSize(size);
            setPosition({ x: 0, y: 30 }); // Position below top bar
            setSize({ w: window.innerWidth, h: window.innerHeight - 70 }); // Fill space between top and bottom bar
        }
        setIsMaximized(!isMaximized);
    };

    // --- Global Event Listeners Effect for Drag/Resize ---
    useEffect(() => {
        const handleGlobalMouseUp = () => {
            setIsDragging(false);
            setIsResizing(false);
        };

        const handleGlobalMouseMove = (e) => {
            if (isDragging) {
                setPosition({ x: e.clientX - dragStart.x, y: e.clientY - dragStart.y });
            }
            if (isResizing) {
                const newWidth = e.clientX - position.x;
                const newHeight = e.clientY - position.y;
                setSize({ w: Math.max(newWidth, 200), h: Math.max(newHeight, 150) });
            }
        };
        
        if (isDragging || isResizing) {
            window.addEventListener('mousemove', handleGlobalMouseMove);
            window.addEventListener('mouseup', handleGlobalMouseUp);
        }

        return () => {
            window.removeEventListener('mousemove', handleGlobalMouseMove);
            window.removeEventListener('mouseup', handleGlobalMouseUp);
        };
    }, [isDragging, isResizing, dragStart, position]);


    return (
        <div 
            className={`window ${isMaximized ? 'maximized' : ''}`}
            style={{ 
                top: `${position.y}px`, 
                left: `${position.x}px`,
                width: `${size.w}px`,
                height: `${size.h}px`,
                zIndex: zIndex,
            }}
            ref={windowRef}
            onMouseDown={onFocus}
        >
            <div className="title-bar" onMouseDown={handleDragMouseDown} onDoubleClick={handleToggleMaximize}>
                <span className="title">{title}</span>
                <div className="title-bar-buttons">
                    <button className="title-bar-button" onClick={onMinimize}>
                        {/* Minimize Icon */}
                        <svg width="12" height="12" viewBox="0 0 24 24"><path stroke="black" strokeWidth="3" d="M4,12 L20,12"></path></svg>
                    </button>
                    <button className="title-bar-button" onClick={handleToggleMaximize}>
                        {/* Maximize Icon */}
                        <svg width="12" height="12" viewBox="0 0 24 24"><path stroke="black" strokeWidth="3" fill="none" d="M2,2 L22,2 L22,22 L2,22 L2,2 Z"></path></svg>
                    </button>
                    <button className="title-bar-button close-button" onClick={onClose}>
                        {/* Close Icon */}
                        <svg width="12" height="12" viewBox="0 0 24 24"><path d="M6 18L18 6M6 6L18 18" stroke="black" strokeWidth="3"/></svg>
                    </button>
                </div>
            </div>
            <div className="window-content">
                {children}
            </div>
            {!isMaximized && <div className="resize-handle" onMouseDown={handleResizeMouseDown}></div>}
        </div>
    );
};

export default Window;
