#include <stdint.h>

#include "../generic_c/include/contest_encoder.h"
#include "../generic_c/include/contest_incremental_pi.h"
#include "../generic_c/include/contest_low_pass.h"
#include "../generic_c/include/contest_ramp.h"

static contest_encoder_t g_left_encoder;
static contest_incremental_pi_t g_speed_pi;
static contest_low_pass_t g_speed_filter;
static contest_ramp_t g_speed_target_ramp;

void app_speed_loop_init(int32_t initial_encoder_count)
{
    contest_encoder_init(&g_left_encoder, initial_encoder_count, 1040.0f, 0.01f, 65.0f);
    contest_incremental_pi_init(&g_speed_pi, 0.35f, 1.8f, 0.01f, 0.0f, -999.0f, 999.0f);
    contest_low_pass_init(&g_speed_filter, 0.25f, 0.0f);
    contest_ramp_init(&g_speed_target_ramp, 0.0f, 15.0f, 20.0f);
}

float app_speed_loop_step(int32_t current_encoder_count, float target_rpm)
{
    int32_t delta_count;
    float measured_rpm;
    float filtered_rpm;
    float ramped_target;

    /* Sample pulse delta first, then convert it into an actual speed quantity. */
    delta_count = contest_encoder_sample_delta(&g_left_encoder, current_encoder_count);
    measured_rpm = contest_encoder_delta_to_rpm(&g_left_encoder, delta_count);

    /* Smooth the feedback and also limit target speed jumps before the PI loop. */
    filtered_rpm = contest_low_pass_update(&g_speed_filter, measured_rpm);
    ramped_target = contest_ramp_update(&g_speed_target_ramp, target_rpm);

    /* The returned value can be mapped to PWM duty, DAC output, or another drive command. */
    return contest_incremental_pi_update(&g_speed_pi, ramped_target, filtered_rpm);
}
