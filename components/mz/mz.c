#include "mz.h"

#include <stdio.h>
#include "button.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include <stdbool.h> // Necessário para usar 'true' se desejar
#include <stdio.h>

static const char *TAG = "Mz_COMPONENT";

float u[3] = {0};
float y[3] = {0};
float w0 = 2.0f * M_PI * 60.0f;
float T = 0.1;

float num[4];
float den[4];

float condition = 1.0f;

void config_item(int value_mz)
{
    switch (value_mz)
    {
    case 1:
        // ============================
        // item 4 num = [1];
        // den = [1 0 0 0];
        // num_inv = [T ^ 2 / 2  T ^ 2 / 2  0];
        // den_inv = [1 - 3 3 - 1];

        // ============================

        ESP_LOGI(TAG, "item %s: %i", "4", value_mz);

        // ============================
        // numerador
        // ============================
        num[0] = (T * T) / 2;
        num[1] = (T * T) / 2;
        num[2] = 0;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 1;
        den[1] = -3;
        den[2] = 3;
        den[3] = -1;

        break;
    case 2:
        // ============================
        // item 8 num = [a];
        // den = [1 a 0];
        // num_inv = [(1 - exp(-a * T))0];
        // den_inv = [1 - (1 + exp(-a * T)) exp(-a * T)];
        // ============================
        ESP_LOGI(TAG, "item %s: %i", "8", value_mz);
        break;
    case 3:
        // ============================
        // item 12 num = [1 0];
        // den = [1 0 a ^ 2];
        // num_inv = [1 - cos(a * T) 0];
        // den_inv = [1 - 2 * cos(a * T) 1];
        // ============================
        ESP_LOGI(TAG, "item %s: %i", "12", value_mz);
        break;
    case 4:
        // ============================
        // item 11 num = [a];
        // den = [1 0 a ^ 2];
        // num_inv = [0 sin(a * T) 0];
        // den_inv = [1 - 2 * cos(a * T) 1];
        // ============================
        ESP_LOGI(TAG, "item %s: %i", "11", value_mz);
        break;
    default:
        ESP_LOGI(TAG, "", "", value_mz);
    }
}
