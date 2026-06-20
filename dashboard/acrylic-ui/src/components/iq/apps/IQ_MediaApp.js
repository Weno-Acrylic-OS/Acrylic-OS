// dashboard/acrylic-ui/src/components/iq/apps/IQ_MediaApp.js
import React from 'react';
import '../IQ.css';
import { useMedia } from './MediaContext';
import { Play, Pause, SkipBack, SkipForward } from 'react-feather';

const IQ_MediaApp = () => {
    const { nowPlaying, play, pause, next, prev } = useMedia();

    const handlePlayPause = () => {
        if (nowPlaying.isPlaying) {
            pause();
        } else {
            play();
        }
    };

    return (
        <div className="iq-app iq-media-app">
            <div className="album-art-large">
                <img src={nowPlaying.albumArtUrl} alt={nowPlaying.title} />
            </div>
            <div className="track-details">
                <h1 className="track-title-large">{nowPlaying.title}</h1>
                <p className="track-artist-large">{nowPlaying.artist}</p>
            </div>
            <div className="progress-bar-container">
                <div className="progress-bar" style={{ width: `${nowPlaying.progress}%` }}></div>
            </div>
            <div className="media-controls-large">
                <button onClick={prev}><SkipBack size={32} /></button>
                <button className="play-btn-large" onClick={handlePlayPause}>
                    {nowPlaying.isPlaying ? <Pause size={40} /> : <Play size={40} />}
                </button>
                <button onClick={next}><SkipForward size={32} /></button>
            </div>
        </div>
    );
};

export default IQ_MediaApp;
