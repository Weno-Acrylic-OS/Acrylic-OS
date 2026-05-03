WenoFitOS.log("JS Widget loading...");

function createMyWidget(parentAddr) {
  WenoFitOS.log("Creating JS-Card widget on parent: " + parentAddr);

  // Create a card object that will be our widget's container
  let cardAddr = LVGL.createObject(parentAddr);

  // Style the card to look like a native widget
  // A negative value for width/height signifies percentage, e.g. -100 means 100%
  LVGL.setSize(cardAddr, -100, 75); // 100% width, 75px height
  LVGL.setBgColor(cardAddr, 0x282828); // Dark grey card color
  LVGL.setRadius(cardAddr, 12);
  LVGL.setBorderWidth(cardAddr, 0);
  LVGL.setPadAll(cardAddr, 10);
  LVGL.setFlexFlow(cardAddr, LVGL.FLEX_FLOW_COLUMN); // Arrange children vertically

  // Create a title label inside the card
  let titleAddr = LVGL.createLabel(cardAddr);
  LVGL.setLabelText(titleAddr, "JS Widget");
  LVGL.setTextColor(titleAddr, 0xFFFFFF); // White color for title

  // Create a data label inside the card
  let dataAddr = LVGL.createLabel(cardAddr);
  let steps = WenoFitOS.getSteps();
  LVGL.setLabelText(dataAddr, "Steps today: " + steps);
  LVGL.setTextColor(dataAddr, 0xE0E0E0); // Light grey for body text
}

WenoFitOS.registerWidget("JS-Card", createMyWidget);

WenoFitOS.log("JS Widget loaded.");
