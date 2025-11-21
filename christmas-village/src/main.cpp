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

std::string allState = SWITCH_OFF;
std::string gingerbreadState = SWITCH_OFF;
std::string honeydukesState = SWITCH_OFF;
std::string threebroomsticksState = SWITCH_OFF;
std::string toystoreState = SWITCH_OFF;
std::string musicstoreState = SWITCH_OFF;
std::string trolleyState = SWITCH_OFF;
std::string treesState = SWITCH_OFF;
std::string lampsState = SWITCH_OFF;

// ********************* MQTT CLIENT SETUP *********************
MqttClient client("christmas_village"); // MQTT Client

// ********************* LIGHT SETUP *************************

int pwmChannel = 0;

// Gingerbread House
Light gingerbreadLight(GINGERBREAD_PIN);
Light honeydukesLight(HONEYDUKES_PIN);
Light threebrommsticksLight(THREEBROOMSTICKS_PIN);
Light toystoreLight(TOYSTORE_PIN);
Light musicstoreLight(MUSICSTORE_PIN);
Light trolleyLight(TROLLEY_PIN);
Light treesLight(TREES_PIN, pwmChannel++);
Light lampsLight(LAMPS_PIN);

// ************************ STATE UPDATES **********************

/** Update all lights based on the current state */
void updateLightsFromState(void) {
  gingerbreadState == SWITCH_ON ? gingerbreadLight.on() : gingerbreadLight.off(); // Gingerbread house
  honeydukesState == SWITCH_ON ? honeydukesLight.on() : honeydukesLight.off(); // Honeydukes
  threebroomsticksState == SWITCH_ON ? threebrommsticksLight.on() : threebrommsticksLight.off(); // Three Broomsticks
  toystoreState == SWITCH_ON ? toystoreLight.on() : toystoreLight.off(); // Toy Store
  musicstoreState == SWITCH_ON ? musicstoreLight.on() : musicstoreLight.off(); // Music Store
  trolleyState == SWITCH_ON ? trolleyLight.on() : trolleyLight.off(); // Trolley
  treesState == SWITCH_ON ? treesLight.on(25) : treesLight.off(); // Trees (custom brightness)
  lampsState == SWITCH_ON ? lampsLight.on() : lampsLight.off(); // Lamps
}

/**
 * Updates the ALL lights state in response to individual light state changes.
 * 
 * Intended behavior: Any light on means "ALL" lights state should be on (best behavior for automation)
 */
void updateAllStateFromSwitchChange(void) {
  bool anyLightIsOn = (
    gingerbreadState == SWITCH_ON ||
    honeydukesState == SWITCH_ON ||
    threebroomsticksState == SWITCH_ON ||
    toystoreState == SWITCH_ON ||
    musicstoreState == SWITCH_ON ||
    trolleyState == SWITCH_ON ||
    treesState == SWITCH_ON ||
    lampsState == SWITCH_ON
  );
  if (anyLightIsOn) {
    allState = SWITCH_ON;
  } else {
    allState = SWITCH_OFF;
  }
}

/**
 * Constructs a JSON representation of the current state and publishes it to the
 * MQTT client
 */
void publishCurrentState(void) {
  // Create state object and fill with state values
  cJSON *state = cJSON_CreateObject();
  
  cJSON_AddStringToObject(state, "all", allState.c_str());
  cJSON_AddStringToObject(state, "gingerbread", gingerbreadState.c_str());
  cJSON_AddStringToObject(state, "honeydukes", honeydukesState.c_str());
  cJSON_AddStringToObject(state, "threebroomsticks", threebroomsticksState.c_str());
  cJSON_AddStringToObject(state, "toystore", toystoreState.c_str());
  cJSON_AddStringToObject(state, "musicstore", musicstoreState.c_str());
  cJSON_AddStringToObject(state, "trolley", trolleyState.c_str());
  cJSON_AddStringToObject(state, "trees", treesState.c_str());
  cJSON_AddStringToObject(state, "lamps", lampsState.c_str());

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
  updateAllStateFromSwitchChange();
  updateLightsFromState();
  publishCurrentState();
}

// Alias for handleSubscripton with the isSwitchStr validation function
void handleSwitchSubscription(std::string data, std::string &state) {
  handleSubscription(data, state, &isSwitchStr);
}

// *********************** SUBSCRIPTION CALLBACKS *********************

void setAllState(std::string data) {
  if (!isSwitchStr(data)) {
    return;
  }
  allState = data;
  // Update all lights with new state
  gingerbreadState = data;
  honeydukesState = data;
  threebroomsticksState = data;
  toystoreState = data;
  musicstoreState = data;
  trolleyState = data;
  treesState = data;
  lampsState = data;
  // Finalize updates
  updateLightsFromState();
  publishCurrentState();
}

// Gingerbread State
void setGingerbreadState(std::string data) {
  handleSwitchSubscription(data, gingerbreadState);
}

// Honeydukes State
void setHoneydukesState(std::string data) {
  handleSwitchSubscription(data, honeydukesState);
}

// Three Broomsticks State
void setThreebroomsticksState(std::string data) {
  handleSwitchSubscription(data, threebroomsticksState);
}

// Toy Store State
void setToystoreState(std::string data) {
  handleSwitchSubscription(data, toystoreState);
}

// Music Store State
void setMusicstoreState(std::string data) {
  handleSwitchSubscription(data, musicstoreState);
}

// Trolley State
void setTrolleyState(std::string data) {
  handleSwitchSubscription(data, trolleyState);
}

// Trees State
void setTreesState(std::string data) {
  handleSwitchSubscription(data, treesState);
}

// Lamps State
void setLampsState(std::string data) {
  handleSwitchSubscription(data, lampsState);
}

// Add all topic subscriptions to the MQTT client
void configureTopicSubscriptions(void) {
  // Gingerbread House Topics
  client.onTopic(SUB_ALL_TOPIC, &setAllState)
      .onTopic(SUB_GINGERBREAD_TOPIC, &setGingerbreadState)
      .onTopic(SUB_HONEYDUKES_TOPIC, &setHoneydukesState)
      .onTopic(SUB_THREEBROOMSTICKS_TOPIC, &setThreebroomsticksState)
      .onTopic(SUB_TOYSTORE_TOPIC, &setToystoreState)
      .onTopic(SUB_MUSICSTORE_TOPIC, &setMusicstoreState)
      .onTopic(SUB_TROLLEY_TOPIC, &setTrolleyState)
      .onTopic(SUB_TREES_TOPIC, &setTreesState)
      .onTopic(SUB_LAMPS_TOPIC, &setLampsState);
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
    gingerbreadLight.blink();
  }
}

/**
 * Main loop function for lighting effects
 */
void loop(unsigned int now) {
  gingerbreadLight.loop(now); // Blinks while trying to establish a connection
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
  client.configure(PUB_AVAILABLE_TOPIC, AVAILABLE_OFFLINE, true);
  // Configure all of the topic subscriptions
  configureTopicSubscriptions();

  // Listen for client connection events and start the client
  client.onConnecting(&onConnectionUpdate).start();

  // Start main loop
  Utils::startLoop(&loop);
}
