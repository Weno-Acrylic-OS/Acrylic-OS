import React, { useState, useRef, useEffect } from 'react';
import './Browser.css';
import TextInput from '../components/common/TextInput';

const Browser = () => {
    const initialUrl = 'https://www.google.com/webhp?igu=1';
    const [history, setHistory] = useState([initialUrl]);
    const [historyIndex, setHistoryIndex] = useState(0);
    const [inputValue, setInputValue] = useState(initialUrl);
    const iframeRef = useRef(null);

    const url = history[historyIndex];

    // Update input value when navigating with back/forward buttons
    useEffect(() => {
        setInputValue(url);
    }, [url]);

    const navigate = (newUrl, isNewEntry = true) => {
        let finalUrl = newUrl;
        if (isNewEntry && !/^https?:\/\//i.test(finalUrl)) {
            finalUrl = 'https://' + finalUrl;
        }

        if (isNewEntry) {
            // If we are navigating to a new page, not via back/forward
            const newHistory = history.slice(0, historyIndex + 1);
            newHistory.push(finalUrl);
            setHistory(newHistory);
            setHistoryIndex(newHistory.length - 1);
        } else {
            // This is just for rendering, the index is already set
        }
        setInputValue(finalUrl);
    };

    const handleGo = () => {
        navigate(inputValue);
    };
    
    const handleBack = () => {
        if (historyIndex > 0) {
            setHistoryIndex(historyIndex - 1);
        }
    };

    const handleForward = () => {
        if (historyIndex < history.length - 1) {
            setHistoryIndex(historyIndex + 1);
        }
    };

    const handleReload = () => {
        if (iframeRef.current) {
            iframeRef.current.src += ''; // A common way to force a reload
        }
    };

    const handleKeyPress = (e) => {
        if (e.key === 'Enter') {
            handleGo();
        }
    };

    return (
        <div className="browser">
            <div className="browser-toolbar">
                <button onClick={handleBack} disabled={historyIndex === 0}>{'<'}</button>
                <button onClick={handleForward} disabled={historyIndex === history.length - 1}>{'>'}</button>
                <button onClick={handleReload}>{'↻'}</button>
                <TextInput 
                    type="text" 
                    value={inputValue} 
                    onChange={(e) => setInputValue(e.target.value)}
                    onKeyPress={handleKeyPress}
                    placeholder="Enter address..."
                />
                <button onClick={handleGo}>Go</button>
            </div>
            <iframe 
                ref={iframeRef}
                src={url} 
                title="Acrylic Browser"
                sandbox="allow-forms allow-modals allow-pointer-lock allow-popups allow-presentation allow-same-origin allow-scripts"
                referrerPolicy="no-referrer"
            ></iframe>
            <div className="browser-notice">
                <p>Note: Many websites will block being loaded in an iframe for security reasons.</p>
            </div>
        </div>
    );
};

export default Browser;
