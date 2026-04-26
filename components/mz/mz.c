#include "mz.h"

#include <stdio.h>
#include "Button.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include <stdbool.h> // Necessário para usar 'true' se desejar
#include <stdio.h>

static const char *TAG = "Mz_COMPONENT";

float u[4] = {0};
float y[4] = {0};
float w0 = 2.0f * M_PI * 60.0f;
float T = 0.1f;
float a = 1.5f;

float num[4];
float den[4];

float condition = 1.0f;
float condition_2 = 0.0f;

int value_mz_item = 0;
bool rampa = false;
bool imp = false;

void config_item(int value_mz)
{
    value_mz_item = (int)value_mz;
    switch (value_mz)
    {
    case 4:
        reset_sinc();

        ESP_LOGI(TAG, "item %d: m(nT)= 0.5 * (n * T)^2", 4);

        // ============================
        // numerador
        // ============================
        num[0] = 0;
        num[1] = (T * T) / 2;
        num[2] = (T * T) / 2;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 1;
        den[1] = -3;
        den[2] = 3;
        den[3] = -1;

        ESP_LOGI(TAG,
                 "item 104:\n"
                 "num = [%.6f %.6f %.6f %.6f]\n"
                 "den = [%.6f %.6f %.6f %.6f]\n",
                 num[0], num[1], num[2], num[3],
                 den[0], den[1], den[2], den[3]);

        break;
    case 104:
        reset_sinc();
        // ============================
        // item 4
        // num_inv = [T^2/2 T^2/2 0];
        // den_inv = [1 -3 3 -1];

        // ============================

        ESP_LOGI(TAG, "item %s: %i : m(nT)= 0,5 * (n * T)^2 ", "104", value_mz);

        // ============================
        // numerador
        // ============================
        num[0] = 0;
        num[1] = (T * T * T) / 6;
        num[2] = (4 * (T * T * T)) / 6;
        num[3] = (T * T * T) / 6;

        // ============================
        // denominador
        // ============================
        den[0] = -1;
        den[1] = 3;
        den[2] = -3;
        den[3] = 1;

        break;

    case 8:
        reset_sinc();
        // ============================
        // item 8
        // num_inv = [ (1 - exp(-a*T))  0 ];
        // den_inv = [ 1  -(1 + exp(-a*T))  exp(-a*T) ];
        // ============================
        ESP_LOGI(TAG, "item %s: %i 1 : m(nT)= 1 - e^(-a * n * T) ", "8", value_mz);
        // ============================
        // numerador
        // ============================
        num[0] = 0;
        num[1] = (1 - exp(-a * T));
        num[2] = 0;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 1;
        den[1] = -(1 + exp(-a * T));
        den[2] = exp(-a * T);
        den[3] = 0;

        break;
    case 108:
        reset_sinc();
        // ============================
        // item 8
        // num_inv = [ (1 - exp(-a*T))  0 ];
        // den_inv = [ 1  -(1 + exp(-a*T))  exp(-a*T) ];
        // ============================
        ESP_LOGI(TAG, "item %s: %i 1 : m(nT)= 1 - e^(-a * n * T) ", "108", value_mz);
        // ============================
        // numerador
        // ============================
        num[0] = 0;
        num[1] = (a * T - 1 + exp(-a * T)) / a;
        num[2] = (1 - exp(-a * T) - a * T * exp(-a * T)) / a;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 1;
        den[1] = -(1 + exp(-a * T));
        den[2] = exp(-a * T);
        den[3] = 0;

        break;
    case 12:
        reset_sinc();
        // ============================
        // item 12
        // num_inv = [0  sin(a*T)  0];
        // den_inv = [1  -2*cos(a*T)  1];
        // ============================
        ESP_LOGI(TAG, "item %s: %i : cos (a * n * T)", "12", value_mz);
        // ============================
        // numerador
        // ============================
        num[0] = 1;
        num[1] = -cos(a * T);
        num[2] = 0;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 1;
        den[1] = -(2 * cos(a * T));
        den[2] = 1;
        den[3] = 0;
        ESP_LOGI(TAG,
                 "item 12:\n"
                 "num = [%.3f %.3f %.3f %.3f]\n"
                 "den = [%.3f %.3f %.3f %.3f]\n",
                 num[0], num[1], num[2], num[3],
                 den[0], den[1], den[2], den[3]);
        break;
    case 112:
        reset_sinc();
        // ============================
        // item 12
        // num_inv = [0  sin(a*T)  0];
        // den_inv = [1  -2*cos(a*T)  1];
        // ============================
        ESP_LOGI(TAG, "item %s: %i : cos (a * n * T)", "112", value_mz);
        // ============================
        // numerador
        // ============================
        num[0] = 0;
        num[1] = sin(a * T) / a;
        num[2] = -sin(a * T) / a;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 0;
        den[1] = -(2 * cos(a * T));
        den[2] = 1;
        den[3] = 0;
        ESP_LOGI(TAG,
                 "item 112:\n"
                 "num = [%.3f %.3f %.3f %.3f]\n"
                 "den = [%.3f %.3f %.3f %.3f]\n",
                 num[0], num[1], num[2], num[3],
                 den[0], den[1], den[2], den[3]);
        break;
    case 11:
        reset_sinc();
        // ============================
        // item 11
        // num_inv = [0 sin(a * T) 0];
        // den_inv = [1 - 2 * cos(a * T) 1];
        // ============================
        ESP_LOGI(TAG, "item %s: %i : cos (a * n * T)\n\r", "11", value_mz);
        // ============================
        // numerador
        // ============================
        num[0] = 0;
        num[1] = sin(a * T);
        num[2] = 0;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 1;
        den[1] = -(2 * cos(a * T));
        den[2] = 1;
        den[3] = 0;
        ESP_LOGI(TAG,
                 "item 11:\n"
                 "num = [%.3f %.3f %.3f %.3f]\n"
                 "den = [%.3f %.3f %.3f %.3f]\n",
                 num[0], num[1], num[2], num[3],
                 den[0], den[1], den[2], den[3]);
        break;
    case 111:
        reset_sinc();
        // ============================
        // item 11
        // num_inv = [0 sin(a * T) 0];
        // den_inv = [1 - 2 * cos(a * T) 1];
        // ============================
        ESP_LOGI(TAG, "ZOH: item %s: %i : cos (a * n * T)\n\r", "111", value_mz);
        // ============================
        // numerador
        // ============================
        num[0] = 0;
        num[1] = (1 - cos(a * T)) / a;
        num[2] = (1 - cos(a * T)) / a;
        num[3] = 0;

        // ============================
        // denominador
        // ============================
        den[0] = 0;
        den[1] = -(2 * cos(a * T));
        den[2] = 1;
        den[3] = 0;
        ESP_LOGI(TAG,
                 "item 111:\n"
                 "num = [%.3f %.3f %.3f %.3f]\n"
                 "den = [%.3f %.3f %.3f %.3f]\n",
                 num[0], num[1], num[2], num[3],
                 den[0], den[1], den[2], den[3]);
        break;
    default:
        // ============================
        // default numerador
        // ============================
        num[0] = 0;
        num[1] = 0;
        num[2] = 0;
        num[3] = 0;

        // ============================
        // default denominador
        // ============================
        den[0] = 0;
        den[1] = 0;
        den[2] = 0;
        den[3] = 0;
        ESP_LOGI(TAG, "não implementado", "", value_mz);
    }
}

void deslocar_dados(float array[])
{
    // Preserve the full history because some recurrences use index 3.
    array[3] = array[2];
    array[2] = array[1];
    array[1] = array[0];
}

void func_mz(void)
{
    deslocar_dados(u);
    deslocar_dados(y);
    if (rampa == false)
    {
        if (!button_get_state())
        {
            u[0] = condition; // Pressionado
        }
        else
        {
            u[0] = condition_2; // Não pressionado
        }
    }
    else
    {
        u[0] += T;
        if (u[0] > 10)
        {
            rampa = false;
            for (int i = 0; i < 4; i++)
            {
                u[i] = 0.0f;
                y[i] = 0.0f;
            }
        }
    }
    if (imp == true)
    {
        imp = false;
        u[0] = 1.0f;
    }

    switch (value_mz_item)
    {
    case 4:
        y[0] = num[2] * u[2] + num[1] * u[1] + num[0] * u[0] - den[1] * y[1] - den[2] * y[2] - den[3] * y[3];
        break;
    case 104:
        y[0] = num[1] * u[1] + num[2] * u[2] - den[1] * y[1] - den[2] * y[2] - den[3] * y[3];
        break;

    case 8:
        y[0] = num[0] * u[0] + num[1] * u[1] - den[1] * y[1] - den[2] * y[2];
        break;
    case 108:
        y[0] = num[1] * u[1] + num[2] * u[2] - den[1] * y[1] - den[2] * y[2];
        break;

    case 11:
        y[0] = (num[1] * u[1]) + (num[3] * u[3]) - (den[1] * y[1]) - (y[2]);
        break;
    case 111:
        y[0] = (num[1] * u[1]) + (num[2] * u[2]) - (den[1] * y[1]) - (y[2]);
        break;

    case 12:
        y[0] = (u[0]) + (num[1] * u[1]) - (den[1] * y[1]) - (y[2]);
        break;

    case 112:
        y[0] = (num[1] * u[1]) + (num[2] * u[2]) - (den[1] * y[1]) - (y[2]);
        break;

    default:
        break;
    }
}

void get_printf(void) { printf(">Entrada:%f\n>Saida_Y:%f\n", u[0], y[0]); }

float get_a(void) { return a; }

float get_ts(void) { return T; }

int get_model_item(void) { return value_mz_item; }

void reset_sinc(void)
{
    rampa = false;
    imp = false;
    condition = 1.0f; // Não pressionado
    condition_2 = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        u[i] = 0.0f;
        y[i] = 0.0f;
    }
}

void im_sinc(void)
{
    rampa = false;
    imp = true;
}

void deg_sinc(void)
{
    imp = false;
    rampa = false;
    condition = 1.0f; // Não pressionado
    condition_2 = 1.0f;
}
void ram_sinc(void)
{
    imp = false;
    rampa = true;
}

void config_hz_ts(float a_config, float T_config)
{
    // Update both runtime parameters before recomputing the coefficients.
    a = a_config;
    T = T_config;
    config_item(value_mz_item);
}
