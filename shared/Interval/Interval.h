#ifndef INTERVAL_H
#define INTERVAL_H

#define DEFAULT_CHECK_INTERVAL 1000

class Interval {
private:
  int checkInterval = DEFAULT_CHECK_INTERVAL;
  unsigned int lastChecked = 0;
  bool firstCheck = true;

public:
  Interval(){};
  // Initialize interval with a custom check interval in milliseconds
  Interval(int checkInterval) : checkInterval{checkInterval} {};

  // Reset the interval to synchronize with a new timestamp
  void reset() { firstCheck = true; }

  // Check if the interval time has passed. Must pass timestamp in milliseconds
  bool check(unsigned int now) {
    if (firstCheck || now - lastChecked >= checkInterval) {
      firstCheck = false;
      lastChecked = now;
      return true;
    }
    return false;
  };
};

#endif