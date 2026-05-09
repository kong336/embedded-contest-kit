#ifndef CONTEST_HAL_STATUS_LED_H
#define CONTEST_HAL_STATUS_LED_H

#include <stdbool.h>
#include <stdint.h>

#include "../../generic_c/include/contest_led_pattern.h"
#include "contest_stm32_hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    bool active_level;
    contest_led_pattern_t pattern;
} contest_hal_status_led_t;

bool contest_hal_status_led_init(
    contest_hal_status_led_t *led,
    GPIO_TypeDef *port,
    uint16_t pin,
    bool active_level
);
void contest_hal_status_led_set(contest_hal_status_led_t *led, bool on);
bool contest_hal_status_led_start_pattern(
    contest_hal_status_led_t *led,
    uint16_t on_ticks,
    uint16_t off_ticks,
    uint16_t repeat_target
);
void contest_hal_status_led_stop(contest_hal_status_led_t *led);
void contest_hal_status_led_tick(contest_hal_status_led_t *led);

#endif
