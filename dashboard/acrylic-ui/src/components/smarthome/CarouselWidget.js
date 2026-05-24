import React, { useState, useEffect } from 'react';
import './CarouselWidget.css';

const slides = [
    { type: 'Weather', content: '☀️ 72°F & Sunny' },
    { type: 'Suggestion', content: 'Turn on the Living Room lights?' },
    { type: 'Calendar', content: 'Team Meeting at 3:00 PM' },
];

const CarouselWidget = () => {
    const [currentSlide, setCurrentSlide] = useState(0);

    useEffect(() => {
        const slideInterval = setInterval(() => {
            setCurrentSlide(prev => (prev + 1) % slides.length);
        }, 5000); // Change slide every 5 seconds
        return () => clearInterval(slideInterval);
    }, []);

    return (
        <div className="carousel-widget">
            <div className="carousel-inner" style={{ transform: `translateX(-${currentSlide * 100}%)` }}>
                {slides.map((slide, index) => (
                    <div className="carousel-slide" key={index}>
                        <h3>{slide.type}</h3>
                        <p>{slide.content}</p>
                    </div>
                ))}
            </div>
            <div className="carousel-dots">
                {slides.map((_, index) => (
                    <span 
                        key={index} 
                        className={`dot ${currentSlide === index ? 'active' : ''}`}
                        onClick={() => setCurrentSlide(index)}
                    ></span>
                ))}
            </div>
        </div>
    );
};

export default CarouselWidget;
