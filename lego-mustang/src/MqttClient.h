#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "Interval.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <functional>

#define DEFAULT_CONNECTION_CHECK_INTERVAL 1000
#define CONNECTING_CALLBACK_SIGNATURE std::function<void(unsigned int, bool)>
#define CONNECTED_CALLBACK_SIGNATURE std::function<void(unsigned int)>

class MqttClient {
private:
  // Clients
  WiFiClient wifiClient;   // WiFi Client
  PubSubClient mqttClient; // MQTT Client

  // Intervals
  Interval connectionCheckInterval;

  // Callbacks
  CONNECTING_CALLBACK_SIGNATURE connectingCallback = NULL;
  CONNECTED_CALLBACK_SIGNATURE connectedCallback = NULL;

  // State
  bool firstCheck = true;
  bool connecting = true;

public:
  MqttClient();
  MqttClient(int checkInterval);

  void setup();

  void onConnecting(CONNECTING_CALLBACK_SIGNATURE);

  void onConnected(CONNECTED_CALLBACK_SIGNATURE);

  bool loop(unsigned int now);
};

#endif