mergeInto(LibraryManager.library, {
  display_init: function() {
    console.log("display_init called");
  },
  display_clear: function(color) {
    var ctx = Module.canvas.getContext('2d');
    var css_color = "#" + ("000000" + color.toString(16)).slice(-6);
    ctx.fillStyle = css_color;
    ctx.fillRect(0, 0, Module.canvas.width, Module.canvas.height);
  },
  display_draw_pixel: function(x, y, color) {
    var ctx = Module.canvas.getContext('2d');
    var css_color = "#" + ("000000" + color.toString(16)).slice(-6);
    ctx.fillStyle = css_color;
    ctx.fillRect(x, y, 1, 1);
  },
  display_update: function() {
    // No-op
  },
  heart_rate_get_bpm: function() {
    return 70 + (Math.random() * 20);
  },
  touch_get_event: function() {
    // 0 = NONE, 1 = SWIPE_LEFT, 2 = SWIPE_RIGHT
    var random_event = Math.random() * 100;
    if (random_event < 5) {
        return 1;
    } else if (random_event < 10) {
        return 2;
    }
    return 0;
  }
});
