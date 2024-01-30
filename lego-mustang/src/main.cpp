#include "cJSON.h"
#include "settings.h" // Includes pin, topic, and behavior settings
#include <Light.h>
#include <MqttClient.h>
#include <SequentialLightGroup.h>
#include <Utils.h>
#include <cJSON.h>
#include <string>

// Export main function for C compiler
extern "C" {
void app_main(void);
}

// ********************* STATE DEFINITIONS *********************

// Lighting Modes
#define LIGHT_MODE_OFF "OFF"           // Lights Off
#define LIGHT_MODE_RUNNING "RUNNING"   // Daytime Running Lights
#define LIGHT_MODE_LOW_BEAM "LOW_BEAM" // Low Beams

// Light Switch Modes
#define SWITCH_ON "ON"   // Light Channel On
#define SWITCH_OFF "OFF" // Light Channel Off

// Turning Modes
#define TURNING_OFF "OFF"     // No turn signal
#define TURNING_LEFT "LEFT"   // Left turn signal
#define TURNING_RIGHT "RIGHT" // Right turn signal

#define AVAILABLE_YES "YES" // Board is available
#define AVAILABLE_NO "NO"   // Board is not available

// ********************* STATES ***********************
std::string lightingState = LIGHT_MODE_OFF;
std::string highBeamState = SWITCH_OFF;
std::string turningState = TURNING_OFF;
std::string reverseState = SWITCH_OFF;
std::string fogState = SWITCH_OFF;
std::string interiorState = SWITCH_OFF;
std::string hazardState = SWITCH_OFF;

// ********************* MQTT CLIENT SETUP *********************
MqttClient client("lego_mustang"); // MQTT Client

// ********************* LIGHT SETUP *************************
int pwmChannel = 0; // incremental PWM channel

// Left Headlight
Light leftHeadlight(LEFT_HEADLIGHT_PIN, pwmChannel++);

// Right Headlight
Light rightHeadlight(RIGHT_HEADLIGHT_PIN, pwmChannel++);

// Left Taillight Group
Light leftInnerTaillight(LEFT_INNER_TAILLIGHT_PIN, pwmChannel++);
Light leftMiddleTaillight(LEFT_MIDDLE_TAILLIGHT_PIN, pwmChannel++);
Light leftOuterTaillight(LEFT_OUTER_TAILLIGHT_PIN, pwmChannel++);
SequentialLightGroup leftTaillight(leftInnerTaillight, leftMiddleTaillight,
                                   leftOuterTaillight);

// Right Taillight Group
Light rightInnerTaillight(RIGHT_INNER_TAILLIGHT_PIN, pwmChannel++);
Light rightMiddleTaillight(RIGHT_MIDDLE_TAILLIGHT_PIN, pwmChannel++);
Light rightOuterTaillight(RIGHT_OUTER_TAILLIGHT_PIN, pwmChannel++);
SequentialLightGroup rightTaillight(rightInnerTaillight, rightMiddleTaillight,
                                    rightOuterTaillight);

// Other lights
Light fogLights(FOG_LIGHTS_PIN);
Light runningLights(RUNNING_LIGHTS_PIN);
Light reverseLights(REVERSE_LIGHTS_PIN);
Light interiorLights(INTERIOR_LIGHTS_PIN);

// ************************ STATE UPDATES **********************

/** Update all lights based on the current state */
void updateLightsFromState(void) {
  // Fog lights
  fogState == SWITCH_ON ? fogLights.on() : fogLights.off();
  // Reverse lights
  reverseState == SWITCH_ON ? reverseLights.on() : reverseLights.off();
  // Interior lights
  interiorState == SWITCH_ON ? interiorLights.on() : interiorLights.off();
  // Lights switched off
  if (lightingState == LIGHT_MODE_OFF) {
    runningLights.off();
    leftHeadlight.off();
    rightHeadlight.off();
    leftTaillight.off();
    rightTaillight.off();
  }
  // Daytime running lights
  else if (lightingState == LIGHT_MODE_RUNNING) {
    runningLights.on();
    leftHeadlight.off();
    rightHeadlight.off();
    leftTaillight.on(RUNNING_BRIGHTNESS);
    rightTaillight.on(RUNNING_BRIGHTNESS);
  }
  // Low beams
  else if (lightingState == LIGHT_MODE_LOW_BEAM) {
    runningLights.on();
    leftHeadlight.on(LOW_BEAM_BRIGHTNESS);
    rightHeadlight.on(LOW_BEAM_BRIGHTNESS);
    leftTaillight.on(LOW_BEAM_BRIGHTNESS);
    rightTaillight.on(LOW_BEAM_BRIGHTNESS);
  }
  // High Beams
  if (highBeamState == SWITCH_ON) {
    leftHeadlight.on(HIGH_BEAM_BRIGHTNESS);
    rightHeadlight.on(HIGH_BEAM_BRIGHTNESS);
  }
  // Hazards
  if (hazardState == SWITCH_ON) {
    leftHeadlight.blink(BLINKING_INTERVAL);
    rightHeadlight.blink(BLINKING_INTERVAL);
    leftTaillight.blink(BLINKING_INTERVAL);
    rightTaillight.blink(BLINKING_INTERVAL);
  }
  // Turning left
  else if (turningState == TURNING_LEFT) {
    leftHeadlight.blink(BLINKING_INTERVAL);
    leftTaillight.start(BLINKING_INTERVAL, SEQUENTIAL_INTERVAL);
  }
  // Turning right
  else if (turningState == TURNING_RIGHT) {
    rightHeadlight.blink(BLINKING_INTERVAL);
    rightTaillight.start(BLINKING_INTERVAL, SEQUENTIAL_INTERVAL);
  }
}

/**
 * Constructs a JSON representation of the current state and publishes it to the
 * MQTT client
 */
void publishCurrentState(void) {
  // Create state object and fill with state values
  cJSON *state = cJSON_CreateObject();
  cJSON_AddStringToObject(state, "lighting", lightingState.c_str());
  cJSON_AddStringToObject(state, "high_beam", highBeamState.c_str());
  cJSON_AddStringToObject(state, "turning", turningState.c_str());
  cJSON_AddStringToObject(state, "reverse", reverseState.c_str());
  cJSON_AddStringToObject(state, "fog", fogState.c_str());
  cJSON_AddStringToObject(state, "interior", interiorState.c_str());
  cJSON_AddStringToObject(state, "hazard", hazardState.c_str());
  // Convert state object to string
  char *stateStr = cJSON_PrintUnformatted(state);
  // Deallocate state object (MUST be done to avoid memory leaks)
  cJSON_Delete(state);
  // Publish state to topic
  client.publish(PUB_STATE_TOPIC, stateStr, true);
}

// *********************** SUBSCRIPTION UTILITIES ***********************

// Checks if the payload data is a valid light mode string
bool isLightModeStr(std::string data) {
  return (data == LIGHT_MODE_OFF || data == LIGHT_MODE_RUNNING ||
          data == LIGHT_MODE_LOW_BEAM);
}

// Checks if the payload data is a valid turning string
bool isTurningStr(std::string data) {
  return data == TURNING_OFF || data == TURNING_LEFT || data == TURNING_RIGHT;
}

// Checks if the payload data is a valid switch string
bool isSwitchStr(std::string data) {
  return data == SWITCH_ON || data == SWITCH_OFF;
}

/**
 * Handles validating data and applying state updates for a topic subscription
 * @param data The data string payload from the topic subscription
 * @param state Reference to a state variable that should be updated
 * @param validate A pointer to a validation function to check the data
 */
void handleSubscription(std::string data, std::string &state,
                        bool (*validate)(std::string)) {
  if (!validate(data)) {
    return;
  }
  state = data;
  updateLightsFromState();
  publishCurrentState();
}

// Alias for handleSubscripton with the isSwitchStr validation function
void handleSwitchSubscription(std::string data, std::string &state) {
  handleSubscription(data, state, &isSwitchStr);
}

// *********************** SUBSCRIPTION CALLBACKS *********************

/**
 * Set the state of all lights simultaneously
 * @param data Should be ON or OFF
 */
void setAllLights(std::string data) {
  if (!isSwitchStr(data)) {
    return;
  }
  // Turn off all lights
  if (data == SWITCH_OFF) {
    lightingState = LIGHT_MODE_OFF;
    highBeamState = SWITCH_OFF;
    turningState = TURNING_OFF;
    reverseState = SWITCH_OFF;
    fogState = SWITCH_OFF;
    interiorState = SWITCH_OFF;
    hazardState = SWITCH_OFF;
  }
  // Turn all lights on with no effects
  else if (data == SWITCH_ON) {
    lightingState = LIGHT_MODE_LOW_BEAM;
    highBeamState = SWITCH_OFF;
    turningState = TURNING_OFF;
    reverseState = SWITCH_ON;
    fogState = SWITCH_ON;
    interiorState = SWITCH_ON;
    hazardState = SWITCH_OFF;
  }
  updateLightsFromState();
  publishCurrentState();
}

/**
 * Sets the lighting mode. This only affects running lights, headlights, and
 * taillights
 * @param data Should be OFF, RUNNING, or LOW_BEAM
 */
void setLightState(std::string data) {
  handleSubscription(data, lightingState, &isLightModeStr);
}

/**
 * Sets the high beam state. This overrides the behavior of the low beam
 * lighting mode while on
 * @param data Should be ON or OFF
 */
void setHighBeamState(std::string data) {
  handleSwitchSubscription(data, highBeamState);
}

/**
 * Sets the turning state. This overrides the behavior of the low beam
 * lighting mode while on
 * @param data Should be OFF, LEFT, or RIGHT
 */
void setTurningState(std::string data) {
  handleSubscription(data, turningState, &isTurningStr);
}

/**
 * Sets the reverse lights state. This is a standalone effect
 * @param data Should be ON or OFF
 */
void setReverseState(std::string data) {
  handleSwitchSubscription(data, reverseState);
}

/**
 * Sets the fog lights state. This is a standalone effect
 * @param data Should be ON or OFF
 */
void setFogState(std::string data) { handleSwitchSubscription(data, fogState); }

/**
 * Sets the interior lights state. This is a standalone effect
 * @param data Should be ON or OFF
 */
void setInteriorState(std::string data) {
  handleSwitchSubscription(data, interiorState);
}

/**
 * Sets the hazard lights state. This effect overrides low beam, high beam, and
 * turning states
 * @param data Should be ON or OFF
 */
void setHazardState(std::string data) {
  handleSwitchSubscription(data, hazardState);
}

// Add all topic subscriptions to the MQTT client
void configureTopicSubscriptions(void) {
  client.onTopic(SUB_LIGHTING_TOPIC, &setLightState)
      .onTopic(SUB_HIGH_BEAM_TOPIC, &setHighBeamState)
      .onTopic(SUB_TURNING_TOPIC, &setTurningState)
      .onTopic(SUB_REVERSE_TOPIC, &setReverseState)
      .onTopic(SUB_FOG_TOPIC, &setFogState)
      .onTopic(SUB_INTERIOR_TOPIC, &setInteriorState)
      .onTopic(SUB_HAZARD_TOPIC, &setHazardState)
      .onTopic(SUB_ALL_TOPIC, &setAllLights);
}

/** Handle MQTT Client Connection State */
void onConnectionUpdate(bool wifiOk, bool ipOk, bool mqttOk) {
  /** Resume previous state when client is fully connected */
  if (client.isConnected()) {
    // Publish the availability
    client.publish(PUB_AVAILABLE_TOPIC, AVAILABLE_YES, true);
    // Restore and publish the existing state
    updateLightsFromState();
    publishCurrentState();
  } else {
    // Client is disconnected (turn off all lights except headlights and
    // taillights)
    fogLights.off();
    runningLights.off();
    interiorLights.off();
    reverseLights.off();
    // Blink headlights like in the hazard state
    leftHeadlight.blink(BLINKING_INTERVAL);
    rightHeadlight.blink(BLINKING_INTERVAL);
    // WiFi status uses inner taillight
    if (wifiOk) {
      leftInnerTaillight.on();
      rightInnerTaillight.on();
    } else {
      leftInnerTaillight.blink(BLINKING_INTERVAL);
      rightInnerTaillight.blink(BLINKING_INTERVAL);
    }
    // IP status uses middle taillight
    if (ipOk) {
      leftMiddleTaillight.on();
      rightMiddleTaillight.on();
    } else {
      leftMiddleTaillight.blink(BLINKING_INTERVAL);
      rightMiddleTaillight.blink(BLINKING_INTERVAL);
    }
    // MQTT status uses outer taillight (it will also just be blinking, so this
    // just helps sync it with the other lights)
    leftOuterTaillight.blink(BLINKING_INTERVAL);
    rightOuterTaillight.blink(BLINKING_INTERVAL);
  }
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

  // Set initial light state
  updateLightsFromState();

  // Configure the MQTT client and setup the LWT topic and message
  client.configure(PUB_AVAILABLE_TOPIC, AVAILABLE_NO, true);
  // Configure all of the topic subscriptions
  configureTopicSubscriptions();

  // Listen for client connection events and start the client
  client.onConnecting(&onConnectionUpdate).start();

  // Start main loop
  Utils::startLoop(&loop);
}
