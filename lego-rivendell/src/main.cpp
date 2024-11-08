#include <stdint.h>
#include "esp_random.h"

#include <Light.h>
#include <Interval.h>
#include <Utils.h>

// Export main function for C compiler
extern "C" {
void app_main(void);
}

uint32_t get_random_between(uint32_t min, uint32_t max) {
    return (esp_random() % ((max + 1) - min)) + min;
}

Light test(16, 0);
Interval tmpInt(get_random_between(250, 1000));


void loop(unsigned int now) {
    if (tmpInt.check(now)) {
        tmpInt.reset(get_random_between(250, 1000));
        tmpInt.check(now); // Trigger first check
        test.on(get_random_between(50, 100));
    }
}

void app_main(void) {
    Light::configurePWMTimer();

    Utils::startLoop(&loop);
}