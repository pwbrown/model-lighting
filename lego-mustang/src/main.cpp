#include "Interval.h"
#include "MqttClient.h"
#include <Arduino.h>
#include <Light.h>

Light testLight(BUILTIN_LED); // Status light
MqttClient client(5000);      // MQTT Client with a 5 second check interval

Interval connectingInterval(250); // Blink interval for the status LED

void onConnecting(unsigned int now, bool first) {
  if (first) {
    connectingInterval.reset(now);
  }
  if (connectingInterval.check(now)) {
    testLight.toggle();
  }
}

void onConnected(unsigned int) { testLight.on(); }

void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);

  client.setup();
  client.onConnecting(onConnecting);
  client.onConnected(onConnected);
}

void loop() {
  unsigned int now = millis();
  client.loop(now);
}