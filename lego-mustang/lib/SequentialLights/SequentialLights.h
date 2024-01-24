#ifndef SEQUENTIAL_LIGHTS_H
#define SEQUENTIAL_LIGHTS_H

#include <DimmableLight.h>
#include <Interval.h>

#define DEFAULT_BLINK_INTERVAL 1000
#define DEFAULT_STAGGER_INTERVAL 250

class SequentialLights {

public:
  /**
   * Create a Sequential lights controller
   * @param outer Outer light
   * @param middle Middle light
   * @param inner Inner light
   * @param blinkInterval The amount of time in milliseconds the lights are
   * considered on and off (1000 would be on for 1 second, off for 1 second)
   * @param staggerInterval The amount of time in milliseconds between turning
   * on individual lights (250 would turn inner light on at 0, middle at 250,
   * and outer at 500)
   */
  SequentialLights(DimmableLight &outer, DimmableLight &middle,
                   DimmableLight &inner,
                   int blinkInterval = DEFAULT_BLINK_INTERVAL,
                   int staggerInterval = DEFAULT_STAGGER_INTERVAL);

  /** Simplifies configuring all 3 lights */
  void configure(void);

  /**
   * Turn on all lights to a specific brightness (cancels any existing blinking
   * animation)
   * @param brightness Number from 0 to 100 representing brightness as a
   * percentage
   */
  void on(int brightness = 100);

  /**
   * Turn off all lights (cancels any existing blinking animation)
   */
  void off(void);

  /**
   * Starts the sequential blinking animation (will reset if already started)
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
  DimmableLight &outer;  // Outer light
  DimmableLight &middle; // Middle light
  DimmableLight &inner;  // Inner light

  // State
  bool isOn = false;         // Indicates if the lights are in the "on" state
  bool isBlinking = false;   // Indicates if the blinking animation is active
  int blinkBrightness = 100; // How bright the blink animation should be

  // Intervals for animations
  Interval blinkInterval;
  Interval staggerInterval;
};

#endif