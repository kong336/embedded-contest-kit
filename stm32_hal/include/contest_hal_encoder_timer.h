#ifndef CONTEST_HAL_ENCODER_TIMER_H
#define CONTEST_HAL_ENCODER_TIMER_H

#include <stdbool.h>
#include <stdint.h>

#include "../../generic_c/include/contest_encoder.h"
#include "contest_stm32_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t counter_period;
    uint32_t last_counter;
    contest_encoder_t encoder;
} contest_hal_encoder_timer_t;

bool contest_hal_encoder_timer_init(
    contest_hal_encoder_timer_t *context,
    TIM_HandleTypeDef *htim,
    float counts_per_rev,
    float sample_dt_s,
    float wheel_diameter_mm
);
bool contest_hal_encoder_timer_start(contest_hal_encoder_timer_t *context);
void contest_hal_encoder_timer_reset(contest_hal_encoder_timer_t *context);
int32_t contest_hal_encoder_timer_sample_delta(contest_hal_encoder_timer_t *context);
float contest_hal_encoder_timer_delta_to_rpm(const contest_hal_encoder_timer_t *context, int32_t delta_count);
float contest_hal_encoder_timer_delta_to_speed_mmps(const contest_hal_encoder_timer_t *context, int32_t delta_count);

#endif
