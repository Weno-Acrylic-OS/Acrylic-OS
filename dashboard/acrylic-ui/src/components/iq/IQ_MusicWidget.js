// dashboard/acrylic-ui/src/components/iq/IQ_MusicWidget.js
import React from 'react';
import './IQ.css';
import { useMedia } from './apps/MediaContext';
import { Rewind, Play, Pause, FastForward, Volume2 } from 'react-feather';

const IQ_MusicWidget = () => {
    const { nowPlaying, play, pause } = useMedia();

    const handlePlayPause = () => {
        if (nowPlaying.isPlaying) {
            pause();
        } else {
            play();
        }
    };

    if (!nowPlaying) return null;

    return (
        <div className="iq-music-widget">
            <div className="track-info">
                <p className="track-title">{nowPlaying.title}</p>
                <p className="track-artist">{nowPlaying.artist}</p>
            </div>
            <div className="playback-controls">
                <button><Rewind size={20} /></button>
                <button className="play-btn" onClick={handlePlayPause}>
                    {nowPlaying.isPlaying ? <Pause size={24} /> : <Play size={24} />}
                </button>
                <button><FastForward size={20} /></button>
            </div>
            <div className="volume-control">
                <Volume2 size={20} />
            </div>
        </div>
    );
};

export default IQ_MusicWidget;
