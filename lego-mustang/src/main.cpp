#include "pins.h"
#include <BlinkingLight.h>
#include <DimmableLight.h>
#include <SequentialLights.h>
// #include <Light.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <MqttClient.h>
#include <string>

extern "C" {
void app_main(void);
}

// MQTT Client for communication
MqttClient client;

// Dimmable Lights
DimmableLight leftHeadlight(LEFT_HEADLIGHT, 0);
DimmableLight rightHeadlight(RIGHT_HEADLIGHT, 1);
DimmableLight leftOuterTaillight(LEFT_OUTER_TAILLIGHT, 2);
DimmableLight leftMiddleTaillight(LEFT_MIDDLE_TAILLIGHT, 3);
DimmableLight leftInnerTaillight(LEFT_INNER_TAILLIGHT, 4);
DimmableLight rightOuterTaillight(RIGHT_OUTER_TAILLIGHT, 5);
DimmableLight rightMiddleTaillight(RIGHT_MIDDLE_TAILLIGHT, 6);
DimmableLight rightInnerTaillight(RIGHT_INNER_TAILLIGHT, 7);

// Blinking Lights
BlinkingLight leftHeadlightBlinking(leftHeadlight, 500);
BlinkingLight rightHeadlightBlinking(rightHeadlight, 500);

// Sequential Lights
SequentialLights leftTaillightSequential(leftOuterTaillight,
                                         leftMiddleTaillight,
                                         leftInnerTaillight, 500, 100);
SequentialLights rightTaillightSequential(rightOuterTaillight,
                                          rightMiddleTaillight,
                                          rightInnerTaillight, 500, 100);

// // Non-Dimmable Lights
// Light fogLights(FOG_LIGHTS);
// Light runningLights(RUNNING_LIGHTS);
// Light interiorLights(INTERIOR_LIGHTS);

// void configureLights(void) {
//   leftHeadlight.configure();
//   rightHeadlight.configure();
//   leftOuterTaillight.configure();
//   leftMiddleTaillight.configure();
//   leftInnerTaillight.configure();
//   rightOuterTaillight.configure();
//   rightMiddleTaillight.configure();
//   rightInnerTaillight.configure();
// }

bool hazardsOn = false;

void turnOnHazards(void) {
  hazardsOn = true;
  leftHeadlightBlinking.startBlinking(50);
  rightHeadlightBlinking.startBlinking(50);
  leftTaillightSequential.startBlinking(50);
  rightTaillightSequential.startBlinking(50);
}

void turnOffHazards(void) {
  hazardsOn = false;
  leftHeadlightBlinking.off();
  rightHeadlightBlinking.off();
  leftTaillightSequential.off();
  rightTaillightSequential.off();
}

void setHazardState(std::string state) {
  if (state == "ON" && !hazardsOn) {
    turnOnHazards();
  } else if (state != "ON" && hazardsOn) {
    turnOffHazards();
  }
  client.publish("/lego/mustang/hazards/state", hazardsOn ? "ON" : "OFF");
}

void loop(unsigned int now) {
  leftHeadlightBlinking.loop(now);
  rightHeadlightBlinking.loop(now);
  leftTaillightSequential.loop(now);
  rightTaillightSequential.loop(now);
}

void app_main(void) {
  // Start MQTT Client
  client.configure("LegoMustang")
      .onTopic("/lego/mustang/hazards/set", &setHazardState)
      .start();

  // Setup Lights and Effects
  DimmableLight::configureGlobalTimer();
  leftHeadlightBlinking.configure();
  rightHeadlightBlinking.configure();
  leftTaillightSequential.configure();
  rightTaillightSequential.configure();

  // Start main loop
  while (1) {
    vTaskDelay(1);
    unsigned int now = esp_timer_get_time() / 1000;
    loop(now);
  }

  // leftHeadlight.configure();
  // rightHeadlight.configure();
  // leftTail1.configure();
  // leftTail2.configure();
  // leftTail3.configure();
  // rightTail1.configure();
  // rightTail2.configure();
  // rightTail3.configure();
  // // Initialize MQTT
  // client.configure("LegoMustang")
  //     .onTopic("/lego/mustang/left_headlight/set", &setLeft)
  //     .onTopic("/lego/mustang/right_headlight/set", &setRight)
  //     .onTopic("/lego/mustang/left_taillight_1/set", &setLeftTail1)
  //     .onTopic("/lego/mustang/left_taillight_2/set", &setLeftTail2)
  //     .onTopic("/lego/mustang/left_taillight_3/set", &setLeftTail3)
  //     .onTopic("/lego/mustang/right_taillight_1/set", &setRightTail1)
  //     .onTopic("/lego/mustang/right_taillight_2/set", &setRightTail2)
  //     .onTopic("/lego/mustang/right_taillight_3/set", &setRightTail3)
  //     .start();
}