#include "../include/contest_hal_motor.h"

static void contest_hal_motor_set_direction(contest_hal_motor_t *motor, bool forward)
{
    GPIO_PinState a_state;
    GPIO_PinState b_state;

    if (motor == 0) {
        return;
    }

    if (motor->inverted_direction) {
        forward = !forward;
    }

    /* Drive one pin high and the other low for a typical H-bridge direction pair. */
    a_state = forward ? GPIO_PIN_SET : GPIO_PIN_RESET;
    b_state = forward ? GPIO_PIN_RESET : GPIO_PIN_SET;

    HAL_GPIO_WritePin(motor->dir_port_a, motor->dir_pin_a, a_state);
    HAL_GPIO_WritePin(motor->dir_port_b, motor->dir_pin_b, b_state);
}

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
)
{
    if (motor == 0 || htim_pwm == 0 || dir_port_a == 0 || dir_port_b == 0 || pwm_max == 0u) {
        return false;
    }

    /* CubeMX owns timer base setup; this helper only stores the binding information. */
    motor->htim_pwm = htim_pwm;
    motor->pwm_channel = pwm_channel;
    motor->dir_port_a = dir_port_a;
    motor->dir_pin_a = dir_pin_a;
    motor->dir_port_b = dir_port_b;
    motor->dir_pin_b = dir_pin_b;
    motor->pwm_max = pwm_max;
    motor->inverted_direction = inverted_direction;
    return true;
}

bool contest_hal_motor_start(contest_hal_motor_t *motor)
{
    if (motor == 0 || motor->htim_pwm == 0) {
        return false;
    }

    return HAL_TIM_PWM_Start(motor->htim_pwm, motor->pwm_channel) == HAL_OK;
}

void contest_hal_motor_set_signed_output(contest_hal_motor_t *motor, int16_t signed_pwm)
{
    uint16_t magnitude;

    if (motor == 0 || motor->htim_pwm == 0) {
        return;
    }

    if (signed_pwm >= 0) {
        contest_hal_motor_set_direction(motor, true);
        magnitude = (uint16_t)signed_pwm;
    } else {
        /* Negative command means reverse direction with the same absolute PWM magnitude. */
        contest_hal_motor_set_direction(motor, false);
        magnitude = (uint16_t)(-signed_pwm);
    }

    if (magnitude > motor->pwm_max) {
        magnitude = motor->pwm_max;
    }

    /* Update timer compare value to control effective motor drive level. */
    __HAL_TIM_SET_COMPARE(motor->htim_pwm, motor->pwm_channel, magnitude);
}
