import React from 'react';
import './AppMenu.css';

const AppMenu = ({ items }) => {
    return (
        <div className="app-menu">
            <ul>
                {items.map((item, index) => (
                    <li key={index}>{item}</li>
                ))}
            </ul>
        </div>
    );
};

export default AppMenu;
