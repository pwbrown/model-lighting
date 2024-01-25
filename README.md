# Model Lighting

## Introduction

I originally created this project after lighting some of my Lego vehicles (Ford Mustang, James Bond Aston Martin, etc.) with premade lighting kits. This was fun, but the effect was purely binary: the lights were either on or off. I wanted MORE!! I typically don't play with the Lego sets that I build, and my kids are usually too rough with them, but I still wanted them to be interactive (from a distance). So this project was born.

My idea was to replace the original lighting kits with a custom made solution that could allow me to control individual lights or light groups, apply cool effects, and be able to control all of that remotely.

## The Solution (as of Jan 2024)

My solution involves using ESP32 chips that are flashed with custom software (this repository) that talk to an MQTT broker. I'm using ESP32 chips because they are the best bang for the buck. They have builtin Wifi, Bluetooth, and are really cheap. For my MQTT setup, I'm using a self-hosted MQTT broker through docker, and I'm using my own Home Assistant server to interact with the models via the MQTT integration.

## [Shared Libraries](./shared/README.md)

As part of this project I knew I wanted to light up different models (mostly Lego, but not exclusively), so creating a set of reusable shared libraries was key to doing that successfully. Most of the effort went into building these fundamental building blocks.

## Projects

| Name | Description | Status |
| --- | --- | --- |
| [Lego Mustang](./lego-mustang/README.md) | Custom lighting for the [Lego Ford Mustang](https://www.lego.com/en-us/product/ford-mustang-10265) with 12+ individual lighting channels and working sequential taillights. | In Progress |