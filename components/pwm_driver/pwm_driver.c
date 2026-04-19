#include "pwm_driver.h"
#include "esp_err.h"

#define PWM_MODE LEDC_HIGH_SPEED_MODE
#define PWM_TIMER LEDC_TIMER_0
#define PWM_CHANNEL LEDC_CHANNEL_0
#define PWM_RESOLUTION LEDC_TIMER_8_BIT

static int max_duty = 255;

void pwm_init(int gpio, int freq_hz)
{
    ledc_timer_config_t timer = {.speed_mode = PWM_MODE,
                                 .timer_num = PWM_TIMER,
                                 .duty_resolution = PWM_RESOLUTION,
                                 .freq_hz = freq_hz,
                                 .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&timer));

    ledc_channel_config_t channel = {.gpio_num = gpio,
                                     .speed_mode = PWM_MODE,
                                     .channel = PWM_CHANNEL,
                                     .intr_type = LEDC_INTR_DISABLE,
                                     .timer_sel = PWM_TIMER,
                                     .duty = 0,
                                     .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&channel));

    max_duty = (1 << PWM_RESOLUTION) - 1;
}

void pwm_set_duty(uint32_t duty)
{
    if (duty > max_duty)
        duty = max_duty;

    ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
    ledc_update_duty(PWM_MODE, PWM_CHANNEL);
}
uint32_t normaliza_pwm(float x)
{
    if (x > 30.0f)
        x = 30.0f;
    if (x < -30.0f)
        x = -30.0f;

    return (uint32_t)(((x + 30.0f) / 60.0f) * 255.0f);
}