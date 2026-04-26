idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES Button
                             led
                             menu_display
                             mz
                             pwm_driver
                             serial_monitor
                             esp_timer)
