#include "contest_complementary_angle.h"

#include "contest_common.h"

bool contest_complementary_angle_init(
    contest_complementary_angle_t *filter,
    float alpha,
    float initial_angle_deg
)
{
    if (filter == 0) {
        return false;
    }

    filter->alpha = contest_clampf(alpha, 0.0f, 1.0f);
    filter->angle_deg = initial_angle_deg;
    filter->initialized = true;
    return true;
}

void contest_complementary_angle_reset(
    contest_complementary_angle_t *filter,
    float initial_angle_deg
)
{
    if (filter == 0) {
        return;
    }

    filter->angle_deg = initial_angle_deg;
    filter->initialized = true;
}

float contest_complementary_angle_update(
    contest_complementary_angle_t *filter,
    float gyro_rate_dps,
    float accel_angle_deg,
    float dt_s
)
{
    float predicted_angle;

    if (filter == 0 || dt_s <= 0.0f) {
        return accel_angle_deg;
    }

    if (!filter->initialized) {
        filter->angle_deg = accel_angle_deg;
        filter->initialized = true;
        return filter->angle_deg;
    }

    /* Gyro gives fast short-term motion, accelerometer gives slow absolute reference. */
    predicted_angle = filter->angle_deg + (gyro_rate_dps * dt_s);
    filter->angle_deg = (filter->alpha * predicted_angle) + ((1.0f - filter->alpha) * accel_angle_deg);
    return filter->angle_deg;
}
