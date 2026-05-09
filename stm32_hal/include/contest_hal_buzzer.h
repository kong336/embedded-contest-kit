#ifndef CONTEST_HAL_BUZZER_H
#define CONTEST_HAL_BUZZER_H

#include <stdbool.h>
#include <stdint.h>

#include "../../generic_c/include/contest_buzzer_pattern.h"
#include "contest_stm32_hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    bool active_level;
    contest_buzzer_pattern_t pattern;
} contest_hal_buzzer_t;

bool contest_hal_buzzer_init(
    contest_hal_buzzer_t *buzzer,
    GPIO_TypeDef *port,
    uint16_t pin,
    bool active_level
);
void contest_hal_buzzer_set(contest_hal_buzzer_t *buzzer, bool on);
bool contest_hal_buzzer_start_pattern(
    contest_hal_buzzer_t *buzzer,
    uint16_t on_ticks,
    uint16_t off_ticks,
    uint16_t repeat_target
);
void contest_hal_buzzer_stop(contest_hal_buzzer_t *buzzer);
void contest_hal_buzzer_tick(contest_hal_buzzer_t *buzzer);

#endif
