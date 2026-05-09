#include "../include/contest_hal_encoder_timer.h"

bool contest_hal_encoder_timer_init(
    contest_hal_encoder_timer_t *context,
    TIM_HandleTypeDef *htim,
    float counts_per_rev,
    float sample_dt_s,
    float wheel_diameter_mm
)
{
    if (context == 0 || htim == 0) {
        return false;
    }

    context->htim = htim;
    context->counter_period = (uint32_t)__HAL_TIM_GET_AUTORELOAD(htim);
    context->last_counter = (uint32_t)__HAL_TIM_GET_COUNTER(htim);
    contest_encoder_init(&context->encoder, 0, counts_per_rev, sample_dt_s, wheel_diameter_mm);
    return true;
}

bool contest_hal_encoder_timer_start(contest_hal_encoder_timer_t *context)
{
    if (context == 0 || context->htim == 0) {
        return false;
    }

    return HAL_TIM_Encoder_Start(context->htim, TIM_CHANNEL_ALL) == HAL_OK;
}

void contest_hal_encoder_timer_reset(contest_hal_encoder_timer_t *context)
{
    if (context == 0 || context->htim == 0) {
        return;
    }

    context->last_counter = (uint32_t)__HAL_TIM_GET_COUNTER(context->htim);
}

int32_t contest_hal_encoder_timer_sample_delta(contest_hal_encoder_timer_t *context)
{
    uint32_t current_counter;
    int32_t delta;
    int32_t range;
    int32_t half_range;

    if (context == 0 || context->htim == 0) {
        return 0;
    }

    current_counter = (uint32_t)__HAL_TIM_GET_COUNTER(context->htim);
    delta = (int32_t)current_counter - (int32_t)context->last_counter;
    range = (int32_t)context->counter_period + 1;
    half_range = range / 2;

    /*
     * Encoder timers wrap around, so fold the count delta back into a signed
     * range around zero instead of treating wrap as a giant speed spike.
     */
    if (delta > half_range) {
        delta -= range;
    } else if (delta < -half_range) {
        delta += range;
    }

    context->last_counter = current_counter;
    return delta;
}

float contest_hal_encoder_timer_delta_to_rpm(const contest_hal_encoder_timer_t *context, int32_t delta_count)
{
    if (context == 0) {
        return 0.0f;
    }

    return contest_encoder_delta_to_rpm(&context->encoder, delta_count);
}

float contest_hal_encoder_timer_delta_to_speed_mmps(const contest_hal_encoder_timer_t *context, int32_t delta_count)
{
    if (context == 0) {
        return 0.0f;
    }

    return contest_encoder_delta_to_speed_mmps(&context->encoder, delta_count);
}
