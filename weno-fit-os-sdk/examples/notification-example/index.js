// index.js for Notification Example

// Log to the console that the app has started
WenoFitOS.log("Notification Example app started.");

// Create a simple UI
let screen = LVGL.createScreen();
let label = LVGL.createLabel(screen);
LVGL.setLabelText(label, "Sending a notification...");
LVGL.alignObject(label, LVGL.ALIGN_CENTER, 0, 0);

// Send a notification
// This demonstrates how a third-party app can use the notification service.
const appName = "Notification Example";
const title = "Hello from an App!";
const body = "This is a notification from a third-party Javascript app.";

const success = WenoFitOS.sendNotification(appName, title, body);

if (success) {
    WenoFitOS.log("Notification sent successfully!");
    LVGL.setLabelText(label, "Notification sent!");
} else {
    WenoFitOS.log("Failed to send notification.");
    LVGL.setLabelText(label, "Failed to send notification.");
}

// NOTE: The current Weno Fit OS Javascript SDK is very limited and
// does not expose APIs for creating buttons or handling click events.
// A more realistic app would send a notification based on some event or user interaction.
