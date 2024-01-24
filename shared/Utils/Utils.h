#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <functional>

namespace Utils {
/**
 * Start the main loop
 * @param callback Function that is called by the loop that receives the current
 * timestamp in milliseconds
 * @param tick A delay tick value used for delaying each iteration of the loop
 * to allow background tasks to complete
 */
void startLoop(void (*callback)(unsigned int), TickType_t tick = 1) {
  while (1) {
    vTaskDelay(tick);
    unsigned int now = esp_timer_get_time() / 1000;
    callback(now);
  }
}
} // namespace Utils