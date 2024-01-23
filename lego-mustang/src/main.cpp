#include "MqttClient.h"
#include <Light.h>
#include <string>

extern "C" {
void app_main(void);
}

Light leftHeadlight(16);
Light rightHeadlight(13);
Light leftTail1(21);
Light leftTail2(22);
Light leftTail3(23);
Light rightTail1(26);
Light rightTail2(25);
Light rightTail3(33);

MqttClient client;

std::string leftHeadlightState = "OFF";

void setLeft(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  leftHeadlightState = state;
  if (leftHeadlightState == "ON") {
    leftHeadlight.on();
  } else {
    leftHeadlight.off();
  }
  client.publish("/lego/mustang/left_headlight/state", leftHeadlightState);
}

std::string rightHeadlightState = "OFF";

void setRight(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  rightHeadlightState = state;
  if (rightHeadlightState == "ON") {
    rightHeadlight.on();
  } else {
    rightHeadlight.off();
  }
  client.publish("/lego/mustang/right_headlight/state", rightHeadlightState);
}

std::string leftTail1State = "OFF";

void setLeftTail1(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  leftTail1State = state;
  if (leftTail1State == "ON") {
    leftTail1.on();
  } else {
    leftTail1.off();
  }
  client.publish("/lego/mustang/left_taillight_1/state", leftTail1State);
}

std::string leftTail2State = "OFF";

void setLeftTail2(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  leftTail2State = state;
  if (leftTail2State == "ON") {
    leftTail2.on();
  } else {
    leftTail2.off();
  }
  client.publish("/lego/mustang/left_taillight_2/state", leftTail2State);
}

std::string leftTail3State = "OFF";

void setLeftTail3(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  leftTail3State = state;
  if (leftTail3State == "ON") {
    leftTail3.on();
  } else {
    leftTail3.off();
  }
  client.publish("/lego/mustang/left_taillight_3/state", leftTail3State);
}

std::string rightTail1State = "OFF";

void setRightTail1(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  rightTail1State = state;
  if (rightTail1State == "ON") {
    rightTail1.on();
  } else {
    rightTail1.off();
  }
  client.publish("/lego/mustang/right_taillight_1/state", rightTail1State);
}

std::string rightTail2State = "OFF";

void setRightTail2(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  rightTail2State = state;
  if (rightTail2State == "ON") {
    rightTail2.on();
  } else {
    rightTail2.off();
  }
  client.publish("/lego/mustang/right_taillight_2/state", rightTail2State);
}

std::string rightTail3State = "OFF";

void setRightTail3(std::string state) {
  if (state != "ON" && state != "OFF") {
    return;
  }
  rightTail3State = state;
  if (rightTail3State == "ON") {
    rightTail3.on();
  } else {
    rightTail3.off();
  }
  client.publish("/lego/mustang/right_taillight_3/state", leftTail3State);
}

void app_main(void) {
  leftHeadlight.configure();
  rightHeadlight.configure();
  leftTail1.configure();
  leftTail2.configure();
  leftTail3.configure();
  rightTail1.configure();
  rightTail2.configure();
  rightTail3.configure();
  // Initialize MQTT
  client.configure("LegoMustang")
      .onTopic("/lego/mustang/left_headlight/set", &setLeft)
      .onTopic("/lego/mustang/right_headlight/set", &setRight)
      .onTopic("/lego/mustang/left_taillight_1/set", &setLeftTail1)
      .onTopic("/lego/mustang/left_taillight_2/set", &setLeftTail2)
      .onTopic("/lego/mustang/left_taillight_3/set", &setLeftTail3)
      .onTopic("/lego/mustang/right_taillight_1/set", &setRightTail1)
      .onTopic("/lego/mustang/right_taillight_2/set", &setRightTail2)
      .onTopic("/lego/mustang/right_taillight_3/set", &setRightTail3)
      .start();
}