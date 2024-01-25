# [Model Lighting](../../README.md)/[Shared Libraries](../README.md)/Utilities

## Introduction
Utils is a namespaced library with functions that are useful across multiple projects.

## Setup
This setup assumes that you are using PlatformIO to manage projects and that the project is in a folder sitting at the root of this repository. You need to create a symlink dependency to the library and then PlatformIO will automatically compile it and make it available in your project

_**{repository_root}/{project_dir}/platformio.ini**_
```ini
[env:nodemcu-32s]
...
lib_deps =
  ...
  symlink://../shared/Utils
```

## Functions


### `Utils::startLoop(void (*callback)(unsigned int), TickType_t tick = 1)`

This function is used for starting a loop that runs indefinitely where each iteration is delayed by the specified tick value to allow other background tasks to continue running without blocking execution.

**Parameters**
| Type | Name | Description | Default |
| --- | --- | --- | --- |
| void (*)(unsigned int) | callback | A callback function that is run on every iteration of the loop | N/A |
| TickType_t | tick | The tick value to delay on every iteration of the loop | `1` |

**Callback Parameters**
| Type | Name | Description |
| --- | --- | --- |
| unsigned int | now | The timestamp in milliseconds since the board has booted |

_**Simple Usage with an [Interval](../Interval/README.md)**_
```cpp
#include <Utils.h>
#include <Interval.h>
#include <string>

// Create a time-based 5 second interval
Interval fiveSecondInterval(5000);

void loop(unsigned int now) {
  // Used the timestamp passed by the loop to check the interval status
  if (oneSecondInterval.check(now)) {
    printf("This should print every five seconds\n");
  }
}

void app_main(void) {
  // Start the loop with the provided loop callback
  Utils::startLoop(&loop);
}
```