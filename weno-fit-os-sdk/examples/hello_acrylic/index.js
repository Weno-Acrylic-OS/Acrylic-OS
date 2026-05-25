// Hello, Acrylic App for Acrylic OS

import Acrylic from '../../api/acrylic.js';

let count = 0;

function App() {
    return Acrylic.createElement(
        'View', 
        { style: { padding: 20, backgroundColor: '#f0f0f0', height: '100%' } },
        Acrylic.createElement('Text', { style: { fontSize: 24, marginBottom: 10 } }, `Hello, Acrylic!`),
        Acrylic.createElement('Text', null, `You clicked ${count} times`),
        Acrylic.createElement(
            'Button',
            {
                title: 'Click Me',
                onClick: () => {
                    count++;
                    // Re-render the app
                    Acrylic.render(App());
                },
            },
        )
    );
}

Acrylic.render(App());
