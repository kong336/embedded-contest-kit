#ifndef CONTEST_COMPLEMENTARY_ANGLE_H
#define CONTEST_COMPLEMENTARY_ANGLE_H

#include <stdbool.h>

typedef struct {
    float alpha;
    float angle_deg;
    bool initialized;
} contest_complementary_angle_t;

bool contest_complementary_angle_init(
    contest_complementary_angle_t *filter,
    float alpha,
    float initial_angle_deg
);
void contest_complementary_angle_reset(
    contest_complementary_angle_t *filter,
    float initial_angle_deg
);
float contest_complementary_angle_update(
    contest_complementary_angle_t *filter,
    float gyro_rate_dps,
    float accel_angle_deg,
    float dt_s
);

#endif
