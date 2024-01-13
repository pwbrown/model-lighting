#ifndef INTERVAL_H
#define INTERVAL_H

#include <Arduino.h>

#define DEFAULT_CHECK_INTERVAL 1000

class Interval {
private:
  int check_interval = DEFAULT_CHECK_INTERVAL;
  unsigned int last_checked = 0;

public:
  Interval(){};
  // Initialize interval with a custom check interval in milliseconds
  Interval(int check_interval) : check_interval{check_interval} {};

  // Check if the interval time has passed. Must pass timestamp in milliseconds
  bool check(unsigned int now) {
    if (now - last_checked >= check_interval) {
      last_checked = now;
      Serial.println(last_checked);
      return true;
    }
    return false;
  };
};

#endif