#include "../generic_c/include/contest_complementary_angle.h"
#include "../generic_c/include/contest_incremental_pi.h"
#include "../generic_c/include/contest_periodic_task.h"
#include "../generic_c/include/contest_pid.h"

static contest_complementary_angle_t g_angle_filter;
static contest_pid_t g_angle_pid;
static contest_incremental_pi_t g_speed_pi;
static contest_periodic_task_t g_imu_task;
static contest_periodic_task_t g_motor_task;

void app_balance_car_init(void)
{
    /* Assume the outer tick is 1 ms. Use separate tasks for IMU fusion and motor loop updates. */
    contest_complementary_angle_init(&g_angle_filter, 0.98f, 0.0f);
    contest_pid_init(&g_angle_pid, 38.0f, 0.0f, 1.5f, 0.005f, -250.0f, 250.0f);
    contest_incremental_pi_init(&g_speed_pi, 0.20f, 1.1f, 0.01f, 0.0f, -120.0f, 120.0f);
    contest_periodic_task_init(&g_imu_task, 5u);
    contest_periodic_task_init(&g_motor_task, 10u);
}

void app_balance_car_tick_1ms(void)
{
    contest_periodic_task_tick(&g_imu_task);
    contest_periodic_task_tick(&g_motor_task);
}

float app_balance_car_update_angle(float gyro_rate_dps, float accel_angle_deg)
{
    return contest_complementary_angle_update(&g_angle_filter, gyro_rate_dps, accel_angle_deg, 0.005f);
}

float app_balance_car_step(
    float target_speed_rpm,
    float measured_speed_rpm,
    float fused_angle_deg,
    float target_angle_deg
)
{
    float angle_correction;
    float speed_trim;

    /* Outer speed loop slowly shifts the desired body angle, inner angle loop drives motors. */
    speed_trim = contest_incremental_pi_update(&g_speed_pi, target_speed_rpm, measured_speed_rpm);
    angle_correction = contest_pid_update(&g_angle_pid, target_angle_deg + speed_trim, fused_angle_deg);
    return angle_correction;
}
