# [Model Lighting](../../README.md)/[Shared Libraries](../README.md)/Interval

## Introduction
Interval is an abstracted class for managing time-based tasks. It simplifies checking to see if an interval of time has passed before performing an action.

## Setup
This setup assumes that you are using PlatformIO to manage projects and that the project is in a folder sitting at the root of this repository. You need to create a symlink dependency to the library and then PlatformIO will automatically compile it and make it available in your project

_**{repository_root}/{project_dir}/platformio.ini**_
```ini
[env:nodemcu-32s]
...
lib_deps =
  ...
  symlink://../shared/Interval
```

## Usage Examples

### Basic Interval

```cpp
#include <Interval.h>
#include <Utils.h>
#include <string>

// Create a new interval (defaults to 1 second)
Interval myInterval()

// Main loop
void loop(unsigned int now) {
  // Check the passage of time with the interval
  if (myInterval.check(now)) {
    printf("I should print out once every second");
  }
}

void app_main(void) {
  Utils::startLoop(&loop);
}
```

### Interval Resetting

```cpp
#include <Interval.h>
#include <Utils.h>
#include <string>

// Create a new 2 second interval
Interval myInterval(2000)

// Keep track of how many times the interval was hit
int hits = 0;

// Main loop
void loop(unsigned int now) {
  if (myInterval.check(now)) {
    hits += 1;
    if (hits == 10) {
      myInterval.reset(1000);
    }
    printf("I should print 10 times every 2 seconds, and then print every second after that");
  }
}

void app_main(void) {
  Utils::startLoop(&loop);
}
```

## Member Functions

### `Interval(void)` (constructor)

Create an interval instance with the default interval period of 1000 milliseconds (1 second)

### `Interval(int checkInterval)` (constructor)

Create an interval instance with a custom interval period

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| int | checkInterval | The check interval/period  |

### `void reset(int checkInterval = -1)`

Reset the interval state and provide an optional new interval period

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| int | checkInterval | A new check interval/period to replace the existing (passing -1 will reuse the existing check interval) | `-1` |

### `bool check(unsigned int now)`

Checks to see if the time indicated by the interval has passed since the last successfull check. If the time has passed, the function will return true and the last successfull check value will reset to allow the check function to be called indefinitely for repeated intervals.

**Parameters**
| Type | Name | Description |
| --- | --- | --- |
| unsigned int | now | The current timestamp in milliseconds |