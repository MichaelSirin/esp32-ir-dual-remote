'use strict';

async function sendButtonPress(id, display) {
    console.log('Button:', id);

    const brand_name = document.getElementById('tvType').value;
    const channel= document.getElementById('irChannel').value;

    const disp = document.getElementById('display');
    if (!disp) {
        console.error('Display element not found');
        return;
    }
    
    disp.textContent = display;
    setTimeout(() => { disp.textContent = ''; }, 500);

    try {
        const resp = await fetch(`http://${location.host}/api`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ channel: channel, brand: brand_name, cmd: id }),
        });
        if (!resp.ok) {
            console.error('Server error:', resp.status, await resp.text());
            disp.textContent = `Error: ${resp.status}`;
        }
    } catch (err) {
        console.error('Network error:', err);
        disp.textContent = 'Network error';
    }
}
