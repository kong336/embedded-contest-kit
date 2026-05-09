#include "../include/contest_hal_status_led.h"

static contest_hal_status_led_t g_status_led;

void app_stm32_status_led_init(void)
{
    contest_hal_status_led_init(&g_status_led, GPIOC, GPIO_PIN_13, false);
}

void app_stm32_status_led_show_ready(void)
{
    /* One long blink is an easy "ready" indication after power-on. */
    (void)contest_hal_status_led_start_pattern(&g_status_led, 100u, 0u, 1u);
}

void app_stm32_status_led_show_warning(void)
{
    /* Three short blinks work well for warning or parameter-save feedback. */
    (void)contest_hal_status_led_start_pattern(&g_status_led, 20u, 20u, 3u);
}

void app_stm32_status_led_tick_1ms(void)
{
    contest_hal_status_led_tick(&g_status_led);
}
