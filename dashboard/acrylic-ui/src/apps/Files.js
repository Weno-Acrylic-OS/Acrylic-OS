import React, { useState } from 'react';
import './Files.css';

// Mock file system data
const fileSystem = {
    'Home': [
        { name: 'Documents', type: 'folder' },
        { name: 'Downloads', type: 'folder' },
        { name: 'Pictures', type: 'folder' },
        { name: 'report.pdf', type: 'file' },
    ],
    'Documents': [
        { name: 'work', type: 'folder' },
        { name: 'project-notes.txt', type: 'file' },
    ],
    'Downloads': [
        { name: 'weno-fit-os-sdk.zip', type: 'file' },
    ],
    'Pictures': [
        { name: 'vacation.jpg', type: 'file' },
    ],
    'work': [
        { name: 'quarterly-review.docx', type: 'file' },
    ]
};

const Files = () => {
    const [currentPath, setCurrentPath] = useState('Home');
    const [history, setHistory] = useState(['Home']);

    const openItem = (item) => {
        if (item.type === 'folder') {
            const newPath = item.name;
            // A real implementation would handle nested paths properly
            // For this mock, we just use the name as the key.
            if (fileSystem[newPath]) {
                setHistory([...history, newPath]);
                setCurrentPath(newPath);
            }
        }
        // else: open file logic would go here
    };

    const goBack = () => {
        if (history.length > 1) {
            const newHistory = [...history];
            newHistory.pop();
            setHistory(newHistory);
            setCurrentPath(newHistory[newHistory.length - 1]);
        }
    };

    const currentFiles = fileSystem[currentPath] || [];

    return (
        <div className="files-app">
            <div className="files-toolbar">
                <button onClick={goBack} disabled={history.length <= 1}>↑ Up</button>
                <div className="path-display">/{currentPath}</div>
            </div>
            <div className="files-list">
                {currentFiles.map((item, index) => (
                    <div key={index} className="file-item" onDoubleClick={() => openItem(item)}>
                        <span className="file-icon">{item.type === 'folder' ? '📁' : '📄'}</span>
                        <span className="file-name">{item.name}</span>
                    </div>
                ))}
            </div>
        </div>
    );
};

export default Files;
