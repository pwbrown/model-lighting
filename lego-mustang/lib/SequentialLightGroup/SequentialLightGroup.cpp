#include "SequentialLightGroup.h"

// Initialize Sequential lights with dimmable lights and blinking interval
// settings
SequentialLightGroup::SequentialLightGroup(Light &first, Light &second,
                                           Light &third)
    : _first{first}, _second{second}, _third{third} {}

// Configures all three lights
void SequentialLightGroup::configure(void) {
  _first.configure();
  _second.configure();
  _third.configure();
}

// Turns lights on to a specific brightness
void SequentialLightGroup::on(int brightness, bool stopEffects) {
  if (stopEffects) {
    _isRunning = false;
  }
  _first.on(brightness, stopEffects);
  _second.on(brightness, stopEffects);
  _third.on(brightness, stopEffects);
}

// Turn off lights
void SequentialLightGroup::off(bool stopEffects) {
  if (stopEffects) {
    _isRunning = false;
  }
  _first.off(stopEffects);
  _second.off(stopEffects);
  _third.off(stopEffects);
}

// Starts the blink effect for each light
void SequentialLightGroup::blink(int intervalInMs, int highBrightness,
                                 int lowBrightness) {
  // Stop any other active effects
  _isRunning = false;
  // Start blink on each light
  _first.blink(intervalInMs, highBrightness, lowBrightness);
  _second.blink(intervalInMs, highBrightness, lowBrightness);
  _third.blink(intervalInMs, highBrightness, lowBrightness);
}

// Start sequential effect
void SequentialLightGroup::start(int blinkInterval, int staggerInterval,
                                 int highBrightness, int lowBrightness) {
  // Turn off lights
  off();
  // Indicate that sequential effect is running
  _isRunning = true;
  // Set new brightness levels
  _highBrightness = highBrightness;
  _lowBrightness = lowBrightness;
  // Reset intervals
  _blinkInterval.reset(blinkInterval);
  _staggerInterval.reset(staggerInterval);
}

// Loop function to process effects
void SequentialLightGroup::loop(unsigned int now) {
  // Handle sequential effect
  if (_isRunning) {
    bool wasHigh = _first.getBrightness() == _highBrightness;
    bool keepHigh = wasHigh;
    // Handle flipping on state
    if (_blinkInterval.check(now)) {
      keepHigh = !keepHigh;
    }
    // Handle high state
    if (keepHigh) {
      bool firstHigh = wasHigh;
      bool secondHigh = _second.getBrightness() == _highBrightness;
      bool thirdHigh = _third.getBrightness() == _highBrightness;
      if (!thirdHigh && _staggerInterval.check(now)) {
        if (!firstHigh) {
          _first.on(_highBrightness);
        } else if (!secondHigh) {
          _second.on(_highBrightness);
        } else if (!thirdHigh) {
          _third.on(_highBrightness);
        }
      }
    }
    // Handle transitioning to low state
    else if (wasHigh) {
      on(_lowBrightness, false);
    }
  }
  // Pass all other effects to individual lights
  else {
    _first.loop(now);
    _second.loop(now);
    _third.loop(now);
  }
}