# [Model Lighting](../../README.md)/[Shared Libraries](../README.md)/Light

## Introduction
Light is an abstracted class for interacting with GPIO and PWM LED lights. It simplifies the configuration process and gives access to some simple time-based lighting effects.

## Setup
This setup assumes that you are using PlatformIO to manage projects and that the project is in a folder sitting at the root of this repository. You need to create a symlink dependency to the library and then PlatformIO will automatically compile it and make it available in your project

_**{repository_root}/{project_dir}/platformio.ini**_
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

  // Toggle the light's on/off state
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

## Static Functions

### `Light::configurePWMTimer(void)`

This function MUST be called before configuring or using any dimmable lights in order to setup the global PWM timer

## Member Functions

### `Light(int pin)` (constructor)

Create a _**non-dimmable**_ light instance assigned to the specified GPIO pin

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| int | pin | The GPIO pin for the light  |

### `Light(int pin, int channel)` (constructor)

Create a _**dimmable**_ light instance assigned to the specified GPIO pin and PWM channel

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| int | pin | The GPIO pin for the light |
| int | channel | The PWM channel for the light |

### `int getPin(void)`

Returns the numeric value of the light's GPIO pin

### `int getChannel(void)`

Returns the numeric value of the light's PWM channel. If the light is non-dimmable, the channel value will be `-1`

### `int getBrightness(void)`

Returns the current brightness value of the light.

### `bool isOn(void)`

Indicates if the light is on (brightness value is > 0)

### `bool isDimmable(void)`

Indicates if the light is configured as dimmable

### `bool isBlinking(void)`

Indicates if the light's blinking effect is active

### `void configure(void)`

Handles configuring the light's GPIO and PWM channel settings. This function is automatically called by the `on(...)` method, so in most cases it doesn't need to be called explicitly, but for some situations it is good to configure it explictly.

### `void on(int brightness = 100, bool stopEffects = true)`

Handles turning on the light to a specific brightness. If the light has not been configured yet, it will automatically be configured when calling this function the first time.

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| int | brightness | The brightness of the light as a percentage value from 0 to 100 | `100` |
| bool | stopEffects | If enabled, it will stop any active lighting effects | `true` |

### `void off(bool stopEffects = true)`

Turns off the light. Internally this simply calls the on method with a brightness of 0, so it will also automatically configure the light if needed.

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| bool | stopEffects | If enabled, it will stop any active lighting effects | `true` |

### `void toggle(bool stopEffects = true)`

Toggles the light between the current and previous brightness values. This means that the light can toggle between on and off, or some variation of different brightness values.

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| bool | stopEffects | If enabled, it will stop any active lighting effects | `true` |

### `void blink(int intervalInMs = 1000, int highBrightness = 100, int lowBrightness = 0)`

Starts the blinking effect. The blinking effect is a time-based animation and does not use blocking delays. To progress the animation, the `loop(...)` function must called as frequently as possible (frequency will determine how accurate the effect is).

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| int | intervalInMs | The duration of the interavl in milliseconds for switching between the high and low brightness values | `1000` |
| int | highBrightness | The high brightness value | `100` |
| int | lowBrightness | The low brightness value | `0` |

### `void loop(unsigned int now)`

Should be called as frequently as possible if lighting effects like `blink(...)` are being used. It interacts with the internal time-based intervals to progress animated effects. Calling it while lighting effects are turned off will not have any negative impact.

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| unsigned int | now | The current timestamp in milliseconds |