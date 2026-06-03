import React, { useState, useEffect } from 'react';
import './Photos.css';

const Photos = () => {
    const [photos, setPhotos] = useState([]);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState(null);
    const [selectedPhoto, setSelectedPhoto] = useState(null);

    useEffect(() => {
        const fetchPhotos = async () => {
            setIsLoading(true);
            setError(null);
            try {
                const response = await fetch('http://localhost:5002/api/photos');
                if (!response.ok) {
                    throw new Error(`API Error: ${response.status}`);
                }
                const data = await response.json();
                setPhotos(data);
            } catch (err) {
                setError(err.message);
            } finally {
                setIsLoading(false);
            }
        };

        fetchPhotos();
    }, []);

    const PhotoModal = ({ photo, onClose }) => {
        if (!photo) return null;
        return (
            <div className="photo-modal-overlay" onClick={onClose}>
                <div className="photo-modal-content">
                    <img src={`http://localhost:5002${photo.full_url}`} alt={photo.id} />
                </div>
            </div>
        );
    };

    return (
        <div className="photos-app">
            <div className="photos-header">
                <h1>Photos</h1>
            </div>
            <div className="photos-grid">
                {isLoading && <p>Loading photos...</p>}
                {error && <p className="error-message">Error loading photos: {error}</p>}
                {photos.map(photo => (
                    <div key={photo.id} className="photo-thumbnail" onClick={() => setSelectedPhoto(photo)}>
                        <img src={`http://localhost:5002${photo.thumbnail_url}`} alt={`Thumbnail for ${photo.id}`} />
                    </div>
                ))}
            </div>
            <PhotoModal photo={selectedPhoto} onClose={() => setSelectedPhoto(null)} />
        </div>
    );
};

export default Photos;

