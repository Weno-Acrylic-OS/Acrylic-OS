# Weno Fit OS SDK - Application Manifest (`app.json`)

Each Weno Fit OS application must include an `app.json` file in its root directory. This manifest provides essential metadata about the application to the operating system, including its identity, entry point, and required permissions.

## Structure

```json
{
  "name": "AppName",
  "version": "1.0.0",
  "author": "DeveloperName",
  "description": "A short description of the app.",
  "permissions": [
    "access_heart_rate_sensor",
    "send_notifications",
    "access_location"
  ],
  "entry_point": "index.js",
  "icon": "icon.png",
  "min_os_version": "0.1.0",
  "max_os_version": "1.0.0"
}
```

## Fields

*   **`name`** (string, required): The human-readable name of the application.
*   **`version`** (string, required): The version number of the application (e.g., "1.0.0").
*   **`author`** (string, required): The name of the developer or organization that created the app.
*   **`description`** (string, optional): A brief explanation of what the app does.
*   **`permissions`** (array of strings, optional): A list of permissions the app requires to function. The OS will use this to grant or deny access to sensitive resources.
    *   `access_heart_rate_sensor`: Allows access to the device's heart rate sensor.
    *   `send_notifications`: Allows the app to send notifications.
    *   `access_location`: Allows access to location data (GPS, if available).
    *   *(More permissions will be defined as the SDK evolves)*
*   **`entry_point`** (string, required): The path to the main JavaScript file that the OS will execute when the app is launched. This file should contain the app's initialization logic.
*   **`icon`** (string, optional): The path to the app's icon image (e.g., "icon.png"). This icon will be displayed in the app launcher.
*   **`min_os_version`** (string, optional): The minimum Weno Fit OS version required for this app to run.
*   **`max_os_version`** (string, optional): The maximum Weno Fit OS version compatible with this app.

## Example

```json
{
  "name": "MyFitnessTracker",
  "version": "1.2.0",
  "author": "Acme Corp.",
  "description": "Track your daily steps and heart rate.",
  "permissions": [
    "access_heart_rate_sensor"
  ],
  "entry_point": "app.js",
  "icon": "fitness_icon.png",
  "min_os_version": "0.1.0"
}
```
