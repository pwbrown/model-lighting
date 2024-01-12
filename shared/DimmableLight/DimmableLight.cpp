#include "DimmableLight.h"
#include <Arduino.h>

// Initialize GPIO pin and PWM channel
DimmableLight::DimmableLight(int pin, int channel)
    : pin{pin}, channel{channel} {};

// Setup method (to be called during setup process)
void DimmableLight::setup() {
  if (!isSetup) {
    if (!isSetup) {
      // Setup new PWM channel
      ledcSetup(channel, PWM_FREQUENCY, PWM_RESOLUTION);
      // Attach GPIO pin to the PWM channel
      ledcAttachPin(pin, channel);
      isSetup = true;
    }
  }
};

// Turn on light to a specific brightness
void DimmableLight::on(int brightness) const {
  ledcWrite(channel, brightness);
};

// Turn off light
void DimmableLight::off() const { ledcWrite(channel, 0); };