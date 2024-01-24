#include "SequentialLights.h"

/**
 * Initialize Sequential lights with dimmable lights and blinking interval
 * settings
 */
SequentialLights::SequentialLights(DimmableLight &outer, DimmableLight &middle,
                                   DimmableLight &inner, int blinkInterval,
                                   int staggerInterval)
    : outer{outer}, middle{middle}, inner{inner},
      blinkInterval{Interval(blinkInterval)},
      staggerInterval{Interval(staggerInterval)} {}

/**
 * Configures all three lights
 */
void SequentialLights::configure(void) {
  outer.configure();
  middle.configure();
  inner.configure();
}

/**
 * Turns lights on to a specific brightness
 */
void SequentialLights::on(int brightness) {
  // Update state accordingly
  isOn = true;
  isBlinking = false;
  // Set lights
  outer.on(brightness);
  middle.on(brightness);
  inner.on(brightness);
}

/** Turn off lights */
void SequentialLights::off(void) {
  // Update state accordingly
  isOn = false;
  isBlinking = false;
  // Set lights
  outer.off();
  middle.off();
  inner.off();
}

/**
 * Updates the state of the lights to start blinking (handled by loop
 * method)
 */
void SequentialLights::startBlinking(int brightness) {
  // Update state accordingly
  isOn = false;
  isBlinking = true;
  blinkBrightness = brightness;
  // Reset animation intervals
  blinkInterval.reset();
  staggerInterval.reset();
}

/**
 * Loop method that should be called as quickly as possible that is responsible
 * for animating the blinking lights
 */
void SequentialLights::loop(unsigned int now) {
  // Only animate lights if state is blinking
  if (isBlinking) {
    bool wasOn = isOn;
    // Handle flipping overall on state during animation
    if (blinkInterval.check(now)) {
      isOn = !isOn;
    }
    // Handle on state
    if (isOn) {
      if (!outer.isOn() && staggerInterval.check(now)) {
        if (!inner.isOn()) {
          inner.on(blinkBrightness);
        } else if (!middle.isOn()) {
          middle.on(blinkBrightness);
        } else {
          outer.on(blinkBrightness);
        }
      }
    }
    // Handle initial off state (turn off lights and reset the stagger interval)
    else if (wasOn) {
      inner.off();
      middle.off();
      outer.off();
      staggerInterval.reset();
    }
  }
}