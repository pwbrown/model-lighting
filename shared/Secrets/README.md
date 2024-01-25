# [Model Lighting](../../README.md)/[Shared Libraries](../README.md)/Secrets

## Introduction
Secrets is a temporary library for holding secret/sensitive values that are by default excluded from GIT.

## Setup
This setup assumes that you are using PlatformIO to manage projects and that the project is in a folder sitting at the root of this repository. You need to create a symlink dependency to the library and then PlatformIO will automatically compile it and make it available in your project

_**{repository_root}/{project_dir}/platformio.ini**_
```ini
[env:nodemcu-32s]
...
lib_deps =
  ...
  symlink://../shared/Secrets
```

## Usage

To fill in secrets, create a new file named `Secrets.h` and paste in the contents from the `Secrets.h.txt` file. Modify the file with the values applicable to the current environment (ex. WiFi credentials, MQTT host/credentials, etc.)