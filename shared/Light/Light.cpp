#include "Light.h"

#include <driver/gpio.h>
#include <driver/ledc.h>
#include <math.h>

// Macros for abstrating type conversions
#define PIN (gpio_num_t) _pin
#define CHANNEL (ledc_channel_t) _channel

// Configures the global PWM timer
void Light::configurePWMTimer(void) {
  ledc_timer_config_t timerConfig = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_13_BIT,
      .timer_num = LEDC_TIMER_0,
      .freq_hz = 4000,
      .clk_cfg = LEDC_AUTO_CLK,
  };
  ledc_timer_config(&timerConfig);
}

// Initialize standard light
Light::Light(int pin) : _pin{pin} {};

// Initialize dimmable light
Light::Light(int pin, int channel) : _pin{pin}, _channel{channel} {};

// Setup light's GPIO pin
void Light::configure(void) {
  if (!_isConfigured) {
    // Configure as standard (non dimmable)
    if (!isDimmable()) {
      gpio_reset_pin(PIN);
      gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    }
    // Configure as PWM (dimmable)
    else {
      ledc_channel_config_t channelConfig = {
          .gpio_num = _pin,
          .speed_mode = LEDC_LOW_SPEED_MODE,
          .channel = CHANNEL,
          .intr_type = LEDC_INTR_DISABLE,
          .timer_sel = LEDC_TIMER_0,
          .duty = 0,
          .hpoint = 0,
      };
      ledc_channel_config(&channelConfig);
    }
    _isConfigured = true;
    // Start with the light turned off
    off();
  }
};

// Turn on light to full brightness
void Light::on(int brightness, bool stopEffects) {
  // Make sure the light is configured
  if (!_isConfigured) {
    configure();
  }
  // Stop any active effects
  if (stopEffects) {
    _isBlinking = false;
  }
  // Only apply new brightness if it has changed
  if (brightness != _currBrightness) {
    // Update previous and current brightness values
    _prevBrightness = _currBrightness;
    _currBrightness = brightness;
    // Update LED brightness based on light type
    if (!isDimmable()) {
      gpio_set_level(PIN, _currBrightness);
    } else {
      double percentage = _currBrightness == 0 ? 0.0 : _currBrightness / 100.0;
      double duty = 8192 * percentage;
      ledc_set_duty(LEDC_LOW_SPEED_MODE, CHANNEL, duty);
      ledc_update_duty(LEDC_LOW_SPEED_MODE, CHANNEL);
    }
  }
};

// Turn off light
void Light::off(bool stopEffects) {
  // Just call the on method with a brightness of 0
  on(0, stopEffects);
};

// Toggles the state of the light
void Light::toggle(bool stopEffects) {
  // Just call the on method with the previous brightness
  on(_prevBrightness, stopEffects);
}

// Starts the blinking effect by setting various state variables
void Light::blink(int intervalInMs, int highBrightness, int lowBrightness) {
  // Ignore the blink effect if the high and low settings match
  if (highBrightness == lowBrightness) {
    return;
  }
  // Start by turning off the light
  off();
  // Set blinking flag so the loop function can handle the blinking effect
  _isBlinking = true;
  // Reset the effect interval
  _effectInterval.reset(intervalInMs);
  // Set updated brightness values to let the toggle function (starting with the
  // opposite values so the first loop run will toggle to the high value)
  _currBrightness = lowBrightness;
  _prevBrightness = highBrightness;
}

// Loop function for handling lighting effects
void Light::loop(unsigned int now) {
  // Handle blinking effect
  if (_isBlinking && _effectInterval.check(now)) {
    toggle(false);
  }
}