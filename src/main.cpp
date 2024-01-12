#include "light.h"
#include "secrets.h"

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

Light testLight(BUILTIN_LED);

WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);

bool isConnecting = true;
unsigned int msSinceLightChange = 0;
const int LIGHT_INTERVAL = 500;

// Called any time the remote client is actively trying to connect
void connecting(bool first) {
  if (first) {
    testLight.on();
  }
  unsigned int now = millis();
  if (now > msSinceLightChange + LIGHT_INTERVAL) {
    msSinceLightChange = now;
    testLight.toggle();
  }
}

// Called when a connection is established
void connected() { testLight.on(); }

unsigned int msSinceLastCheck = 0;
const int CONNECTION_CHECK_INTERVAL = 1000;

// Should be called on every loop to ensure that the connection is stable
bool ensureConnected() {
  unsigned int now = millis();
  bool first = false;
  if (now > msSinceLastCheck + CONNECTION_CHECK_INTERVAL) {
    Serial.println("Checking");
    msSinceLastCheck = now;
    if (WiFi.status() == WL_CONNECTED &&
        (pubSubClient.connected() || pubSubClient.connect("ESP32Client"))) {
      if (isConnecting) {
        Serial.println("Connected");
        isConnecting = false;
        connected();
        return true;
      }
    } else if (!isConnecting) {
      Serial.println("Disconnected");
      isConnecting = true;
      first = true;
    }
  }
  if (isConnecting) {
    connecting(first);
    return false;
  } else {
    return true;
  }
}

void setup() {
  // Setup serial port
  Serial.begin(115200);

  // Setup LEDs
  testLight.setup();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  pubSubClient.setServer(MQTT_HOST, MQTT_PORT);
}

void loop() {
  if (ensureConnected()) {
    pubSubClient.loop();
  }
}