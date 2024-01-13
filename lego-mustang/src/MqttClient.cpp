#include "MqttClient.h"

// Initialize WiFi and MQTT clients
MqttClient::MqttClient(char *clientName) {
  wifiClient = WiFiClient();
  mqttClient = PubSubClient(wifiClient);
  connInterval = Interval();
  this->clientName = clientName;
}

// Initialize WiFi and MQTT clients with custom check interval
MqttClient::MqttClient(char *clientName, int check_interval) {
  wifiClient = WiFiClient();
  mqttClient = PubSubClient(wifiClient);
  connInterval = Interval(check_interval);
  this->clientName = clientName;
}

// Setup starts the WiFi connection and sets the MQTT server details
MqttClient &MqttClient::setup(char *wifiSsid, char *wifiPassword,
                              char *mqttHost, int mqttPort) {
  WiFi.begin(wifiSsid, wifiPassword);
  mqttClient.setServer(mqttHost, mqttPort);

  return *this;
}

// Register a callback for connecting events
MqttClient &MqttClient::onConnecting(CONNECTING_CALLBACK_SIGNATURE) {
  this->connectingCallback = connectingCallback;

  return *this;
}

// Register a callback for connected events
MqttClient &MqttClient::onConnected(CONNECTED_CALLBACK_SIGNATURE) {
  this->connectedCallback = connectedCallback;

  return *this;
}

// Checks if the WiFi connection is up
bool MqttClient::wifiConnected() { return WiFi.status() == WL_CONNECTED; }

// Checks if the MQTT connection is up and attempts to reconnect
bool MqttClient::mqttConnected() {
  return mqttClient.connected() || mqttClient.connect(clientName);
}

// Designed to by called by the loop method to maintain a connection
// and listen for MQTT messages
bool MqttClient::loop(unsigned int now) {
  bool first = false; // Indicates if the connection function is called for the
                      // first time since disconnecting
  // Check for connection status on an interval
  if (connInterval.check(now)) {
    if (wifiConnected() && mqttConnected()) {
      if (isConnecting) {
        isConnecting = false;
        Serial.println("MQTT Client Connected");
        if (connectedCallback != NULL) {
          connectedCallback(now);
        }
      }
    } else if (!isConnecting) {
      isConnecting = true;
      first = true;
      Serial.println("MQTT Client Disconnected");
      Serial.print("Attempting to Reconnect.");
    }
  }
  // Report connecting status or run MQTT client loop
  if (isConnecting) {
    if (connectingCallback != NULL) {
      connectingCallback(now, first);
    }
    return false;
  } else {
    return true; // mqttClient.loop();
  }
}