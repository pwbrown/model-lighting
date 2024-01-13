#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Interval.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <functional>

#define CONNECTING_CALLBACK_SIGNATURE                                          \
  std::function<void(unsigned int now, bool first)> connectingCallback
#define CONNECTED_CALLBACK_SIGNATURE                                           \
  std::function<void(unsigned int now)> connectedCallback

class MqttClient {
private:
  // Clients
  WiFiClient wifiClient;   // WiFi Client
  PubSubClient mqttClient; // MQTT Client
  char *clientName;        // MQTT Client Name

  // Event Callbacks
  CONNECTING_CALLBACK_SIGNATURE =
      NULL;                            // Called when actively trying to connect
  CONNECTED_CALLBACK_SIGNATURE = NULL; // Called once for every time a
                                       // connection is reestablished

  // Intervals
  Interval connInterval;

  // State
  bool isConnecting = true;

  // Connection status checks
  bool wifiConnected();
  bool mqttConnected();

public:
  MqttClient(char *clientName);
  MqttClient(char *clientName, int check_interval);

  MqttClient &setup(char *wifiSsid, char *wifiPassword, char *mqttHost,
                    int mqttPort);

  MqttClient &onConnecting(CONNECTING_CALLBACK_SIGNATURE);
  MqttClient &onConnected(CONNECTED_CALLBACK_SIGNATURE);

  bool loop(unsigned int now);
};

#endif