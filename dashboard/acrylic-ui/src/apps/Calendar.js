import React, { useState } from 'react';
import './Calendar.css';
import { ChevronLeft, ChevronRight } from 'react-feather';

// Mock Events Data
const mockEvents = {
    '2026-07-10': [{ time: '10:00 AM', title: 'Team Standup' }],
    '2026-07-15': [
        { time: '2:00 PM', title: 'Project Deadline' },
        { time: '6:00 PM', title: 'Design Review' }
    ],
    '2026-08-01': [{ time: '1:00 PM', title: 'Lunch with Jane' }],
};

const CalendarApp = () => {
    const [currentDate, setCurrentDate] = useState(new Date(2026, 6, 1)); // Start in July 2026 for demo
    const [selectedDate, setSelectedDate] = useState(new Date(2026, 6, 1));

    const changeMonth = (offset) => {
        setCurrentDate(new Date(currentDate.setMonth(currentDate.getMonth() + offset)));
    };

    const generateCalendarGrid = () => {
        const year = currentDate.getFullYear();
        const month = currentDate.getMonth();
        const firstDayOfMonth = new Date(year, month, 1).getDay();
        const daysInMonth = new Date(year, month + 1, 0).getDate();

        const grid = [];
        let dayCounter = 1;

        // 6 rows for a standard calendar view
        for (let i = 0; i < 6; i++) {
            const week = [];
            for (let j = 0; j < 7; j++) {
                if ((i === 0 && j < firstDayOfMonth) || dayCounter > daysInMonth) {
                    week.push({ day: '', isPadding: true });
                } else {
                    const date = new Date(year, month, dayCounter);
                    const dateString = date.toISOString().split('T')[0];
                    week.push({ 
                        day: dayCounter, 
                        date,
                        isToday: new Date().toDateString() === date.toDateString(),
                        hasEvents: !!mockEvents[dateString]
                    });
                    dayCounter++;
                }
            }
            grid.push(week);
        }
        return grid;
    };

    const calendarGrid = generateCalendarGrid();
    const selectedDateString = selectedDate.toISOString().split('T')[0];
    const eventsForSelectedDay = mockEvents[selectedDateString] || [];

    return (
        <div className="calendar-app">
            <div className="calendar-main">
                <div className="calendar-header">
                    <h2>{currentDate.toLocaleString('default', { month: 'long', year: 'numeric' })}</h2>
                    <div className="calendar-nav">
                        <button onClick={() => changeMonth(-1)}><ChevronLeft size={20} /></button>
                        <button onClick={() => changeMonth(1)}><ChevronRight size={20} /></button>
                    </div>
                </div>
                <div className="calendar-grid">
                    <div className="day-names">
                        {['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'].map(day => <div key={day}>{day}</div>)}
                    </div>
                    <div className="days-grid">
                        {calendarGrid.flat().map((dayInfo, index) => (
                            <div 
                                key={index} 
                                className={`day-cell ${dayInfo.isPadding ? 'padding' : ''} ${dayInfo.isToday ? 'today' : ''} ${selectedDate.toDateString() === dayInfo.date?.toDateString() ? 'selected' : ''}`}
                                onClick={() => !dayInfo.isPadding && setSelectedDate(dayInfo.date)}
                            >
                                {dayInfo.day}
                                {dayInfo.hasEvents && <div className="event-dot"></div>}
                            </div>
                        ))}
                    </div>
                </div>
            </div>
            <div className="event-panel">
                <div className="event-panel-header">
                    <h3>{selectedDate.toLocaleDateString('default', { weekday: 'long', month: 'long', day: 'numeric' })}</h3>
                </div>
                <div className="event-list">
                    {eventsForSelectedDay.length > 0 ? (
                        eventsForSelectedDay.map((event, index) => (
                            <div key={index} className="event-item">
                                <div className="event-time">{event.time}</div>
                                <div className="event-title">{event.title}</div>
                            </div>
                        ))
                    ) : (
                        <p className="no-events">No events scheduled.</p>
                    )}
                </div>
            </div>
        </div>
    );
};

export default CalendarApp;
