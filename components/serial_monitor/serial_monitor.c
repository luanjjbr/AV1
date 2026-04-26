#include "serial_monitor.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "menu_display.h"
#include "mz.h"

static const char *TAG = "SERIAL_MONITOR";
static bool print_flag = true;

static void refresh_menu(void)
{
    esp_err_t ret = menu_display_refresh();
    if (ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Falha ao atualizar o menu do display");
    }
}

static void serial_monitor_task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        char buffer[32];

        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            buffer[strcspn(buffer, "\r\n")] = 0;

            // Keep the original key so 'r' and 'R' can map to different actions.
            char cmd = buffer[0];

            switch (cmd)
            {
            case 'i':
            case 'I':
                ESP_LOGI(TAG, "SERIAL>>> %s <<<", buffer);
                im_sinc();
                break;

            case 'd':
            case 'D':
                ESP_LOGI(TAG, "SERIAL>>> %s <<<", buffer);
                deg_sinc();
                break;

            case 'r':
                ESP_LOGI(TAG, "SERIAL>>> %s <<<", buffer);
                ram_sinc();
                break;

            case 'R':
            case 'g':
            case 'G':
                ESP_LOGI(TAG, "SERIAL>>> %s <<<", buffer);
                reset_sinc();
                break;

            case 'p':
            case 'P':
                ESP_LOGI(TAG, "SERIAL>>> %s <<<", buffer);
                print_flag = !print_flag;
                ESP_LOGI(TAG, "Print flag: %s", print_flag ? "ON" : "OFF");
                break;

            default:
            {
                float v1, v2;
                int val;
                char extra;

                // Expected format: a,T in seconds.
                if (sscanf(buffer, "%f,%f", &v1, &v2) == 2)
                {
                    if (v1 == 0.0f || v2 == 0.0f)
                    {
                        ESP_LOGW(TAG, "Divisao por zero!");
                        break;
                    }

                    ESP_LOGI(TAG, "Valor 1: %.2f", v1);
                    ESP_LOGI(TAG, "Valor 2: %.2f", v2);

                    config_hz_ts(v1, v2);
                    refresh_menu();
                }
                else if (sscanf(buffer, "%d %c", &val, &extra) == 1)
                {
                    if ((val >= 1 && val <= 14) || (val >= 101 && val <= 114))
                    {
                        config_item(val);
                        refresh_menu();
                    }
                    else
                    {
                        ESP_LOGI(TAG, "Item fora da faixa: %d", val);
                    }
                }
                else
                {
                    ESP_LOGI(TAG, "Formato invalido: %s", buffer);
                }
                break;
            }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void serial_monitor_start(void)
{
    xTaskCreate(serial_monitor_task, "serial_task", 2048, NULL, 5, NULL);
}
