import React, { createContext, useState, useContext } from 'react';

// --- Available Widgets Registry ---
// In a real app, this might be dynamically generated
import WeatherWidget from '../widgets/WeatherWidget';
import AgendaWidget from '../widgets/AgendaWidget';

export const WIDGET_REGISTRY = {
    'Weather': WeatherWidget,
    'Agenda': AgendaWidget,
};

// --- Initial Layout ---
const initialLayouts = {
    desktop: ['Weather', 'Agenda'],
    phone: ['Weather', 'Agenda'],
    smarthome: [], // No widgets on smarthome for now
};

// --- Context Definition ---
const WidgetLayoutContext = createContext();
export const useWidgetLayout = () => useContext(WidgetLayoutContext);


// --- Provider Component ---
export const WidgetLayoutProvider = ({ children }) => {
    const [layouts, setLayouts] = useState(initialLayouts);

    const getWidgetsFor = (personality) => {
        return layouts[personality] || [];
    };

    const addWidget = (personality, widgetName) => {
        setLayouts(prevLayouts => {
            const currentWidgets = prevLayouts[personality] || [];
            if (currentWidgets.includes(widgetName)) {
                return prevLayouts; // Don't add duplicates
            }
            return {
                ...prevLayouts,
                [personality]: [...currentWidgets, widgetName],
            };
        });
    };

    const removeWidget = (personality, widgetIndex) => {
        setLayouts(prevLayouts => {
            const currentWidgets = prevLayouts[personality] || [];
            const newWidgets = [...currentWidgets];
            newWidgets.splice(widgetIndex, 1);
            return {
                ...prevLayouts,
                [personality]: newWidgets,
            };
        });
    };

    const moveWidget = (personality, fromIndex, toIndex) => {
        setLayouts(prevLayouts => {
            const currentWidgets = prevLayouts[personality] || [];
            if (fromIndex < 0 || fromIndex >= currentWidgets.length || toIndex < 0 || toIndex >= currentWidgets.length) {
                return prevLayouts; // Invalid index
            }
            const newWidgets = [...currentWidgets];
            const [item] = newWidgets.splice(fromIndex, 1);
            newWidgets.splice(toIndex, 0, item);
            return {
                ...prevLayouts,
                [personality]: newWidgets,
            };
        });
    };

    const value = {
        layouts,
        getWidgetsFor,
        addWidget,
        removeWidget,
        moveWidget,
    };

    return (
        <WidgetLayoutContext.Provider value={value}>
            {children}
        </WidgetLayoutContext.Provider>
    );
};
