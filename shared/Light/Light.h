#ifndef LIGHT_H
#define LIGHT_H

class Light {
private:
  int pin;
  bool isSetup = false;
  bool _isOn = false;

public:
  // Set light pin
  Light(int pin);

  // Setup light's GPIO pin
  void configure();

  // Turn on light to full brightness
  void on();

  // Turn off light
  void off();

  // Check if the light is on
  bool isOn(void);

  // Toggle light on or off
  void toggle();
};

#endif