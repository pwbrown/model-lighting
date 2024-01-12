#include "Light.h"
#include <Arduino.h>

// Set light pin
Light::Light(int pin) : pin{pin} {};

// Setup light's GPIO pin
void Light::setup() {
  if (!isSetup) {
    // Set GPIO pin
    pinMode(pin, OUTPUT);
    // Only run setup once
    isSetup = true;
    // Always start turned off
    off();
  }
};

// Turn on light to full brightness
void Light::on() {
  isOn = true;
  digitalWrite(pin, HIGH);
};

// Turn off light
void Light::off() {
  isOn = false;
  digitalWrite(pin, LOW);
};

// Toggles the state of the light
void Light::toggle() {
  if (isOn) {
    off();
  } else {
    on();
  }
}