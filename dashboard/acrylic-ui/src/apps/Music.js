import React, { useState, useEffect } from 'react';
import './Music.css';
import { Play, Pause, SkipBack, SkipForward } from 'react-feather';

const mockLibrary = [
    {
        album: 'Acoustic Dreams',
        artist: 'Orion',
        art: 'https://images.unsplash.com/photo-1619983081593-e22f5899fa24?w=500',
        tracks: ['Morning Dew', 'Sunset Drive', 'Midnight Wanderer'],
    },
    {
        album: 'Neon City',
        artist: 'Cyber Runner',
        art: 'https://images.unsplash.com/photo-1511379938547-c1f69419868d?w=500',
        tracks: ['First Light', 'Downtown Rush', 'Last Goodbye'],
    },
    {
        album: 'Ocean Breath',
        import React, { useState, useEffect, useCallback } from 'react';
        import './Music.css';
        import { Play, Pause, SkipBack, SkipForward } from 'react-feather';

        // ... (mockLibrary const is unchanged) ...

        const Music = () => {
            const [library] = useState(mockLibrary);
            const [currentTrack, setCurrentTrack] = useState({ albumIndex: 0, trackIndex: 0 });
            const [isPlaying, setIsPlaying] = useState(false);
            const [progress, setProgress] = useState(0);

            const activeAlbum = library[currentTrack.albumIndex];
            const activeTrackName = activeAlbum.tracks[currentTrack.trackIndex];

            const handleNext = useCallback(() => {
                let { albumIndex, trackIndex } = currentTrack;
                if (trackIndex < library[albumIndex].tracks.length - 1) {
                    trackIndex++;
                } else {
                    albumIndex = (albumIndex + 1) % library.length;
                    trackIndex = 0;
                }
                setCurrentTrack({ albumIndex, trackIndex });
                setProgress(0);
            }, [currentTrack, library]);

            useEffect(() => {
                let interval;
                if (isPlaying) {
                    interval = setInterval(() => {
                        setProgress(p => {
                            if (p >= 100) {
                                handleNext();
                                return 0;
                            }
                            return p + 1; // Simulate 100-second songs
                        });
                    }, 1000);
                }
                return () => clearInterval(interval);
            }, [isPlaying, currentTrack, handleNext]);

            const handlePlayPause = () => {
                setIsPlaying(!isPlaying);
            };

            const handlePrev = () => {
        let { albumIndex, trackIndex } = currentTrack;
        if (progress > 3 || trackIndex === 0) {
            // Go to beginning of track or previous track
        } else {
            trackIndex--;
        }
        
        if (trackIndex < 0) {
             albumIndex = (albumIndex - 1 + library.length) % library.length;
             trackIndex = library[albumIndex].tracks.length - 1;
        }
        setCurrentTrack({ albumIndex, trackIndex });
        setProgress(0);
    };

    const selectTrack = (albumIndex, trackIndex) => {
        setCurrentTrack({ albumIndex, trackIndex });
        setProgress(0);
        setIsPlaying(true);
    };

    return (
        <div className="music-app">
            <div className="music-sidebar">
                <div className="now-playing">
                    <img src={activeAlbum.art} alt={activeAlbum.album} className="now-playing-art" />
                    <h3 className="now-playing-title">{activeTrackName}</h3>
                    <p className="now-playing-artist">{activeAlbum.artist}</p>
                </div>
                <div className="playback-controls">
                    <div className="progress-bar-container">
                        <div className="progress-bar" style={{ width: `${progress}%` }}></div>
                    </div>
                    <div className="control-buttons">
                        <button onClick={handlePrev}><SkipBack size={24} /></button>
                        <button className="play-pause-btn" onClick={handlePlayPause}>
                            {isPlaying ? <Pause size={32} /> : <Play size={32} />}
                        </button>
                        <button onClick={handleNext}><SkipForward size={24} /></button>
                    </div>
                </div>
            </div>
            <div className="music-main-content">
                <div className="music-header">
                    <h1>Library</h1>
                </div>
                <div className="album-grid">
                    {library.map((album, albumIndex) => (
                        <div key={album.album} className="album-card" onClick={() => selectTrack(albumIndex, 0)}>
                            <img src={album.art} alt={album.album} />
                            <div className="album-info">
                                <p className="album-title">{album.album}</p>
                                <p className="album-artist">{album.artist}</p>
                            </div>
                        </div>
                    ))}
                </div>
            </div>
        </div>
    );
};

export default Music;

