/*
INTERACTING WITH WATCH:
- A button 'tap' is a quick press (a second or less).
- A button 'hold' is a long press (1.5 seconds or longer).
- Watch will usually start up in welcome mode.

Author: Brett McLain
Copyright MIT License 2017
*/

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
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
char       buttonHistory[8];
uint8_t    numButtonPresses = 0;
uint8_t    messageIndex = 0;

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
    numButtonPresses = 0;
    memset(&buttonHistory[0], 0, sizeof(buttonHistory));
  } else if (a == ACTION_HOLD_RIGHT && numButtonPresses < 5) {
    buttonHistory[numButtonPresses] = 'r';
    numButtonPresses++;
  } else if (a == ACTION_HOLD_LEFT && numButtonPresses < 5) {
    buttonHistory[numButtonPresses] = 'l';
    numButtonPresses++;
  }
  uint8_t oldMessageIndex = messageIndex;
  if (strcmp(buttonHistory, "llll") == 0) {
    messageIndex = 1;
  } else if (strcmp(buttonHistory, "rrrr") == 0) {
    messageIndex = 2;
  } else if (strcmp(buttonHistory, "rrrl") == 0) {
    messageIndex = 3;
  } else if (strcmp(buttonHistory, "rrll") == 0) {
    messageIndex = 4;
  } else if (strcmp(buttonHistory, "rlll") == 0) {
    messageIndex = 5;
  } else if (strcmp(buttonHistory, "lllr") == 0) {
    messageIndex = 6;
  } else if (strcmp(buttonHistory, "llrr") == 0) {
    messageIndex = 7;
  } else if (strcmp(buttonHistory, "lrrr") == 0) {
    messageIndex = 8;
  } else if (strcmp(buttonHistory, "lrlr") == 0) {
    messageIndex = 9;
  } else if (strcmp(buttonHistory, "rlrl") == 0) {
    messageIndex = 10;
  } else if (strcmp(buttonHistory, "lrrl") == 0) {
    messageIndex = 11;
  } else if (strcmp(buttonHistory, "rllr") == 0) {
    messageIndex = 12;
  } else if (strcmp(buttonHistory, "rrlr") == 0) {
    messageIndex = 0;
  }
  if (oldMessageIndex != messageIndex) {
    numButtonPresses = 0;
    memset(&buttonHistory[0], 0, sizeof(buttonHistory));
  }
  (*modeFunc[mode])(a); // Action is passed to clock-drawing function
  watch.swapBuffers();
}

