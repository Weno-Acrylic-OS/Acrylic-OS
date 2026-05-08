# Weno Fit OS SDK - Notifications

This document explains how to send notifications from your Javascript app.

## Sending a Notification

To send a notification, you can use the `WenoFitOS.sendNotification` function.

### `WenoFitOS.sendNotification(appName, title, body)`

-   `appName` (String): The name of your application.
-   `title` (String): The title of the notification.
-   `body` (String): The body text of the notification.

Returns `true` if the notification was sent successfully, `false` otherwise (e.g., if the notification queue is full).

### Example

```javascript
// Example: Sending a notification from a "My Awesome App"
const appName = "My Awesome App";
const title = "Hello from JS!";
const body = "This is a notification sent from a third-party app.";

const success = WenoFitOS.sendNotification(appName, title, body);

if (success) {
    WenoFitOS.log("Notification sent successfully!");
} else {
    WenoFitOS.log("Failed to send notification.");
}
```
