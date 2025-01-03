#include "cJSON.h"
#include "settings.h" // Includes pin, topic, and behavior settings
#include <Light.h>
#include <MqttClient.h>
#include <Utils.h>
#include <cJSON.h>
#include <string>

// Export main function for C compiler
extern "C" {
void app_main(void);
}

// ********************* STATE DEFINITIONS *********************

// Light Switch Modes
#define SWITCH_ON "ON"   // Light Channel On
#define SWITCH_OFF "OFF" // Light Channel Off

// Availability
#define AVAILABLE_ONLINE "online"   // Board is available
#define AVAILABLE_OFFLINE "offline" // Board is not available

// ********************* STATES ***********************

// Gingerbread House
std::string gingerPorchState = SWITCH_OFF;
std::string gingerCandleState = SWITCH_OFF;
std::string gingerFirstState = SWITCH_OFF;
std::string gingerSecondState = SWITCH_OFF;

// ********************* MQTT CLIENT SETUP *********************
MqttClient client("christmas_village"); // MQTT Client

// ********************* LIGHT SETUP *************************

// Gingerbread House
Light gingerPorchLight(GINGERBREAD_PORCH_PIN);
Light gingerCandleLight(GINGERBREAD_CANDLE_PIN);
Light gingerFirstLight(GINGERBREAD_FIRST_PIN);
Light gingerSecondLight(GINGERBREAT_SECOND_PIN);

// ************************ STATE UPDATES **********************

/** Update all lights based on the current state */
void updateLightsFromState(void) {
  // Gingerbread House Lights
  gingerPorchState == SWITCH_ON ? gingerPorchLight.on() : gingerPorchLight.off(); // Porch Light
  gingerCandleState == SWITCH_ON ? gingerCandleLight.on() : gingerCandleLight.off(); // Candle Lights (first floor above fireplace)
  gingerFirstState == SWITCH_ON ? gingerFirstLight.on() : gingerFirstLight.off(); // First Floor Ceiling Lights
  gingerSecondState == SWITCH_ON ? gingerSecondLight.on() : gingerSecondLight.off(); // Second Floor Ceiling Lights
}

/**
 * Constructs a JSON representation of the current state and publishes it to the
 * MQTT client
 */
void publishCurrentState(void) {
  // Create state object and fill with state values
  cJSON *state = cJSON_CreateObject();

  // Gingerbread House State
  cJSON *gingerbreadState = cJSON_CreateObject();
  cJSON_AddItemToObject(state, "gingerbread", gingerbreadState);
  cJSON_AddStringToObject(gingerbreadState, "porch", gingerPorchState.c_str());
  cJSON_AddStringToObject(gingerbreadState, "candle", gingerCandleState.c_str());
  cJSON_AddStringToObject(gingerbreadState, "first", gingerFirstState.c_str());
  cJSON_AddStringToObject(gingerbreadState, "second", gingerSecondState.c_str());

  // Convert state object to string
  char *stateStr = cJSON_PrintUnformatted(state);
  // Deallocate state object (MUST be done to avoid memory leaks)
  cJSON_Delete(state);
  // Publish state to topic
  client.publish(PUB_STATE_TOPIC, stateStr, true);
}

// *********************** SUBSCRIPTION UTILITIES ***********************

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

// Gingerbread control all state
void setGingerAllState(std::string data) {
  if (!isSwitchStr(data)) {
    return;
  }
  gingerPorchState = data;
  gingerCandleState = data;
  gingerFirstState = data;
  gingerSecondState = data;
  updateLightsFromState();
  publishCurrentState();
}

// Gingerbread State Callbacks
void setGingerPorchState(std::string data) {
  handleSwitchSubscription(data, gingerPorchState);
}

void setGingerCandleState(std::string data) {
  handleSwitchSubscription(data, gingerCandleState);
}

void setGingerFirstState(std::string data) {
  handleSwitchSubscription(data, gingerFirstState);
}

void setGingerSecondState(std::string data) {
  handleSwitchSubscription(data, gingerSecondState);
}

// Add all topic subscriptions to the MQTT client
void configureTopicSubscriptions(void) {
  // Gingerbread House Topics
  client.onTopic(SUB_GINGERBREAD_ALL_TOPIC, &setGingerAllState)
      .onTopic(SUB_GINGERBREAD_PORCH_TOPIC, &setGingerPorchState)
      .onTopic(SUB_GINGERBREAD_CANDLE_TOPIC, &setGingerCandleState)
      .onTopic(SUB_GINGERBREAD_FIRST_TOPIC, &setGingerFirstState)
      .onTopic(SUB_GINGERBREAD_SECOND_TOPIC, &setGingerSecondState);
}

/** Handle MQTT Client Connection State */
void onConnectionUpdate(bool wifiOk, bool ipOk, bool mqttOk) {
  /** Resume previous state when client is fully connected */
  if (client.isConnected()) {
    // Publish the availability
    client.publish(PUB_AVAILABLE_TOPIC, AVAILABLE_ONLINE, true);
    // Restore and publish the existing state
    updateLightsFromState();
    publishCurrentState();
  } else {
    // Client is disconnected so turn off all lights and blink the candles
    gingerPorchLight.blink();
    gingerFirstLight.off();
    gingerSecondLight.off();
    gingerCandleLight.off();
  }
}

/**
 * Main loop function for lighting effects
 */
void loop(unsigned int now) {
  gingerPorchLight.loop(now); // Blinks while trying to establish a connection
}

/**
 * Application entrypoint. Configure lights, MQTT client, and start the main
 * effects loop
 */
void app_main(void) {
  // Set initial light state
  updateLightsFromState();

  // Configure the MQTT client and setup the LWT topic and message
  client.configure(PUB_AVAILABLE_TOPIC, AVAILABLE_OFFLINE, true);
  // Configure all of the topic subscriptions
  configureTopicSubscriptions();

  // Listen for client connection events and start the client
  client.onConnecting(&onConnectionUpdate).start();

  // Start main loop
  Utils::startLoop(&loop);
}
