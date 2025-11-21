#include <Light.h>

Light testingLight(15);

// Export main function for C compiler
extern "C" {
void app_main(void);
}

void app_main(void) {
    testingLight.on();
}