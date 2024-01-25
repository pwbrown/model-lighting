#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "mqtt_client.h"
#include <functional>
#include <map>
#include <string>

// Logging tag
static const char *MQTT_CLIENT_TAG = "mqtt_client";

#define CONNECTING_CALLBACK                                                    \
  std::function<void(bool, bool,                                               \
                     bool)> // Callback signature for connecting events
#define SUBSCRIPTION_CALLBACK                                                  \
  std::function<void(std::string)> // Callback signature for topic subscriptions
#define SUBSCRIPTION_MAP                                                       \
  std::map<std::string,                                                        \
           SUBSCRIPTION_CALLBACK> // Topic subscription map definition

/**
 * MqttClient is an abstraction layer on top of the underlying
 * ESP IDF WiFi and MQTT clients. It simplifies the initialization process
 * and provides quick methods for registering topic subscriptions that will
 * be automatically recovered across mqtt sessions
 */
class MqttClient {
public:
  MqttClient(const char *clientId);

  /**
   * Configures NVS (non-volatile storage), WiFi, and MQTT
   */
  MqttClient &configure(void);

  /**
   * Registers the connecting callback
   * @param callback The callback function for connecting events
   */
  MqttClient &onConnecting(CONNECTING_CALLBACK callback);

  /**
   * Starts connecting to WiFi and the MQTT broker
   */
  MqttClient &start(void);

  /**
   * Indicates if the client is fully connected to the MQTT broker
   */
  bool isConnected(void);

  /**
   * Registers a topic subscription for the MQTT client to listen to
   * @param topic The name of the topic to subscribe to
   * @param callback A callback to execute when a message on the topic comes in
   */
  MqttClient &onTopic(std::string topic, SUBSCRIPTION_CALLBACK callback);

  /**
   * Publish data string on a topic
   * @param topic The name of the topic to publish to
   * @param data A data string to send
   * @param retain Whether the MQTT broker should retain the message
   */
  MqttClient &publish(std::string topic, std::string data, bool retain = false);

  /**
   * Called by the ESP event loop in reponse to background WiFi and MQTT events.
   * Shouldn't be called directly by the user
   */
  void __handleEvents__(esp_event_base_t eventBase, int32_t eventId,
                        void *eventData);

private:
  // Clients
  esp_mqtt_client_handle_t _mqttClient = NULL; // MQTT Client

  // State
  const char *_clientId;       // MQTT Client Id
  bool _wifiConnected = false; // Indicates if the client is connected to wifi
                               // and has an IP address
  bool _ipReceived = false;    // Indicates if an IP address has been received
  bool _mqttConnected = false; // Indicates if the mqtt client is connected
                               // to the broker and is ready
                               // to send and receive messages

  // Callbacks
  CONNECTING_CALLBACK _connectingCallback =
      NULL; // Called without delay while disconnected

  // Subscriptions
  SUBSCRIPTION_MAP _subscriptions; // Holds all subscription callbacks

  /** Configure Non Volatile Storage for WiFi configuration */
  void configureNvs(void);

  /** Configure Wifi client */
  void configureWifi(void);

  /** Configure Mqtt client */
  void configureMqtt(void);

  /** Handle WiFi events */
  void handleWifiEvent(int32_t eventId, void *eventData);

  /** Handle IP events*/
  void handleIpEvent(int32_t eventId, void *eventData);

  /** Handle MQTT events */
  void handleMqttEvent(int32_t eventId, void *eventData);

  /** Reports the connecting status through the connecting callback */
  void reportConnectingStatus(void);
};

#endif