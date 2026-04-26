#include "menu_display.h"

#include <stdio.h>
#include "esp_log.h"
#include "mz.h"
#include "ssd1306.h"

static const char *TAG = "MENU_DISPLAY";
static bool menu_display_ready = false;

esp_err_t menu_display_refresh(void)
{
    if (!menu_display_ready)
    {
        return ESP_ERR_INVALID_STATE;
    }

    char gain_line[32];
    char timing_line[32];
    char model_line[32];

    float sample_time = get_ts();
    float sample_freq = (sample_time > 0.0f) ? (1.0f / sample_time) : 0.0f;

    snprintf(gain_line, sizeof(gain_line), "a=%.1f", get_a());
    snprintf(timing_line, sizeof(timing_line), "%.0fHz|%.3fs", sample_freq, sample_time);
    snprintf(model_line, sizeof(model_line), "Modelo: %d", get_model_item());

    // Redraw the full menu every time to keep the OLED state consistent.
    esp_err_t ret = ssd1306_clear();
    if (ret != ESP_OK)
    {
        return ret;
    }

    ssd1306_print_str(0, 7, "IFCE: Luan", false);
    ssd1306_print_str(0, 17, gain_line, false);
    ssd1306_print_str(0, 27, timing_line, false);
    ssd1306_print_str(0, 37, model_line, false);

    return ssd1306_display();
}

esp_err_t menu_display_init(void)
{
    esp_err_t ret = init_ssd1306();
    if (ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Nao foi possivel inicializar o display");
        return ret;
    }

    menu_display_ready = true;
    return menu_display_refresh();
}
