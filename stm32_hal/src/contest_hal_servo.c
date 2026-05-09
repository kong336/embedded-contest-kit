#include "../include/contest_hal_servo.h"

bool contest_hal_servo_init(
    contest_hal_servo_t *servo,
    TIM_HandleTypeDef *htim_pwm,
    uint32_t pwm_channel,
    float angle_min_deg,
    float angle_max_deg,
    uint16_t compare_min,
    uint16_t compare_max
)
{
    if (servo == 0 || htim_pwm == 0) {
        return false;
    }

    servo->htim_pwm = htim_pwm;
    servo->pwm_channel = pwm_channel;
    return contest_servo_map_init(&servo->map, angle_min_deg, angle_max_deg, compare_min, compare_max);
}

bool contest_hal_servo_start(contest_hal_servo_t *servo)
{
    if (servo == 0 || servo->htim_pwm == 0) {
        return false;
    }

    return HAL_TIM_PWM_Start(servo->htim_pwm, servo->pwm_channel) == HAL_OK;
}

void contest_hal_servo_set_compare(contest_hal_servo_t *servo, uint16_t compare_value)
{
    if (servo == 0 || servo->htim_pwm == 0) {
        return;
    }

    __HAL_TIM_SET_COMPARE(servo->htim_pwm, servo->pwm_channel, compare_value);
}

void contest_hal_servo_set_angle(contest_hal_servo_t *servo, float angle_deg)
{
    uint16_t compare_value;

    if (servo == 0) {
        return;
    }

    compare_value = contest_servo_map_angle_to_compare(&servo->map, angle_deg);
    contest_hal_servo_set_compare(servo, compare_value);
}
