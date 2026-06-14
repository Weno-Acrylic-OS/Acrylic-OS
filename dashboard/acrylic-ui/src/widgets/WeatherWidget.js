import React from 'react';
import './WeatherWidget.css';
import { Sun } from 'react-feather';

const WeatherWidget = () => {
    const weather = {
        city: 'San Francisco',
        temperature: 75,
        condition: 'Sunny',
    };

    return (
        <div className="widget weather-widget">
            <div className="weather-icon">
                <Sun size={48} />
            </div>
            <div className="weather-info">
                <p className="temperature">{weather.temperature}°</p>
                <p className="condition">{weather.condition}</p>
                <p className="city">{weather.city}</p>
            </div>
        </div>
    );
};

export default WeatherWidget;
