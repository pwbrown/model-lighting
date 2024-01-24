#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "mqtt_client.h"
#include <functional>
#include <map>
#include <string>

static const char *MQTT_CLIENT_TAG = "mqtt_client";

#define CALLBACK_SIGNATURE std::function<void()>
#define SUBSCRIPTION_CALLBACK std::function<void(std::string)>
#define SUBSCRIPTION_MAP std::map<std::string, SUBSCRIPTION_CALLBACK>

/**
 * MqttClient is an abstraction layer on top of the underlying
 * ESP IDF WiFi and MQTT clients. It simplifies the initialization process
 * and provides quick methods for registering topic subscriptions that will
 * be automatically recovered across mqtt sessions
 */
class MqttClient {
public:
  /**
   * Configures the WiFi and MQTT clients
   */
  MqttClient &configure(const char *clientId);

  /**
   * Registers the connecting callback
   * @param callback The callback function for connecting events
   */
  MqttClient &onConnecting(CALLBACK_SIGNATURE callback);

  /**
   * Registers the connected callback
   * @param callback The callback function for connected events
   */
  MqttClient &onConnected(CALLBACK_SIGNATURE callback);

  /**
   * Starts the connection process
   */
  MqttClient &start(void);

  /**
   * Indicates if the client is fully connected to the MQTT broker
   */
  bool connected(void);

  /**
   * Registers a topic subscription for the MQTT client to listen to
   */
  MqttClient &onTopic(std::string topic, SUBSCRIPTION_CALLBACK callback);

  /**
   * Publish data string on a topic
   */
  MqttClient &publish(std::string topic, std::string data, bool retain = false);

  /**
   * Called by the ESP event loop in reponse to background events. Should be
   * called directly
   */
  void __handleEvents__(esp_event_base_t eventBase, int32_t eventId,
                        void *eventData);

private:
  // Clients
  esp_mqtt_client_handle_t mqttClient = NULL; // MQTT Client

  // State
  bool wifiConnected =
      false; // Indicates if the client is connected to wifi and has an IP
  bool mqttConnected =
      false; // Indicates if the mqtt client is connected to the broker

  // Callbacks
  CALLBACK_SIGNATURE connectingCallback =
      NULL; // Called without delay while disconnected
  CALLBACK_SIGNATURE connectedCallback =
      NULL; // Called once for every time a connection is reestablished

  // Subscriptions
  SUBSCRIPTION_MAP subscriptions;

  /** Configure Non Volatile Storage for WiFi configuration */
  void configureNvs(void);

  /** Configure Wifi client */
  void configureWifi(void);

  /** Configure Mqtt client*/
  void configureMqtt(const char *clientId);
};

#endif