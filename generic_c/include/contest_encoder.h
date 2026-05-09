#ifndef CONTEST_ENCODER_H
#define CONTEST_ENCODER_H

#include <stdint.h>

typedef struct {
    int32_t previous_count;
    float counts_per_rev;
    float sample_dt_s;
    float wheel_diameter_mm;
} contest_encoder_t;

void contest_encoder_init(
    contest_encoder_t *encoder,
    int32_t initial_count,
    float counts_per_rev,
    float sample_dt_s,
    float wheel_diameter_mm
);
int32_t contest_encoder_sample_delta(contest_encoder_t *encoder, int32_t current_count);
float contest_encoder_delta_to_rpm(const contest_encoder_t *encoder, int32_t delta_count);
float contest_encoder_delta_to_speed_mmps(const contest_encoder_t *encoder, int32_t delta_count);

#endif
