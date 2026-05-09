#include <stdint.h>

#include "../../generic_c/include/contest_incremental_pi.h"
#include "../../generic_c/include/contest_low_pass.h"
#include "../../generic_c/include/contest_ramp.h"
#include "../include/contest_hal_encoder_timer.h"
#include "../include/contest_hal_motor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

static contest_incremental_pi_t g_speed_pi;
static contest_low_pass_t g_speed_filter;
static contest_ramp_t g_speed_target_ramp;
static contest_hal_encoder_timer_t g_left_encoder_timer;
static contest_hal_motor_t g_left_motor;

void app_stm32_speed_loop_init(void)
{
    contest_incremental_pi_init(&g_speed_pi, 0.35f, 1.8f, 0.01f, 0.0f, -999.0f, 999.0f);
    contest_low_pass_init(&g_speed_filter, 0.25f, 0.0f);
    contest_ramp_init(&g_speed_target_ramp, 0.0f, 12.0f, 16.0f);

    contest_hal_encoder_timer_init(&g_left_encoder_timer, &htim3, 1040.0f, 0.01f, 65.0f);
    (void)contest_hal_encoder_timer_start(&g_left_encoder_timer);

    contest_hal_motor_init(&g_left_motor, &htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, 999u, false);
    (void)contest_hal_motor_start(&g_left_motor);
}

void app_stm32_speed_loop_step(float target_rpm)
{
    int32_t delta_count;
    float measured_rpm;
    float filtered_rpm;
    float ramped_target;
    float drive_output;

    /* Hardware-facing part: read timer encoder counts through the HAL wrapper. */
    delta_count = contest_hal_encoder_timer_sample_delta(&g_left_encoder_timer);
    measured_rpm = contest_hal_encoder_timer_delta_to_rpm(&g_left_encoder_timer, delta_count);

    /* Reusable math part: smooth feedback, shape target, then run the PI controller. */
    filtered_rpm = contest_low_pass_update(&g_speed_filter, measured_rpm);
    ramped_target = contest_ramp_update(&g_speed_target_ramp, target_rpm);
    drive_output = contest_incremental_pi_update(&g_speed_pi, ramped_target, filtered_rpm);

    contest_hal_motor_set_signed_output(&g_left_motor, (int16_t)drive_output);
}
