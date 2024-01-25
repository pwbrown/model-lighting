#ifndef INTERVAL_H
#define INTERVAL_H

#define DEFAULT_CHECK_INTERVAL 1000

/**
 * Interval is an abstraction for creating an time-based interval system that
 * allows the code to check for the passage of time to run actions
 */
class Interval {
public:
  /**
   * Initialize with the default check interval
   */
  Interval(void){};

  /**
   * Initialize with a custom check interval
   * @param checkInterval The interval or period in milliseconds to wait before
   * returning true on a check call
   */
  Interval(int checkInterval) : _checkInterval{checkInterval} {};

  /**
   * Reset the internal state and update the check interval if necessary
   * @param checkInterval The interval or period in milliseconds to wait before
   * returning true on a check call (passing -1 will keep the existing interval
   * value)
   */
  void reset(int checkInterval = -1) {
    if (checkInterval >= 0) {
      _checkInterval = checkInterval;
    }
    _firstCheck = true;
  }

  /**
   * Check if the interval amount of time has passed. When the check returns
   * true, the interval resets to wait for the interval to pass again
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
  int _checkInterval =
      DEFAULT_CHECK_INTERVAL;    // The check interval/period in milliseconds
  unsigned int _lastChecked = 0; // The timestamp in milliseconds that the
                                 // interval was last successfully checked
  bool _firstCheck = true; // Indicates if it is the first check since resetting
};

#endif