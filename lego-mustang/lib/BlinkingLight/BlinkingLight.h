#ifndef BLINKING_LIGHT_H
#define BLINKING_LIGHT_H

#include <DimmableLight.h>
#include <Interval.h>

#define DEFAULT_BLINK_INTERVAL 1000

class BlinkingLight {

public:
  /**
   * Create a Sequential lights controller
   * @param light light
   * @param blinkInterval The amount of time in milliseconds the lights are
   * considered on and off (1000 would be on for 1 second, off for 1 second)
   */
  BlinkingLight(DimmableLight &light,
                int blinkInterval = DEFAULT_BLINK_INTERVAL);

  /** Configures the light */
  void configure(void);

  /**
   * Turn on light to a specific brightness (cancels any existing blinking
   * animation)
   * @param brightness Number from 0 to 100 representing brightness as a
   * percentage
   */
  void on(int brightness = 100);

  /**
   * Turn off light (cancels any existing blinking animation)
   */
  void off(void);

  /**
   * Starts the blinking animation (will reset if already started)
   */
  void startBlinking(int brightness = 100);

  /**
   * Loop function that should be called as often as possible with the updated
   * timestamp in milliseconds. This function is responsible for updating the
   * state of the animation.
   * @param now Timestamp in milliseconds
   */
  void loop(unsigned int now);

private:
  // Lights
  DimmableLight &light;

  // State
  bool isOn = false;         // Indicates if the lights are in the "on" state
  bool isBlinking = false;   // Indicates if the blinking animation is active
  int blinkBrightness = 100; // How bright the blink animation should be

  // Intervals for animations
  Interval blinkInterval;
};

#endif