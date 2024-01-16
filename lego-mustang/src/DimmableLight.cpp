#include "DimmableLight.h"
#include <driver/ledc.h>
#include <math.h>

#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL (ledc_channel_t) channel
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LEDC_FREQUENCY (4000)
#define LEDC_TIMER LEDC_TIMER_0

DimmableLight::DimmableLight(int pin, int channel)
    : pin{pin}, channel{channel} {};

void DimmableLight::configureGlobalTimer() {
  ledc_timer_config_t timerConfig = {.speed_mode = LEDC_MODE,
                                     .duty_resolution = LEDC_DUTY_RES,
                                     .timer_num = LEDC_TIMER,
                                     .freq_hz = LEDC_FREQUENCY,
                                     .clk_cfg = LEDC_AUTO_CLK};
  ledc_timer_config(&timerConfig);
}

void DimmableLight::configure() {
  if (!isSetup) {
    ledc_channel_config_t channelConfig = {.gpio_num = pin,
                                           .speed_mode = LEDC_MODE,
                                           .channel = LEDC_CHANNEL,
                                           .intr_type = LEDC_INTR_DISABLE,
                                           .timer_sel = LEDC_TIMER_0,
                                           .duty = 0,
                                           .hpoint = 0};
    ledc_channel_config(&channelConfig);
    isSetup = true;
  }
};

void DimmableLight::on(int brightness) const {
  double percentage = brightness == 0 ? 0.0 : brightness / 100.0;
  double duty = 8192 * percentage;
  ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
  ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
};

void DimmableLight::off() const {
  ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
  ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
};