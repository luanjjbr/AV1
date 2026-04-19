#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "driver/ledc.h"

void pwm_init(int gpio, int freq_hz);
void pwm_set_duty(uint32_t duty);
uint32_t normaliza_pwm(float x);

#endif