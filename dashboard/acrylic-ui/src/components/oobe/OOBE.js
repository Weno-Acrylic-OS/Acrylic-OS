import React, { useState } from 'react';
import './OOBE.css';

// OOBE Steps
const WelcomeScreen = ({ onNext }) => (
    <div className="oobe-screen">
        <h1>Welcome to Acrylic OS!</h1>
        <p>Let's get your new operating system set up.</p>
        <button onClick={onNext}>Start Setup</button>
    </div>
);

const PinSetupScreen = ({ onNext, onPinChange }) => {
    const [newPin, setNewPin] = useState('');
    const [confirmPin, setConfirmPin] = useState('');
    const [error, setError] = useState('');

    const handleSetPin = () => {
        if (newPin.length !== 4) {
            setError('PIN must be 4 digits.');
            return;
        }
        if (newPin !== confirmPin) {
            setError('PINs do not match.');
            return;
        }
        onPinChange(newPin);
        onNext();
    };

    return (
        <div className="oobe-screen">
            <h1>Set Your PIN</h1>
            <p>This PIN will be used to unlock your device.</p>
            <input type="password" placeholder="New PIN" value={newPin} onChange={e => setNewPin(e.target.value)} maxLength="4" />
            <input type="password" placeholder="Confirm PIN" value={confirmPin} onChange={e => setConfirmPin(e.target.value)} maxLength="4" />
            {error && <p className="oobe-error">{error}</p>}
            <button onClick={handleSetPin}>Set PIN</button>
        </div>
    );
};

const NetworkSetupScreen = ({ onNext }) => (
    <div className="oobe-screen">
        <h1>Network Setup</h1>
        <p>Connect to a Wi-Fi network (placeholder).</p>
        <button onClick={onNext}>Skip for now</button>
    </div>
);

const CompletionScreen = ({ onNext }) => (
    <div className="oobe-screen">
        <h1>Setup Complete!</h1>
        <p>Your Acrylic OS is ready to use.</p>
        <button onClick={onNext}>Finish</button>
    </div>
);


const OOBE = ({ onComplete, onPinChange }) => {
    const [step, setStep] = useState(0);

    const steps = [
        <WelcomeScreen onNext={() => setStep(1)} />,
        <PinSetupScreen onNext={() => setStep(2)} onPinChange={onPinChange} />,
        <NetworkSetupScreen onNext={() => setStep(3)} />,
        <CompletionScreen onNext={onComplete} />,
    ];

    return (
        <div className="oobe-container">
            {steps[step]}
        </div>
    );
};

export default OOBE;
