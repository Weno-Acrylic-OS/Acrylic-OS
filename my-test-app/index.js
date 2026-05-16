// Phone Test App
WenoFitOS.log("Phone Test App started.");

if (WenoFitOS.isPhoneConnected()) {
    let screen = LVGL.createScreen();
    let label = LVGL.createLabel(screen);
    LVGL.setLabelText(label, "Phone is connected!");
    LVGL.alignObject(label, LVGL.ALIGN_CENTER, 0, 0);
} else {
    WenoFitOS.showPhoneRequiredScreen();
}

