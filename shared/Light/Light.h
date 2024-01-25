#ifndef LIGHT_H
#define LIGHT_H

#include <Interval.h>

#define DEFAULT_EFFECT_INTERVAL 1000

/**
 * Light is a utility class for interacting with GPIO and PWM LEDs. It comes
 * with easy methods for controlling LED brightness and state as well as
 * functionality for applying time-based lighting effects.
 */
class Light {
public:
  /**
   * This function must be called before any dimmable light is configured and
   * used otherwise the dimming effect will not work
   */
  static void configurePWMTimer(void);

  /**
   * Initialize Standard light
   * @param pin GPIO pin number
   */
  Light(int pin);

  /**
   * Initialize Dimmable light
   * @param pin GPIO pin number
   * @param channel PWM channel number
   */
  Light(int pin, int channel);

  /** Get current pin value */
  int getPin() { return _pin; }

  /** Get current pwm channel */
  int getChannel() { return _channel; }

  /** Get current brightness */
  int getBrightness() { return _currBrightness; }

  /** Indicates if the light is on (brightness > 0) */
  bool isOn() { return _currBrightness > 0; }

  /** Indicates if the light is dimmable */
  bool isDimmable() { return _channel >= 0; }

  /** Indicates if the light is currently blinking */
  bool isBlinking() { return _isBlinking; }

  /**
   * Configure the light's pin and pwm channel if necessary
   */
  void configure(void);

  /**
   * Turn on the light (brightness value only applies if the light is dimmable).
   * @param brightness Percentage of brightness from 0 to 100
   * @param stopEffects Whether to stop any active effects
   */
  void on(int brightness = 100, bool stopEffects = true);

  /**
   * Turn off the light (will also stop any active effect)
   * @param stopEffects Whether to stop any active effects
   */
  void off(bool stopEffects = true);

  /**
   * Toggle the light between current and previous brightness values
   * @param stopEffects Whether to stop any active effects
   */
  void toggle(bool stopEffects = true);

  /**
   * Starts the blinking effect (must call the loop function to continue
   * blinking in the background)
   * @param intervalInMs The interval used to toggle between on and off
   * @param highBrightness How bright the light should be in the "high" state
   * @param lowBrightness How bright the light should be in the "low" state
   */
  void blink(int intervalInMs = DEFAULT_EFFECT_INTERVAL,
             int highBrightness = 100, int lowBrightness = 0);

  /**
   * Loop function that should be called as frequently as possible to run light
   * effects in the background. This function does not need to be called if no
   * effects are used
   * @param now The current timestamp in milliseconds
   */
  void loop(unsigned int now);

private:
  int _pin;                   // GPIO pin
  int _channel = -1;          // PWM channel
  int _currBrightness = 0;    // Current brightness percentage
  int _prevBrightness = 100;  // Previous brightness percentage
  bool _isConfigured = false; // Indicates if the light has been configured
  bool _isBlinking = false;   // Indicates if the blinking effect is active
  Interval _effectInterval;   // Interval to use for the current effect
};

#endif