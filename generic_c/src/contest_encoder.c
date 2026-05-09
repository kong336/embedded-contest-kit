#include "contest_encoder.h"

void contest_encoder_init(
    contest_encoder_t *encoder,
    int32_t initial_count,
    float counts_per_rev,
    float sample_dt_s,
    float wheel_diameter_mm
)
{
    if (encoder == 0) {
        return;
    }

    encoder->previous_count = initial_count;
    encoder->counts_per_rev = counts_per_rev;
    encoder->sample_dt_s = sample_dt_s;
    encoder->wheel_diameter_mm = wheel_diameter_mm;
}

int32_t contest_encoder_sample_delta(contest_encoder_t *encoder, int32_t current_count)
{
    int32_t delta;

    if (encoder == 0) {
        return 0;
    }

    delta = current_count - encoder->previous_count;
    encoder->previous_count = current_count;
    return delta;
}

float contest_encoder_delta_to_rpm(const contest_encoder_t *encoder, int32_t delta_count)
{
    if (encoder == 0 || encoder->counts_per_rev <= 0.0f || encoder->sample_dt_s <= 0.0f) {
        return 0.0f;
    }

    return ((float)delta_count / encoder->counts_per_rev) * (60.0f / encoder->sample_dt_s);
}

float contest_encoder_delta_to_speed_mmps(const contest_encoder_t *encoder, int32_t delta_count)
{
    float rpm;
    float wheel_perimeter_mm;

    if (encoder == 0 || encoder->wheel_diameter_mm <= 0.0f) {
        return 0.0f;
    }

    rpm = contest_encoder_delta_to_rpm(encoder, delta_count);
    wheel_perimeter_mm = 3.1415926f * encoder->wheel_diameter_mm;
    return (rpm * wheel_perimeter_mm) / 60.0f;
}
