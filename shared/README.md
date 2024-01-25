# [Model Lighting](../README.md)/Shared Libraries

Shared Libraries are available to any PlatformIO projects located in this repo. They can be included as `symlink` library dependencies:

_**{repository_root}/{project_dir}/platformio.ini**_
```ini
[env:nodemcu-32s]
...
lib_deps =
  ...
  symlink://../shared/{Library_Dir}
```

## Libraries

- [Interval](./Interval/README.md) - Controller for time-based interval system
- [Light](./Light/README.md) - Controller for dimmable and non-dimmable LEDs
- [MqttClient](./MqttClient/README.md) - Controller for managing WiFi and MQTT client connection
- [Secrets](./Secrets/README.md) - Manage secret values
- [Utils](./Utils/README.md) - Useful general-purpose utilities that are common between multiple applications