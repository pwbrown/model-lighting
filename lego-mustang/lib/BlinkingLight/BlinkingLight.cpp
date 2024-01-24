#include "BlinkingLight.h"

/**
 * Initialize Sequential lights with dimmable lights and blinking interval
 * settings
 */
BlinkingLight::BlinkingLight(DimmableLight &light, int blinkInterval)
    : light{light}, blinkInterval{Interval(blinkInterval)} {}

/**
 * Configures all three lights
 */
void BlinkingLight::configure(void) { light.configure(); }

/**
 * Turns lights on to a specific brightness
 */
void BlinkingLight::on(int brightness) {
  // Update state accordingly
  isOn = true;
  isBlinking = false;
  // Set lights
  light.on(brightness);
}

/** Turn off lights */
void BlinkingLight::off(void) {
  // Update state accordingly
  isOn = false;
  isBlinking = false;
  // Set lights
  light.off();
}

/**
 * Updates the state of the lights to start blinking (handled by loop
 * method)
 */
void BlinkingLight::startBlinking(int brightness) {
  // Update state accordingly
  isOn = false;
  isBlinking = true;
  blinkBrightness = brightness;
  // Reset animation intervals
  blinkInterval.reset();
}

/**
 * Loop method that should be called as quickly as possible that is responsible
 * for animating the blinking lights
 */
void BlinkingLight::loop(unsigned int now) {
  // Only animate lights if state is blinking
  if (isBlinking) {
    bool wasOn = isOn;
    // Handle flipping overall on state during animation
    if (blinkInterval.check(now)) {
      isOn = !isOn;
      if (isOn) {
        light.on(blinkBrightness);
      } else {
        light.off();
      }
    }
  }
}