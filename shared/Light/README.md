# Light

## Introduction
Light is an abstracted class for interacting with GPIO and PWM LED lights. It simplifies the configuration process and gives access to some simple time-based lighting effects.

## Setup
This setup assumes that you are using PlatformIO to manage projects and that the project is in a folder sitting at the root of this repository. You need to create a symlink dependency to the library and then PlatformIO will automatically compile it and make it available in your project

_**In my_project_name/platformio.ini**_
```ini
[env:nodemcu-32s]
...
lib_deps =
  ...
  symlink://../shared/Light
```

## Usage Examples

### Basic on/off functionality

```cpp
#include <Light.h>

// Create light instance assigned to GPIO pin #2
Light myLight(2);

void app_main(void) {
  // Turn the light on
  myLight.on();

  // Turn the light off
  myLight.off();

  // Toggle the LED on/off state
  myLight.toggle();
}
```

### Basic dimming functionality

```cpp
#include <Light.h>

// Create light for GPIO pin #2 and assign it to PWM channel 0
Light myLight(2, 0);

void app_main(void) {
  // Must initialize the PWM timer before configuring any dimmable lights
  Light::configurePWMTimer();

  // Turns light on to 100% brightness by default
  myLight.on();

  // Turn light on to 50% brigtness
  myLight.on(50);

  // Turn light off (sets brightness to 0%)
  myLight.off();

  // Toggles between the current and previous brightness values.
  // In this example that would be 0% and 50%
  myLight.toggle();
}
```

### Blinking effect without dimming

```cpp
#include <Light.h>
#include <Utils.h>

Light myLight(2);

void loop(unsigned int now) {
  myLight.loop(now);
}

void app_main(void) {
  // Toggle between on/off on a 1 second interval
  myLight.blink(1000);

  // Start the main loop and pass our callback
  // to initiate the time-based effect
  Utils::startLoop(&loop);
}
```

### Blinking effect with dimming

```cpp
#include <Light.h>
#include <Utils.h>

Light myLight(2, 0);

void loop(unsigned int now) {
  myLight.loop(now);
}

void app_main(void) {
  // Initialize PWM Timer
  Light::configurePWMTimer();
  
  // Toggle between 50% and 0% brightness on a
  // half second (500ms) interval
  myLight.blink(500, 50);

  // Toggle between 75% and 25% brightness on a
  // quarter second (250ms) interval
  myLight.blink(250, 75, 25);

  // Start the main loop
  Utils::startLoop(&loop);
}
```