#include <stdint.h>

#include "../../generic_c/include/contest_complementary_angle.h"
#include "../../generic_c/include/contest_incremental_pi.h"
#include "../../generic_c/include/contest_periodic_task.h"
#include "../../generic_c/include/contest_pid.h"
#include "../include/contest_hal_encoder_timer.h"
#include "../include/contest_hal_motor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

static contest_complementary_angle_t g_angle_filter;
static contest_pid_t g_angle_pid;
static contest_incremental_pi_t g_speed_pi;
static contest_periodic_task_t g_imu_task;
static contest_periodic_task_t g_motor_task;
static contest_hal_encoder_timer_t g_left_encoder;
static contest_hal_encoder_timer_t g_right_encoder;
static contest_hal_motor_t g_left_motor;
static contest_hal_motor_t g_right_motor;

extern void app_mpu6050_read_pitch_inputs(float *gyro_rate_dps_out, float *accel_angle_deg_out);

void app_stm32_balance_car_init(void)
{
    contest_complementary_angle_init(&g_angle_filter, 0.98f, 0.0f);
    contest_pid_init(&g_angle_pid, 38.0f, 0.0f, 1.5f, 0.005f, -250.0f, 250.0f);
    contest_incremental_pi_init(&g_speed_pi, 0.20f, 1.1f, 0.01f, 0.0f, -120.0f, 120.0f);
    contest_periodic_task_init(&g_imu_task, 5u);
    contest_periodic_task_init(&g_motor_task, 10u);

    contest_hal_encoder_timer_init(&g_left_encoder, &htim3, 1040.0f, 0.01f, 65.0f);
    contest_hal_encoder_timer_init(&g_right_encoder, &htim4, 1040.0f, 0.01f, 65.0f);
    (void)contest_hal_encoder_timer_start(&g_left_encoder);
    (void)contest_hal_encoder_timer_start(&g_right_encoder);

    contest_hal_motor_init(&g_left_motor, &htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, 999u, false);
    contest_hal_motor_init(&g_right_motor, &htim2, TIM_CHANNEL_1, GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_3, 999u, false);
    (void)contest_hal_motor_start(&g_left_motor);
    (void)contest_hal_motor_start(&g_right_motor);
}

void app_stm32_balance_car_tick_1ms(void)
{
    contest_periodic_task_tick(&g_imu_task);
    contest_periodic_task_tick(&g_motor_task);
}

void app_stm32_balance_car_step(float target_speed_rpm)
{
    static float fused_angle_deg = 0.0f;
    float gyro_rate_dps;
    float accel_angle_deg;
    float left_rpm;
    float right_rpm;
    float average_speed_rpm;
    float speed_trim;
    float drive_output;

    if (contest_periodic_task_consume(&g_imu_task)) {
        app_mpu6050_read_pitch_inputs(&gyro_rate_dps, &accel_angle_deg);
        fused_angle_deg = contest_complementary_angle_update(&g_angle_filter, gyro_rate_dps, accel_angle_deg, 0.005f);
    }

    if (!contest_periodic_task_consume(&g_motor_task)) {
        return;
    }

    left_rpm = contest_hal_encoder_timer_delta_to_rpm(
        &g_left_encoder,
        contest_hal_encoder_timer_sample_delta(&g_left_encoder)
    );
    right_rpm = contest_hal_encoder_timer_delta_to_rpm(
        &g_right_encoder,
        contest_hal_encoder_timer_sample_delta(&g_right_encoder)
    );
    average_speed_rpm = (left_rpm + right_rpm) * 0.5f;

    speed_trim = contest_incremental_pi_update(&g_speed_pi, target_speed_rpm, average_speed_rpm);
    drive_output = contest_pid_update(&g_angle_pid, speed_trim, fused_angle_deg);

    /* A real balancing car usually needs more safety limits and fall-detection logic. */
    contest_hal_motor_set_signed_output(&g_left_motor, (int16_t)drive_output);
    contest_hal_motor_set_signed_output(&g_right_motor, (int16_t)drive_output);
}
