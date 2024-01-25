# [Model Lighting](../README.md)/Lego Mustang

This project is one of several designed to apply custom lighting and effects to my lego projects to make them more interactive and remotely controllable.

## Materials

* [Lego Ford Mustang - 10265](https://www.lego.com/en-us/product/ford-mustang-10265)
* ESP32 Development Board
* 5V USB Power Supply
* 3ft USB A to Micro USB cable
* LEDs (12 - 17)
   * 2 for headlights
   * 2 for running lights
   * 2 for fog lights
   * 6 for taillights
   * 3 for engine stack (optional)
   * 2 for license plates (optional)
* RGB LED Strips (1-3)
   * 1 for cabin lighting
   * 2 for underglow lighting (optional)

## 11-14 Light Groups/Channels (Inidividual GPIO Pins)

* License Plates: 2 LEDS (optional)
* Running Lights: 2 LEDS
* Fog Lights: 2 LEDS
* Left Headlight: 1 LED
* Right Headlight: 1 LED
* Engine Stack: 3 LEDs (Optional)
* Cabin Lighting: 1 LED Strip (Maybe RGB)
* Underglow Lighting: 2 LED Strips (Maybe RGB and Optional)
* Left Taillight 1: 1 LED
* Left Taillight 2: 1 LED
* Left Taillight 3: 1 LED
* Right Taillight 1: 1 LED
* Right Taillight 2: 1 LED
* Right Taillight 3: 1 LED

## Desired Modes/Effects

* Vehicle Off: No sound effects and license plate lights off (optional)
* Vehicle On: Idling sound effect by default and license plate lights on (optional)
* Lights Off: All lights off excluding license plate lights
* Running Lights: Dimmed taillights and running lights on
* Lights On: Slightly dimmed headlights, dimmed taillights, and running lights
* Brights On: Fully lit headlights
* Brake: Fully lit taillights
* Cabin Lighting Off: Turn off cabin lighting
* Cabin Lighting On: Turn on cabin lighting
* Underglow lighting On: Turn on underglow lighting (optional)
* Underglow lighting Off: Turn off underglow lighting (optional)
* Change Light Color: Change cabin and underglow lighting color
* Rev engine: Make engine reving noise and swell brightness of engine stack and maybe exhaust?
* Turning Left/Right: Flash headlight and sequentially light up corresponding taillight
* Hazards: Flash both headlights and taillights together (non-sequential)