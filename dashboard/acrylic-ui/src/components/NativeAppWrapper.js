import React from 'react';

// --- Component Map ---
// Maps the virtual element types from the Acrylic SDK to actual React components.
const componentMap = {
    'View': (props) => <div {...props}>{props.children}</div>,
    'Text': (props) => <p {...props}>{props.children}</p>,
    'Button': (props) => <button onClick={props.onClick} style={props.style}>{props.title || props.children}</button>,
    // Add other component mappings here
};

/**
 * Recursively renders a UI tree from the Acrylic SDK into React components.
 * @param {object} element - A virtual element from the UI tree.
 * @returns {React.Component|null} A React component or null.
 */
const renderAppFromTree = (element) => {
    if (!element || !element.type) {
        return null;
    }

    const Component = componentMap[element.type];
    if (!Component) {
        console.error(`Unsupported component type: ${element.type}`);
        return <div style={{ border: '1px solid red', padding: '5px' }}>Unsupported: {element.type}</div>;
    }
    
    const { children, ...props } = element.props;

    return (
        <Component {...props}>
            {children && children.map((child, index) => {
                if (typeof child === 'string' || typeof child === 'number') {
                    return child;
                }
                return React.cloneElement(renderAppFromTree(child), { key: index });
            })}
        </Component>
    );
};

const NativeAppWrapper = ({ uiTree }) => {
    if (!uiTree) {
        return <div>Loading app...</div>;
    }

    return (
        <div className="native-app-wrapper">
            {renderAppFromTree(uiTree)}
        </div>
    );
};

export default NativeAppWrapper;
