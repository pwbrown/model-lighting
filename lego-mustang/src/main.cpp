// #include "Interval.h"
// #include "MqttClient.h"
// #include <Arduino.h>
// #include <Light.h>

// Light testLight(BUILTIN_LED);     // Status light
// MqttClient client("LegoMustang"); // MQTT Client

// Interval connectingInterval(250); // Blink interval for the status LED

// void onConnecting(unsigned int now, bool first) {
//   if (first) {
//     connectingInterval.reset(now);
//   }
//   if (connectingInterval.check(now)) {
//     testLight.toggle();
//   }
// }

// void onConnected(unsigned int) { testLight.on(); }

// void setup() {
//   Serial.begin(115200);

//   pinMode(BUILTIN_LED, OUTPUT);

//   client.setup()
//       .setKeepAliveInterval(1000)
//       .onConnecting(onConnecting)
//       .onConnected(onConnected);
// }

// void loop() { client.loop(millis()); }

#include "DimmableLight.h"
#include "Light.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

extern "C" {
void app_main(void);
}

const char *TAG = "Main";

DimmableLight testLight(2, 0);

void app_main(void) {
  DimmableLight::configureGlobalTimer();
  testLight.configure();

  bool isBright = false;
  while (1) {
    if (!isBright) {
      testLight.on(100);
    } else {
      testLight.on(25);
    }
    isBright = !isBright;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}