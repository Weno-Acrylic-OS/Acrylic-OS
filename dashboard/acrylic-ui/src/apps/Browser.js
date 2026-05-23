import React, { useState } from 'react';
import './Browser.css';

const Browser = () => {
    const [url, setUrl] = useState('https://www.google.com/webhp?igu=1'); // igu=1 is a trick to allow embedding Google
    const [inputValue, setInputValue] = useState(url);

    const handleGo = () => {
        let finalUrl = inputValue;
        if (!/^https?:\/\//i.test(finalUrl)) {
            finalUrl = 'https://' + finalUrl;
        }
        setUrl(finalUrl);
    };

    const handleKeyPress = (e) => {
        if (e.key === 'Enter') {
            handleGo();
        }
    };

    return (
        <div className="browser">
            <div className="browser-toolbar">
                <input 
                    type="text" 
                    value={inputValue} 
                    onChange={(e) => setInputValue(e.target.value)}
                    onKeyPress={handleKeyPress}
                />
                <button onClick={handleGo}>Go</button>
            </div>
            <iframe 
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
