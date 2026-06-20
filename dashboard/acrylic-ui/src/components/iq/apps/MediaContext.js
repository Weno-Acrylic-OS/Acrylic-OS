// dashboard/acrylic-ui/src/components/iq/apps/MediaContext.js
import React, { createContext, useState, useContext } from 'react';

const mockTracks = [
    { title: 'Starlight', artist: 'Muse', albumArtUrl: 'https://upload.wikimedia.org/wikipedia/en/thumb/e/e9/Muse_Starlight.jpg/220px-Muse_Starlight.jpg', progress: 30 },
    { title: 'Bohemian Rhapsody', artist: 'Queen', albumArtUrl: 'https://upload.wikimedia.org/wikipedia/en/9/9f/Bohemian_Rhapsody.png', progress: 50 },
];

const MediaContext = createContext();
export const useMedia = () => useContext(MediaContext);

export const MediaProvider = ({ children }) => {
    const [trackIndex, setTrackIndex] = useState(0);
    const [isPlaying, setIsPlaying] = useState(false);

    const nowPlaying = {
        ...mockTracks[trackIndex],
        isPlaying,
    };

    const play = () => {
        setIsPlaying(true);
        // When we play, send message to AR view
        const message = {
            iq_sender: 'console',
            type: 'iq-ar-update',
            payload: { type: 'media', ...nowPlaying, isPlaying: true }
        };
        window.parent.postMessage(message, '*');
    };
    const pause = () => setIsPlaying(false);
    const next = () => setTrackIndex((prev) => (prev + 1) % mockTracks.length);
    const prev = () => setTrackIndex((prev) => (prev - 1 + mockTracks.length) % mockTracks.length);

    const value = {
        nowPlaying,
        play,
        pause,
        next,
        prev,
    };

    return (
        <MediaContext.Provider value={value}>
            {children}
        </MediaContext.Provider>
    );
};
