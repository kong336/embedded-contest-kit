#include "../include/contest_hal_button.h"

bool contest_hal_button_init(
    contest_hal_button_t *context,
    GPIO_TypeDef *port,
    uint16_t pin,
    bool active_level,
    uint16_t debounce_ticks,
    uint16_t long_press_ticks
)
{
    if (context == 0 || port == 0) {
        return false;
    }

    context->port = port;
    context->pin = pin;
    contest_button_init(&context->button, active_level, debounce_ticks, long_press_ticks);
    return true;
}

contest_button_event_t contest_hal_button_poll(contest_hal_button_t *context)
{
    bool raw_level;

    if (context == 0 || context->port == 0) {
        return CONTEST_BUTTON_EVENT_NONE;
    }

    /* Convert HAL pin state into a simple bool, then reuse the generic button logic. */
    raw_level = HAL_GPIO_ReadPin(context->port, context->pin) == GPIO_PIN_SET;
    return contest_button_update(&context->button, raw_level);
}

bool contest_hal_button_is_pressed(const contest_hal_button_t *context)
{
    if (context == 0) {
        return false;
    }

    return contest_button_is_pressed(&context->button);
}
