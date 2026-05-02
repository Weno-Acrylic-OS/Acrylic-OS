// index.js
WenoFitOS.log("Hello, World! app started.");

let screen = LVGL.createScreen();
let label = LVGL.createLabel(screen);
LVGL.setLabelText(label, "Hello, Weno Fit OS!");
LVGL.alignObject(label, LVGL.ALIGN_CENTER, 0, 0);
