import React, { useState } from 'react';
import './Home.css';
import { Power, Thermometer, Zap, Sun } from 'react-feather';

const initialDevices = {
    '1': { id: '1', name: 'Living Room Light', type: 'light', state: { isOn: false } },
    '2': { id: '2', name: 'Kitchen Light', type: 'light', state: { isOn: true } },
    '3': { id: '3', name: 'Main Thermostat', type: 'thermostat', state: { temperature: 70 } },
    '4': { id: '4', name: 'Bedroom Outlet', type: 'outlet', state: { isOn: true } },
};

const DeviceCard = ({ device, onStateChange }) => {
    const { name, type, state } = device;

    const handleToggle = () => {
        if (type === 'light' || type === 'outlet') {
            onStateChange(device.id, { isOn: !state.isOn });
        }
    };
    
    const renderIcon = () => {
        switch(type) {
            case 'light':
                return <Sun size={28} color={state.isOn ? '#f0ad4e' : '#666'} />;
            case 'thermostat':
                return <Thermometer size={28} color="#d9534f" />;
            case 'outlet':
                return <Zap size={28} color={state.isOn ? '#5cb85c' : '#666'} />;
            default:
                return null;
        }
    };
    
    const renderState = () => {
        switch(type) {
            case 'light':
            case 'outlet':
                return state.isOn ? 'On' : 'Off';
            case 'thermostat':
                return `${state.temperature}°F`;
            default:
                return null;
        }
    };

    return (
        <div className={`device-card ${state.isOn ? 'on' : ''}`} onClick={handleToggle}>
            <div className="device-icon">
                {renderIcon()}
            </div>
            <div className="device-info">
                <p className="device-name">{name}</p>
                <p className="device-state">{renderState()}</p>
            </div>
        </div>
    );
};


const Home = () => {
    const [devices, setDevices] = useState(initialDevices);

    const handleDeviceStateChange = (deviceId, newState) => {
        setDevices(prevDevices => ({
            ...prevDevices,
            [deviceId]: {
                ...prevDevices[deviceId],
                state: { ...prevDevices[deviceId].state, ...newState },
            },
        }));
    };

    return (
        <div className="home-app">
            <div className="home-header">
                <h1>My Home</h1>
                <p>Welcome back!</p>
            </div>
            <div className="device-grid">
                {Object.values(devices).map(device => (
                    <DeviceCard 
                        key={device.id} 
                        device={device} 
                        onStateChange={handleDeviceStateChange} 
                    />
                ))}
            </div>
        </div>
    );
};

export default Home;

