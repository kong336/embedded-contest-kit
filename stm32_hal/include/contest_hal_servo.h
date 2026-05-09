#ifndef CONTEST_HAL_SERVO_H
#define CONTEST_HAL_SERVO_H

#include <stdbool.h>
#include <stdint.h>

#include "../../generic_c/include/contest_servo_map.h"
#include "contest_stm32_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim_pwm;
    uint32_t pwm_channel;
    contest_servo_map_t map;
} contest_hal_servo_t;

bool contest_hal_servo_init(
    contest_hal_servo_t *servo,
    TIM_HandleTypeDef *htim_pwm,
    uint32_t pwm_channel,
    float angle_min_deg,
    float angle_max_deg,
    uint16_t compare_min,
    uint16_t compare_max
);
bool contest_hal_servo_start(contest_hal_servo_t *servo);
void contest_hal_servo_set_compare(contest_hal_servo_t *servo, uint16_t compare_value);
void contest_hal_servo_set_angle(contest_hal_servo_t *servo, float angle_deg);

#endif
