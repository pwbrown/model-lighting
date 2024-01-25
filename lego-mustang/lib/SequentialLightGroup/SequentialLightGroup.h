#ifndef SEQUENTIAL_LIGHT_GROUP_H
#define SEQUENTIAL_LIGHT_GROUP_H

#include <Interval.h>
#include <Light.h>

#define DEFAULT_BLINK_INTERVAL 1000
#define DEFAULT_STAGGER_INTERVAL 250

class SequentialLightGroup {

public:
  /**
   * Create a Sequential lights controller
   * @param first First light
   * @param second Second light
   * @param third Third light
   * @param blinkInterval The amount of time in milliseconds the lights are
   * considered on and off (1000 would be on for 1 second, off for 1 second)
   * @param staggerInterval The amount of time in milliseconds between turning
   * on individual lights (250 would turn first light on at 0, second at 250,
   * and third at 500)
   */
  SequentialLightGroup(Light &first, Light &second, Light &third);

  /**
   * Configures all 3 lights
   */
  void configure(void);

  /**
   * Turn on all lights to a specific brightness
   * @param brightness Number from 0 to 100 representing brightness as a
   * percentage
   * @param stopEffects If the currently running effect should be stopped
   */
  void on(int brightness = 100, bool stopEffects = true);

  /**
   * Turn off all lights
   * @param stopEffects If the currently running effect should be stopped
   */
  void off(bool stopEffects = true);

  /**
   * Starts the normal blinking effect on all lights
   * @param intervalInMs Interval to blink lights at
   * @param highBrightness High brightness level during blink effect
   * @param lowBrightness Low brightness level during blink effect
   */
  void blink(int intervalInMs = DEFAULT_BLINK_INTERVAL,
             int highBrightness = 100, int lowBrightness = 0);

  /**
   * Start the sequential lights effect going from first to third light
   * @param blinkInterval Interval to switch from the high to the low state
   */
  void start(int blinkInterval = DEFAULT_BLINK_INTERVAL,
             int staggerInterval = DEFAULT_STAGGER_INTERVAL,
             int highBrightness = 100, int lowBrightness = 0);

  /**
   * Loop function that should be called as often as possible with the updated
   * timestamp in milliseconds. This function is responsible for updating the
   * state of the animation.
   * @param now Timestamp in milliseconds
   */
  void loop(unsigned int now);

private:
  // Lights
  Light &_first;
  Light &_second;
  Light &_third;

  // State
  bool _isRunning = false; // Indicates if the sequential effect is running
  int _lowBrightness =
      0; // Saves the low level brightness for the sequential effect
  int _highBrightness =
      100; // Saves the high level brightness for the sequential effect

  // Intervals for animations
  Interval _blinkInterval;
  Interval _staggerInterval;
};

#endif