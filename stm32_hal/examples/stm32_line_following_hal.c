#include <stdbool.h>
#include <stdint.h>

#include "../../generic_c/include/contest_line_sensor.h"
#include "../../generic_c/include/contest_moving_average.h"
#include "../../generic_c/include/contest_pid.h"
#include "../../generic_c/include/contest_ramp.h"
#include "../include/contest_hal_adc_scan.h"
#include "../include/contest_hal_motor.h"

/*
 * This example shows the role split:
 * - STM32 HAL code reads hardware
 * - generic modules do math and control
 * Replace the external handles below with your CubeMX-generated ones.
 */

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

static const float line_weights[7] = {-3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f};
static float sensor_min[7];
static float sensor_max[7];
static float sensor_norm[7];
static float error_filter_storage[5];
static uint16_t raw_sensor_values[7];

static contest_line_sensor_t g_line_sensor;
static contest_moving_average_t g_error_filter;
static contest_pid_t g_steering_pid;
static contest_ramp_t g_left_ramp;
static contest_ramp_t g_right_ramp;
static contest_hal_adc_scan_t g_adc_scan;
static contest_hal_motor_t g_left_motor;
static contest_hal_motor_t g_right_motor;

static void app_set_signed_motor_outputs(float left, float right)
{
    /* Cast here because the HAL motor helper is intended for signed PWM targets. */
    contest_hal_motor_set_signed_output(&g_left_motor, (int16_t)left);
    contest_hal_motor_set_signed_output(&g_right_motor, (int16_t)right);
}

void app_line_following_init(void)
{
    contest_line_sensor_init(&g_line_sensor, sensor_min, sensor_max, line_weights, 7u);
    contest_moving_average_init(&g_error_filter, error_filter_storage, 5u);
    contest_pid_init(&g_steering_pid, 28.0f, 0.2f, 5.0f, 0.01f, -120.0f, 120.0f);
    contest_pid_set_integral_limits(&g_steering_pid, -30.0f, 30.0f);
    contest_ramp_init(&g_left_ramp, 0.0f, 6.0f, 8.0f);
    contest_ramp_init(&g_right_ramp, 0.0f, 6.0f, 8.0f);

    contest_hal_adc_scan_init(&g_adc_scan, &hadc1, 5u);

    /* Replace GPIO ports, pins, timers, and channels with your actual hardware binding. */
    contest_hal_motor_init(&g_left_motor, &htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, 999u, false);
    contest_hal_motor_init(&g_right_motor, &htim2, TIM_CHANNEL_1, GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_3, 999u, false);
    (void)contest_hal_motor_start(&g_left_motor);
    (void)contest_hal_motor_start(&g_right_motor);
}

void app_line_following_calibrate_sample(void)
{
    if (contest_hal_adc_scan_read(&g_adc_scan, raw_sensor_values, 7u)) {
        contest_line_sensor_calibration_sample(&g_line_sensor, raw_sensor_values);
    }
}

void app_line_following_step(void)
{
    const float base_speed = 180.0f;
    float error;
    float filtered_error;
    float correction;
    float left_target;
    float right_target;

    if (!contest_hal_adc_scan_read(&g_adc_scan, raw_sensor_values, 7u)) {
        return;
    }

    contest_line_sensor_normalize(&g_line_sensor, raw_sensor_values, true, sensor_norm);
    if (!contest_line_sensor_compute_error(&g_line_sensor, sensor_norm, &error)) {
        contest_pid_reset(&g_steering_pid);
        app_set_signed_motor_outputs(0.0f, 0.0f);
        return;
    }

    filtered_error = contest_moving_average_update(&g_error_filter, error);
    correction = contest_pid_update_error(&g_steering_pid, filtered_error);

    left_target = contest_ramp_update(&g_left_ramp, base_speed - correction);
    right_target = contest_ramp_update(&g_right_ramp, base_speed + correction);
    app_set_signed_motor_outputs(left_target, right_target);
}
