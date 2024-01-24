#ifndef INTERVAL_H
#define INTERVAL_H

#define DEFAULT_CHECK_INTERVAL 1000

class Interval {
public:
  /**
   * Initialize with the default check interval
   */
  Interval(void){};

  /**
   * Initialize with a custom check interval
   */
  Interval(int checkInterval) : _checkInterval{checkInterval} {};

  /**
   * Reset the internal state and update the check interval if necessary
   * @param checkInterval New check interval (passing nothing keeps the previous
   * interval in place)
   */
  void reset(int checkInterval = -1) {
    if (checkInterval >= 0) {
      _checkInterval = checkInterval;
    }
    _firstCheck = true;
  }

  /**
   * Check if the interval time has passed. Must pass current timestamp in
   * milliseconds
   * @param now Current timestamp in milliseconds
   */
  bool check(unsigned int now) {
    if (_firstCheck || now - _lastChecked >= _checkInterval) {
      _firstCheck = false;
      _lastChecked = now;
      return true;
    }
    return false;
  };

private:
  int _checkInterval = DEFAULT_CHECK_INTERVAL;
  unsigned int _lastChecked = 0;
  bool _firstCheck = true;
};

#endif