#include "Light.h"

#include <driver/gpio.h>

#define PIN (gpio_num_t) pin

// Set light pin
Light::Light(int pin) : pin{pin} {};

// Setup light's GPIO pin
void Light::configure() {
  if (!isSetup) {
    gpio_reset_pin(PIN);
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    isSetup = true;
    off();
  }
};

// Turn on light to full brightness
void Light::on() {
  _isOn = true;
  gpio_set_level(PIN, 1);
};

// Turn off light
void Light::off() {
  _isOn = false;
  gpio_set_level(PIN, 0);
};

// Indicates if the light is on
bool Light::isOn(void) { return _isOn; }

// Toggles the state of the light
void Light::toggle() {
  if (_isOn) {
    off();
  } else {
    on();
  }
}