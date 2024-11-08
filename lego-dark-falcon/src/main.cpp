#include <Light.h>

extern "C" {
    void app_main(void);
}

Light booster(16);

void app_main() {
    booster.on();
}