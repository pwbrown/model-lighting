#include <Arduino.h>
#include <Light.h>
#include <Secrets.h>

#include "Internal.h"
#include "MqttClient.h"

Light testLight(BUILTIN_LED);

MqttClient mqttClient("LegoMustangClient");

Interval connectingInterval(500);

// Called any time the remote client is actively trying to connect
void connecting(unsigned int now, bool first) {
  if (first) {
    testLight.on();
  }
  if (connectingInterval.check(now)) {
    testLight.toggle();
  }
}

// Called when a connection is established
void connected(unsigned int) { testLight.on(); }

void setup() {
  // Setup serial port
  Serial.begin(115200);

  // Setup LEDs
  testLight.setup();

  // Setup WiFi and MQTT and register event callbacks
  mqttClient.setup(WIFI_SSID, WIFI_PASSWORD, MQTT_HOST, MQTT_PORT)
      .onConnecting(connecting)
      .onConnected(connected);
}

void loop() {
  // Create a shared timestamp to ensure synchronized actions
  unsigned int now = millis();
  // Execute client loop to maintain connection and listen for events
  mqttClient.loop(now);
  delay(250);
}