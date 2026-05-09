#include "../include/contest_hal_buzzer.h"

static contest_hal_buzzer_t g_buzzer;

void app_stm32_buzzer_init(void)
{
    contest_hal_buzzer_init(&g_buzzer, GPIOB, GPIO_PIN_8, true);
}

void app_stm32_buzzer_play_success(void)
{
    /* Two short beeps are a simple match-start or command-accepted indicator. */
    (void)contest_hal_buzzer_start_pattern(&g_buzzer, 30u, 20u, 2u);
}

void app_stm32_buzzer_tick_1ms(void)
{
    contest_hal_buzzer_tick(&g_buzzer);
}
