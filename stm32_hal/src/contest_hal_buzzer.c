#include "../include/contest_hal_buzzer.h"

bool contest_hal_buzzer_init(
    contest_hal_buzzer_t *buzzer,
    GPIO_TypeDef *port,
    uint16_t pin,
    bool active_level
)
{
    if (buzzer == 0 || port == 0) {
        return false;
    }

    buzzer->port = port;
    buzzer->pin = pin;
    buzzer->active_level = active_level;
    contest_buzzer_pattern_init(&buzzer->pattern);
    contest_hal_buzzer_set(buzzer, false);
    return true;
}

void contest_hal_buzzer_set(contest_hal_buzzer_t *buzzer, bool on)
{
    GPIO_PinState state;

    if (buzzer == 0 || buzzer->port == 0) {
        return;
    }

    state = on == buzzer->active_level ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(buzzer->port, buzzer->pin, state);
}

bool contest_hal_buzzer_start_pattern(
    contest_hal_buzzer_t *buzzer,
    uint16_t on_ticks,
    uint16_t off_ticks,
    uint16_t repeat_target
)
{
    if (buzzer == 0) {
        return false;
    }

    if (!contest_buzzer_pattern_start(&buzzer->pattern, on_ticks, off_ticks, repeat_target)) {
        return false;
    }

    contest_hal_buzzer_set(buzzer, true);
    return true;
}

void contest_hal_buzzer_stop(contest_hal_buzzer_t *buzzer)
{
    if (buzzer == 0) {
        return;
    }

    contest_buzzer_pattern_stop(&buzzer->pattern);
    contest_hal_buzzer_set(buzzer, false);
}

void contest_hal_buzzer_tick(contest_hal_buzzer_t *buzzer)
{
    bool on;

    if (buzzer == 0) {
        return;
    }

    on = contest_buzzer_pattern_tick(&buzzer->pattern);
    contest_hal_buzzer_set(buzzer, on);
}
