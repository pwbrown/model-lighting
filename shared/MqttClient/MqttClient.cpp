#include "MqttClient.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "mqtt_client.h"
#include "nvs_flash.h"
#include <Secrets.h>
#include <any>
#include <string>

#define log(format, __VA_ARGS__...)                                            \
  ESP_LOGI(MQTT_CLIENT_TAG, format, __VA_ARGS__)

/**
 * Handles forwarding WiFi and MQTT events back to the client. The function is
 * formatted the way that the event loop handler expects
 * @param arg This will be an instance of the client
 * @param eventBase The event's base type
 * @param eventId The id of the specific event fired
 * @param eventData The data passed by the specific event
 */
void forwardingEventHandler(void *arg, esp_event_base_t eventBase,
                            int32_t eventId, void *eventData) {
  // Argument is an instance of the MQTT client
  MqttClient *client = (MqttClient *)arg;
  client->__handleEvents__(eventBase, eventId, eventData);
}

// Create MQTT client with a specific client id
MqttClient::MqttClient(const char *clientId) : _clientId{clientId} {}

// Start the WiFi and MQTT Clients and maintain the connection
MqttClient &MqttClient::start() {
  // Start WiFi client
  ESP_ERROR_CHECK(esp_wifi_start());

  return *this;
}

// Register connecting callback
MqttClient &MqttClient::onConnecting(CONNECTING_CALLBACK callback) {
  _connectingCallback = callback;

  return *this;
}

// Indicates if the client is fully connected
bool MqttClient::isConnected(void) {
  return _wifiConnected && _ipReceived && _mqttConnected;
}

// Register topic subscription
MqttClient &MqttClient::onTopic(std::string topic,
                                SUBSCRIPTION_CALLBACK callback) {
  _subscriptions[topic] = callback;
  // If the subscription is made after the client is already connected, initiate
  // the subscription now
  if (isConnected()) {
    esp_mqtt_client_subscribe(_mqttClient, topic.c_str(), 0);
  }

  return *this;
}

// Publish data on topic
MqttClient &MqttClient::publish(std::string topic, std::string data,
                                bool retain) {
  if (isConnected()) {
    esp_mqtt_client_publish(_mqttClient, topic.c_str(), data.c_str(), 0, 1,
                            retain ? 1 : 0);
  }

  return *this;
}

// Handles all WiFi and MQTT events forwarded by the forwardingEventHandler
void MqttClient::__handleEvents__(esp_event_base_t eventBase, int32_t eventId,
                                  void *eventData) {
  if (eventBase == WIFI_EVENT) {
    handleWifiEvent(eventId, eventData);
  } else if (eventBase == IP_EVENT) {
    handleIpEvent(eventId, eventData);
  } else {
    handleMqttEvent(eventId, eventData);
  }
}

// Handle WiFi Event
void MqttClient::handleWifiEvent(int32_t eventId, void *eventData) {
  // WiFi Station Started Event (can connect now)
  if (eventId == WIFI_EVENT_STA_START) {
    log("WiFi Station Ready to Connect");
    esp_wifi_connect();
  }
  // WiFi Station Connected (should be getting an IP soon)
  else if (eventId == WIFI_EVENT_STA_CONNECTED) {
    log("WiFi Station Connected");
    // Only update wifi status as connected
    updateAndReportStatus(true, _ipReceived, _mqttConnected);
  }
  // WiFi Station disconnected (attempt to reconnect)
  else if (eventId == WIFI_EVENT_STA_DISCONNECTED) {
    log("WiFi Disconnected. Attempting to Reconnect");
    // Mark all statuses as disconnected
    updateAndReportStatus(false, false, false);
    esp_wifi_connect();
  }
}

// Handle IP Event
void MqttClient::handleIpEvent(int32_t eventId, void *eventData) {
  if (eventId == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)eventData;
    log("Got IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
    // Only report wifi and ip statuses
    updateAndReportStatus(true, true, _mqttConnected);
    // Start the MQTT Client connection
    esp_mqtt_client_start(_mqttClient);
  }
}

// Handle MQTT Event
void MqttClient::handleMqttEvent(int32_t eventId, void *eventData) {
  esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)eventData;

  // MQTT Client Connected (subscribe/resubscribe to topics)
  if (eventId == MQTT_EVENT_CONNECTED) {
    log("MQTT Client Connected");
    // Only report mqtt status
    updateAndReportStatus(_wifiConnected, _ipReceived, true);
    // Iterate over all subscription keys (topics) and subscribe to them
    for (SUBSCRIPTION_MAP::iterator iter = _subscriptions.begin();
         iter != _subscriptions.end(); ++iter) {
      esp_mqtt_client_subscribe_single(_mqttClient, iter->first.c_str(), 0);
    }
  }
  // MQTT Client Disconnected (wait to reconnect)
  else if (eventId == MQTT_EVENT_DISCONNECTED) {
    log("MQTT Client Disconnected");
    // Only report MQTT status
    updateAndReportStatus(_wifiConnected, _ipReceived, false);
  }
  // Data received for subscribed topic
  else if (eventId == MQTT_EVENT_DATA) {
    std::string topic;
    topic.assign(event->topic, (size_t)event->topic_len);
    // Execute subscription callback if available
    if (_subscriptions.contains(topic)) {
      std::string data;
      data.assign(event->data, (size_t)event->data_len);
      _subscriptions.at(topic)(data);
    }
  }
  // MQTT Error
  else if (eventId == MQTT_EVENT_ERROR) {
    log("MQTT Error occurred");
  }
}

// Report connecting status through callback
void MqttClient::updateAndReportStatus(bool wifiOk, bool ipOk, bool mqttOk) {
  bool wifiChanged = wifiOk != _wifiConnected;
  bool ipChanged = ipOk != _ipReceived;
  bool mqttChanged = mqttOk != _mqttConnected;
  _wifiConnected = wifiOk;
  _ipReceived = ipOk;
  _mqttConnected = mqttOk;
  // Only report status if a callback has been set, and the status actually
  // changed
  if (_connectingCallback != NULL &&
      (wifiChanged || ipChanged || mqttChanged)) {
    _connectingCallback(_wifiConnected, _ipReceived, _mqttConnected);
  }
}

// Configure and setup Non Volatile Storage for use by the WiFi client to save
// configuration
void MqttClient::configureNvs(void) {
  // Initialize and clear NVS if possible
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}

// Setup and configure WiFi client and listen to events
void MqttClient::configureWifi(void) {
  // Initialize NETIF (abstraction layer for TCP/IP)
  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_init());

  // Create the default event loop to handle WiFi events
  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_create_default());

  // Initialize default WiFi Station
  esp_netif_create_default_wifi_sta();

  // Initialize WiFi with the default config
  wifi_init_config_t defaultConfig = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&defaultConfig));

  // Register WiFi events to handler (listens for connection state)
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &forwardingEventHandler, this, NULL));

  // Register IP events to handler (listens for IP assignment)
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &forwardingEventHandler, this, NULL));

  // Update WiFi mode and config
  wifi_config_t wifiConfig = {
      .sta =
          {
              .ssid = WIFI_SSID,
              .password = WIFI_PASSWORD,
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
}

/**
 * Setup and configure MQTT client and listen to events
 */
void MqttClient::configureMqtt(void) {
  // Configure and initialize MQTT client
  esp_mqtt_client_config_t mqttConfig = {
      .broker =
          {
              .address =
                  {
                      .hostname = MQTT_HOST,
                      .transport = MQTT_TRANSPORT_OVER_TCP,
                      .port = MQTT_PORT,
                  },
          },
      .credentials = {.client_id = _clientId},
  };
  _mqttClient = esp_mqtt_client_init(&mqttConfig);

  // Register MQTT events to handler
  esp_mqtt_client_register_event(_mqttClient,
                                 (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID,
                                 &forwardingEventHandler, this);
}

/**
 * Configure all clients and services
 */
MqttClient &MqttClient::configure(void) {
  configureNvs();
  configureWifi();
  configureMqtt();

  return *this;
}