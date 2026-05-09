#ifndef CONTEST_HAL_MOTOR_H
#define CONTEST_HAL_MOTOR_H

#include <stdbool.h>
#include <stdint.h>

#include "contest_stm32_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim_pwm;
    uint32_t pwm_channel;
    GPIO_TypeDef *dir_port_a;
    uint16_t dir_pin_a;
    GPIO_TypeDef *dir_port_b;
    uint16_t dir_pin_b;
    uint16_t pwm_max;
    bool inverted_direction;
} contest_hal_motor_t;

bool contest_hal_motor_init(
    contest_hal_motor_t *motor,
    TIM_HandleTypeDef *htim_pwm,
    uint32_t pwm_channel,
    GPIO_TypeDef *dir_port_a,
    uint16_t dir_pin_a,
    GPIO_TypeDef *dir_port_b,
    uint16_t dir_pin_b,
    uint16_t pwm_max,
    bool inverted_direction
);
bool contest_hal_motor_start(contest_hal_motor_t *motor);
void contest_hal_motor_set_signed_output(contest_hal_motor_t *motor, int16_t signed_pwm);

#endif
