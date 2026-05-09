// index.js for Notification Example

WenoFitOS.log("Notification Example app started.");

// Create a simple UI
let screen = LVGL.createScreen();

function sendNotification() {
    WenoFitOS.log("Button clicked! Sending notification...");
    const appName = "Notification Example";
    const title = "Button Pressed!";
    const body = "This notification was sent from a button click.";
    WenoFitOS.sendNotification(appName, title, body);
}

// Create a button with a label
let btn = LVGL.createButton(screen, "Send Notification");
LVGL.alignObject(btn, LVGL.ALIGN_CENTER, 0, 0);

// Register a click event handler
LVGL.onClick(btn, sendNotification);
