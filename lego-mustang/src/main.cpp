#include "cJSON.h"
#include "pins.h"
#include <Enums.h>
#include <Light.h>
#include <MqttClient.h>
#include <SequentialLightGroup.h>
#include <Utils.h>

// Export main function for C compiler
extern "C" {
void app_main(void);
}

// ********************* STATES *********************
Enums::LightMode lightMode = Enums::LightMode::Off;

// ********************* MQTT CLIENT SETUP *********************
MqttClient client; // MQTT Client

void setupMqttClient() {}

// PWM Channel incremental (increment every time it is used)
int pwmChannel = 0;

// Left Headlight
Light leftHeadlight(LEFT_HEADLIGHT, pwmChannel++);

// Right Headlight
Light rightHeadlight(RIGHT_HEADLIGHT, pwmChannel++);

// Left Taillight
Light leftInnerTaillight(LEFT_INNER_TAILLIGHT, pwmChannel++);
Light leftMiddleTaillight(LEFT_MIDDLE_TAILLIGHT, pwmChannel++);
Light leftOuterTaillight(LEFT_OUTER_TAILLIGHT, pwmChannel++);
SequentialLightGroup leftTaillight(leftInnerTaillight, leftMiddleTaillight,
                                   leftOuterTaillight);

// Right Taillight
Light rightInnerTaillight(RIGHT_INNER_TAILLIGHT, pwmChannel++);
Light rightMiddleTaillight(RIGHT_MIDDLE_TAILLIGHT, pwmChannel++);
Light rightOuterTaillight(RIGHT_OUTER_TAILLIGHT, pwmChannel++);
SequentialLightGroup rightTaillight(rightInnerTaillight, rightMiddleTaillight,
                                    rightOuterTaillight);

// Other lights
Light fogLights(FOG_LIGHTS);
Light runningLights(RUNNING_LIGHTS);
Light reverseLights(REVERSE_LIGHTS);
Light interiorLights(INTERIOR_LIGHTS);

// Turn off all lights
void allOff(void) {
  runningLights.off();
  fogLights.off();
  leftHeadlight.off();
  rightHeadlight.off();
  interiorLights.off();
  leftTaillight.off();
  rightTaillight.off();
  reverseLights.off();
}

/**
 * Main loop function for lighting effects
 */
void loop(unsigned int now) {
  leftHeadlight.loop(now);
  rightHeadlight.loop(now);
  leftTaillight.loop(now);
  rightTaillight.loop(now);
}

/**
 * Application entrypoint. Configure lights, MQTT client, and start the main
 * effects loop
 */
void app_main(void) {
  Light::configurePWMTimer();

  // Turn off all lights at the beginning
  allOff();

  // Start main loop
  Utils::startLoop(&loop);
}
