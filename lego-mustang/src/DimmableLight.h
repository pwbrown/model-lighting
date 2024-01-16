#ifndef DIMMABLE_LIGHT_H
#define DIMMABLE_LIGHT_H

const int PWM_FREQUENCY = 5000; // Frequency
const int PWM_RESOLUTION = 8;   // Allows setting brightness from 0 to 255

class DimmableLight {
private:
  int pin;              // GPIO pin number
  int channel;          // PWM Channel
  bool isSetup = false; // Indicates if the light has been setup

public:
  /**
   * Called before any dimmable light has been configured to setup the
   * reusable global PWM timer
   */
  static void configureGlobalTimer();

  /**
   * Initialize new dimmable light on the specified
   * GPIO pin and PWM channel
   * @param pin GPIO pin to use for the light
   * @param channel PWM channel to use for the light
   */
  DimmableLight(int pin, int channel);

  /**
   * Configure the light's PWM channel
   */
  void configure();

  /**
   * Turn on the light to a specific brightness
   * @param brigtness percentage value from 0 to 100
   */
  void on(int brightness = 100) const;

  /**
   * Turn off the light
   */
  void off() const;
};

#endif