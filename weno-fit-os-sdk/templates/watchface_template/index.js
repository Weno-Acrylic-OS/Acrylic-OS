WenoFitOS.log("Live Watchface template loading...");

// This watchface demonstrates how to use the real-time data API and timers.

// We need a global variable to hold the address of our time label
let timeLabelAddr = 0;

// This function will be called every second by the timer we create.
function updateTime() {
  if (timeLabelAddr === 0) return;

  // Get the current time from the WenoFitOS API
  let hours = WenoFitOS.getHours();
  let minutes = WenoFitOS.getMinutes();
  let seconds = WenoFitOS.getSeconds();

  // Pad with leading zeros
  if (hours < 10) { hours = "0" + hours; }
  if (minutes < 10) { minutes = "0" + minutes; }
  if (seconds < 10) { seconds = "0" + seconds; }

  // Format the time string and update the label
  let timeString = hours + ":" + minutes + ":" + seconds;
  LVGL.setLabelText(timeLabelAddr, timeString);
}

// This function is called by the OS when the watchface needs to be created.
function createLiveWatchface(parentAddr) {
  WenoFitOS.log("Creating Live Watchface on parent: " + parentAddr);

  // Create a label for the time and store its address
  timeLabelAddr = LVGL.createLabel(parentAddr);
  LVGL.setLabelText(timeLabelAddr, "00:00:00");
  LVGL.alignObject(timeLabelAddr, LVGL.ALIGN_CENTER, 0, 0);

  // Create a timer that calls our updateTime function every 1000ms (1 second)
  WenoFitOS.createTimer(updateTime, 1000);
}

// Register our watchface with the system
WenoFitOS.registerWatchface("JS-Live", createLiveWatchface);

WenoFitOS.log("Live Watchface template loaded.");
