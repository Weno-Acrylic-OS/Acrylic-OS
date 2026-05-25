// Hello, Multi-Device App for Acrylic OS

import { log } from '../../api/system.js';
import { getFormFactor } from '../../api/device.js';
import { createElement, render, showNotification } from '../../api/ui.js';

log('Hello, Multi-Device app started.');

const platform = getFormFactor();

const root = createElement('container', {
    children: [
        createElement('label', {
            text: `Hello, ${platform}!`
        }),
        createElement('button', {
            text: 'Say Hi',
            onClick: () => {
                showNotification('Hello!', `This notification is from the ${platform} app.`);
            }
        })
    ]
});

render(root);
