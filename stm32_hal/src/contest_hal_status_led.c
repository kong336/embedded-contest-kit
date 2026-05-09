#include "../include/contest_hal_status_led.h"

bool contest_hal_status_led_init(
    contest_hal_status_led_t *led,
    GPIO_TypeDef *port,
    uint16_t pin,
    bool active_level
)
{
    if (led == 0 || port == 0) {
        return false;
    }

    led->port = port;
    led->pin = pin;
    led->active_level = active_level;
    contest_led_pattern_init(&led->pattern);
    contest_hal_status_led_set(led, false);
    return true;
}

void contest_hal_status_led_set(contest_hal_status_led_t *led, bool on)
{
    GPIO_PinState state;

    if (led == 0 || led->port == 0) {
        return;
    }

    state = on == led->active_level ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(led->port, led->pin, state);
}

bool contest_hal_status_led_start_pattern(
    contest_hal_status_led_t *led,
    uint16_t on_ticks,
    uint16_t off_ticks,
    uint16_t repeat_target
)
{
    if (led == 0) {
        return false;
    }

    if (!contest_led_pattern_start(&led->pattern, on_ticks, off_ticks, repeat_target)) {
        return false;
    }

    contest_hal_status_led_set(led, true);
    return true;
}

void contest_hal_status_led_stop(contest_hal_status_led_t *led)
{
    if (led == 0) {
        return;
    }

    contest_led_pattern_stop(&led->pattern);
    contest_hal_status_led_set(led, false);
}

void contest_hal_status_led_tick(contest_hal_status_led_t *led)
{
    bool on;

    if (led == 0) {
        return;
    }

    on = contest_led_pattern_tick(&led->pattern);
    contest_hal_status_led_set(led, on);
}
