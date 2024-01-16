#include "MqttClient.h"
#include <Secrets.h>

MqttClient::MqttClient() {
  wifiClient = WiFiClient();
  mqttClient = PubSubClient(wifiClient);
  connectionCheckInterval = Interval(DEFAULT_CONNECTION_CHECK_INTERVAL);
}

MqttClient::MqttClient(int check_interval) {
  wifiClient = WiFiClient();
  mqttClient = PubSubClient(wifiClient);
  connectionCheckInterval = Interval(check_interval);
}

void MqttClient::setup() {
  // Ensure WiFi is running in station mode
  WiFi.mode(WIFI_STA);
  // Begin the WiFi connection using secrets values
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void MqttClient::onConnecting(CONNECTING_CALLBACK_SIGNATURE cb) {
  connectingCallback = cb;
}

void MqttClient::onConnected(CONNECTED_CALLBACK_SIGNATURE cb) {
  connectedCallback = cb;
}

bool MqttClient::loop(unsigned int now) {
  bool justDisconnected = false;
  if (connectionCheckInterval.check(now)) {
    bool wifiOk = WiFi.status() == WL_CONNECTED;
    if (wifiOk) {
      if (connecting) {
        connecting = false;
        Serial.println("WiFi Connection Established");
        Serial.print("Local IP: ");
        Serial.println(WiFi.localIP());
        if (connectedCallback != NULL) {
          connectedCallback(now);
        }
      }
    } else {
      if (!connecting || firstCheck) {
        connecting = true;
        justDisconnected = true;
        Serial.println("WiFi Disconnected");
        Serial.println("Attempting to Connect...");
      } else {
        WiFi.reconnect();
      }
    }
    if (firstCheck) {
      firstCheck = false;
    }
  }
  if (connecting) {
    if (connectingCallback != NULL) {
      connectingCallback(now, justDisconnected);
    }
    return false;
  } else {
    return true;
  }
}