#include "../generic_c/include/contest_complementary_angle.h"

static contest_complementary_angle_t g_angle_filter;

void app_imu_angle_init(float accel_angle_deg)
{
    contest_complementary_angle_init(&g_angle_filter, 0.98f, accel_angle_deg);
}

float app_imu_angle_step(float gyro_rate_dps, float accel_angle_deg)
{
    /* Typical use: call once per fixed sample period, for example every 5 ms. */
    return contest_complementary_angle_update(&g_angle_filter, gyro_rate_dps, accel_angle_deg, 0.005f);
}
