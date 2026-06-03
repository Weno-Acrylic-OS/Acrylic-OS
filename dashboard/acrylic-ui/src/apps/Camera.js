import React, { useState, useRef, useEffect, useCallback } from 'react';
import './Camera.css';
import { Camera as CameraIcon, RefreshCw, Aperture } from 'react-feather';

const CameraApp = () => {
    const videoRef = useRef(null);
    const canvasRef = useRef(null);
    const [lastPhoto, setLastPhoto] = useState(null);
    const [stream, setStream] = useState(null);

    const startCamera = useCallback(async () => {
        try {
            const stream = await navigator.mediaDevices.getUserMedia({ 
                video: { facingMode: 'environment' } 
            });
            if (videoRef.current) {
                videoRef.current.srcObject = stream;
            }
            setStream(stream);
        } catch (err) {
            console.error("Error accessing webcam:", err);
            // Handle cases where permission is denied
        }
    }, []);

    useEffect(() => {
        startCamera();

        // Cleanup: stop the camera stream when the component unmounts
        return () => {
            if (stream) {
                stream.getTracks().forEach(track => track.stop());
            }
        };
    }, [startCamera]);

    const handleTakePhoto = () => {
        if (!videoRef.current) return;

        const video = videoRef.current;
        const canvas = canvasRef.current;
        canvas.width = video.videoWidth;
        canvas.height = video.videoHeight;
        
        const context = canvas.getContext('2d');
        context.drawImage(video, 0, 0, video.videoWidth, video.videoHeight);
        
        const dataUrl = canvas.toDataURL('image/png');
        setLastPhoto(dataUrl);

        // Shutter effect
        const shutter = document.querySelector('.shutter-effect');
        if (shutter) {
            shutter.classList.add('flash');
            setTimeout(() => shutter.classList.remove('flash'), 300);
        }
    };

    return (
        <div className="camera-app">
            <video ref={videoRef} autoPlay playsInline className="camera-viewfinder"></video>
            <canvas ref={canvasRef} style={{ display: 'none' }}></canvas>
            <div className="shutter-effect"></div>

            <div className="camera-controls">
                <div className="thumbnail-container">
                    {lastPhoto && <img src={lastPhoto} alt="Last taken" />}
                </div>

                <div className="shutter-button-container">
                    <button className="shutter-button" onClick={handleTakePhoto}>
                        <Aperture size={32} />
                    </button>
                </div>

                <div className="switch-camera-container">
                    <button className="switch-camera-button">
                        <RefreshCw size={24} />
                    </button>
                </div>
            </div>
        </div>
    );
};

export default CameraApp;
