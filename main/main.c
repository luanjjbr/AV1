#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

// ============================
// Components
// ============================
#include "Button.h"
#include "led.h"
#include "menu_display.h"
#include "mz.h"
#include "pwm_driver.h"
#include "serial_monitor.h"

static const char *TAG = "APP_MAIN";

static int64_t get_period_us(void)
{
    return (int64_t)(get_ts() * 1e6f + 0.5f);
}

static void mz_task(void *pvParameters)
{
    (void)pvParameters;
    int64_t t_ant = esp_timer_get_time();

    while (1)
    {
        int64_t current_period = get_period_us();
        int64_t time_now = esp_timer_get_time();

        if ((time_now - t_ant) >= current_period)
        {
            // Advance from the previous deadline to avoid cumulative drift.
            t_ant += current_period;

            led_on();
            func_mz();
            get_printf();
            led_off();
        }

        vTaskDelay(1);
    }
}

void app_main(void)
{
    led_init();
    button_init();
    pwm_init(18, 5000);

    // Start with a valid model so the initial screen matches the configured UI.
    config_item(4);

    if (menu_display_init() != ESP_OK)
    {
        ESP_LOGW(TAG, "Display SSD1306 indisponivel, seguindo sem interface OLED");
    }

    xTaskCreate(mz_task, "mz", 4096, NULL, 10, NULL);
    serial_monitor_start();
}
