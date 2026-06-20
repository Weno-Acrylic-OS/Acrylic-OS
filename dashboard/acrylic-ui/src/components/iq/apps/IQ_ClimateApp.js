// dashboard/acrylic-ui/src/components/iq/apps/IQ_ClimateApp.js
import React, { useState } from 'react';
import '../IQ.css';
import { Wind, ChevronUp, ChevronDown } from 'react-feather';

const ClimateZone = ({ title, temp, onTempChange }) => {
    return (
        <div className="climate-zone">
            <p className="zone-title">{title}</p>
            <div className="temp-control">
                <button onClick={() => onTempChange(temp - 1)}><ChevronDown size={32} /></button>
                <p className="temp-display">{temp}°</p>
                <button onClick={() => onTempChange(temp + 1)}><ChevronUp size={32} /></button>
            </div>
        </div>
    );
};

const IQ_ClimateApp = () => {
    const [driverTemp, setDriverTemp] = useState(72);
    const [passengerTemp, setPassengerTemp] = useState(74);
    const [fanSpeed, setFanSpeed] = useState(2);
    const [isDriverSeatHeated, setIsDriverSeatHeated] = useState(false);
    const [isPassengerSeatHeated, setIsPassengerSeatHeated] = useState(false);

    const fanSpeeds = [1, 2, 3, 4, 5];

    return (
        <div className="iq-app iq-climate-app">
            <div className="main-climate-controls">
                <ClimateZone title="Driver" temp={driverTemp} onTempChange={setDriverTemp} />
                <div className="fan-control">
                    <Wind size={40} />
                    <div className="fan-speed-selector">
                        {fanSpeeds.map(speed => (
                            <button 
                                key={speed}
                                className={`fan-speed-btn ${fanSpeed >= speed ? 'active' : ''}`}
                                onClick={() => setFanSpeed(speed)}
                            />
                        ))}
                    </div>
                </div>
                <ClimateZone title="Passenger" temp={passengerTemp} onTempChange={setPassengerTemp} />
            </div>
            <div className="secondary-climate-controls">
                <button 
                    className={`seat-heat-btn ${isDriverSeatHeated ? 'active' : ''}`}
                    onClick={() => setIsDriverSeatHeated(!isDriverSeatHeated)}
                >
                    Driver Seat
                </button>
                <button 
                    className={`seat-heat-btn ${isPassengerSeatHeated ? 'active' : ''}`}
                    onClick={() => setIsPassengerSeatHeated(!isPassengerSeatHeated)}
                >
                    Passenger Seat
                </button>
            </div>
        </div>
    );
};

export default IQ_ClimateApp;
