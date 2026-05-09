#include "../generic_c/include/contest_ramp.h"
#include "../generic_c/include/contest_servo_map.h"

static contest_servo_map_t g_servo_map;
static contest_ramp_t g_servo_ramp;

void app_servo_scan_init(void)
{
    contest_servo_map_init(&g_servo_map, -90.0f, 90.0f, 25u, 125u);
    contest_ramp_init(&g_servo_ramp, 0.0f, 2.0f, 3.0f);
}

unsigned short app_servo_scan_step(float target_angle_deg)
{
    float smoothed_angle;

    /* Smooth angle changes first, then convert the result into a PWM compare value. */
    smoothed_angle = contest_ramp_update(&g_servo_ramp, target_angle_deg);
    return contest_servo_map_angle_to_compare(&g_servo_map, smoothed_angle);
}
