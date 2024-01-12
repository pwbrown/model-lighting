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
  // Set light pin and PWM channel
  DimmableLight(int pin, int channel);

  // Setup light's PWM channel and pin
  void setup();

  // Turn on light to a specific brightness
  void on(int brightness = 255) const;

  // Turn off light
  void off() const;
};

#endif