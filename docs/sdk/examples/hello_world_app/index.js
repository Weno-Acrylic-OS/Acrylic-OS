// docs/sdk/examples/hello_world_app/index.js

// This is the entry point for your Weno Fit OS application.
// It will be executed by the Elk JavaScript engine.

// Access WenoFitOS system APIs
WenoFitOS.log("Hello, World! app started.");

// Access LVGL UI APIs
let screen = LVGL.createScreen();
let label = LVGL.createLabel(screen);
LVGL.setLabelText(label, "Hello, Weno Fit OS!");
LVGL.alignObject(label, LVGL.ALIGN_CENTER, 0, 0);

// You can also interact with other modules, e.g., Notifications.
// Notifications.send("HelloWorld", "Your first Weno Fit OS app is running!");
