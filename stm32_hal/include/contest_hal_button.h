#ifndef CONTEST_HAL_BUTTON_H
#define CONTEST_HAL_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

#include "../../generic_c/include/contest_button.h"
#include "contest_stm32_hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    contest_button_t button;
} contest_hal_button_t;

bool contest_hal_button_init(
    contest_hal_button_t *context,
    GPIO_TypeDef *port,
    uint16_t pin,
    bool active_level,
    uint16_t debounce_ticks,
    uint16_t long_press_ticks
);
contest_button_event_t contest_hal_button_poll(contest_hal_button_t *context);
bool contest_hal_button_is_pressed(const contest_hal_button_t *context);

#endif
