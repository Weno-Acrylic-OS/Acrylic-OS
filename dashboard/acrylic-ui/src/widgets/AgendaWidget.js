import React from 'react';
import './AgendaWidget.css';
import { Calendar } from 'react-feather';

const mockAgenda = [
    { time: '10:00 AM', title: 'Team Standup' },
    { time: '2:00 PM', title: 'Project Deadline' },
];

const AgendaWidget = () => {
    return (
        <div className="widget agenda-widget">
            <div className="agenda-header">
                <Calendar size={16} />
                <h4>Today's Agenda</h4>
            </div>
            <div className="agenda-items">
                {mockAgenda.length > 0 ? (
                    mockAgenda.map((item, index) => (
                        <div key={index} className="agenda-item">
                            <span className="item-time">{item.time}</span>
                            <span className="item-title">{item.title}</span>
                        </div>
                    ))
                ) : (
                    <p className="no-events">No more events today.</p>
                )}
            </div>
        </div>
    );
};

export default AgendaWidget;
