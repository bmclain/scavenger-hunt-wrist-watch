/*
INTERACTING WITH WATCH:
- A button 'tap' is a quick press (a second or less).
- A button 'hold' is a long press (1.5 seconds or longer).
- Watch will usually start up in time-setting mode.

TIME SET MODE:
- Tap right button to increase value of current digit.
- Tap left button to advance to next digit.
- Hold both buttons to switch to time display mode.

TIME DISPLAY MODE:
- Hold left or right button to switch forward/back between clocks.
- Some (but not all) clocks may use left or right button tap to
  switch display format (e.g. date vs time).
- Hold both buttons to switch to time set mode.
*/

#include <avr/sleep.h>
#include <avr/power.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Watch.h>

#define BATT_LOW_MV  2881 // Use reduced display below this voltage

#define MODE_SCAVENGER  0
#define MODE_BATTERY 1

void mode_set(uint8_t action);
void mode_battery(uint8_t action);
void mode_scavenger(uint8_t action);

void (*modeFunc[])(uint8_t) = {
  mode_scavenger,
  mode_battery
};
#define N_MODES (sizeof(modeFunc) / sizeof(modeFunc[0]))

// Used by various display modes for smooth fade-out before sleep
const uint8_t PROGMEM
 fade[] =
  {  0,  1,  1,  2,  4,  5,  8, 10, 13, 17, 22, 27, 32, 39, 46,
    53, 62, 71, 82, 93,105,117,131,146,161,178,196,214,234,255 };

Watch      watch(2, LED_PLEX_1, true);
uint8_t    mode      = MODE_SCAVENGER,
           mode_last = MODE_SCAVENGER;
boolean    h24       = false; // 24-hour display mode
uint16_t   fps       = 100;

void setup() {
  Wire.begin();
  mode = MODE_SCAVENGER;
  watch.begin();
  // On powerup and following initialization, watch is immediately put
  // to sleep.  When inserting a new battery, nothing will happen until
  // a button is pressed.  This is to prevent a "death spiral" loop where
  // the watch battery runs low and a brownout restart occurs -- starting
  // the LED matrix, hitting another brownout, ad batterylowum.
  // Immediate inactivity allows the battery a chance to recover and
  // provides a little extra run time, at the expense of needing an
  // initial button press to see the new battery in action.
  watch.setTimeout(0); // sleep
  
}

void loop() {
  uint8_t a = watch.action();
  if(a == ACTION_HOLD_BOTH) {
    if(mode == MODE_SCAVENGER) {
      // Exit time setting, return to last used display mode
      mode = mode_last;
    } else {
      // Save current display mode, switch to time setting
      mode_last = mode;
      mode      = MODE_SCAVENGER;
    }
  } else if(a == ACTION_HOLD_RIGHT) {
    if(mode != MODE_SCAVENGER) {
      // Switch to next display mode (w/wrap)
      if(++mode >= N_MODES) mode = 1;
    }
  } else if(a == ACTION_HOLD_LEFT) {
    if(mode != MODE_SCAVENGER) {
      // Switch to prior display mode (w/wrap)
      if(--mode < 1) mode = N_MODES - 1;
    }
  }
  (*modeFunc[mode])(a); // Action is passed to clock-drawing function
  watch.swapBuffers();
}

