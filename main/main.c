#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <string.h>
#include <ctype.h>

// ============================
// Compinents
// ============================
#include "led.h"
#include "Button.h"
#include "mz.h"
#include "pwm_driver.h"
#include "ssd1306.h"

bool print_flag = true;

int64_t t_ant = 0;
int64_t periodo = 1000000; // 1 segundo em microsegundos

void serial_monitor_task(void *pvParameters)
{
    char str1[32];
    char str2[32];

    while (1)
    {
        char buffer[32];

        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            // remove \n
            buffer[strcspn(buffer, "\r\n")] = 0;

            char cmd = tolower((unsigned char)buffer[0]);

            switch (cmd)
            {
            case 'i':
            case 'd':
            case 'r':
                ESP_LOGI("SERIAL", ">>> %s <<<", buffer);
                break;

            case 'p':
                ESP_LOGI("SERIAL", ">>> %s <<<", buffer);
                print_flag = !print_flag;
                break;

            default:
            {
                float v1, v2;

                if (sscanf(buffer, "%f,%f", &v1, &v2) == 2)
                {
                    if (v1 == 0.0f || v2 == 0.0f)
                    {
                        ESP_LOGW("SERIAL", "Divisao por zero!");
                        break;
                    }

                    periodo = 1000000;

                    ESP_LOGI("SERIAL", "Valor 1: %.2f", v1);
                    ESP_LOGI("SERIAL", "Valor 2: %.2f", v2);

                    snprintf(str1, sizeof(str1), "%.1fHz|%.1fs", v1, 1.0f / v1);
                    snprintf(str2, sizeof(str2), "%.0fHz|%.3fms", v2, 1.0f / v2);

                    ssd1306_clear();
                    ssd1306_print_str(0, 17, "IFCE: Luan", false);
                    ssd1306_print_str(0, 27, str1, false);
                    ssd1306_print_str(0, 37, str2, false);
                    ssd1306_display();
                }
                else
                {
                    ESP_LOGI("SERIAL", "Formato invalido: %s", buffer);
                }
                break;
            }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void mz(void *pvParameters)
{
    periodo = (int64_t)(1000000.0); // segundos → µs

    while (1)
    {
        int64_t Time = esp_timer_get_time();

        if ((Time - t_ant) >= periodo)
        {
            led_on();
            vTaskDelay(1);
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

    // ================================
    // Display
    // ================================
    init_ssd1306();
    ssd1306_print_str(18, 17, "Hello World!", false);
    ssd1306_display();

    vTaskDelay(pdMS_TO_TICKS(5000));

    ssd1306_clear();
    ssd1306_print_str(0, 17, "IFCE: Luan", false);
    ssd1306_print_str(0, 27, "10Hz|0.1s", false);
    ssd1306_print_str(0, 37, "10Hz|0.1s", false);
    ssd1306_display();

    // ================================
    // freertos
    // ================================
    xTaskCreate(mz, "mz", 4096, NULL, 10, NULL);

    xTaskCreate(serial_monitor_task, "serial_task", 2048, NULL, 5, NULL);
}
