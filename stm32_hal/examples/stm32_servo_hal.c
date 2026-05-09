#include "../include/contest_hal_servo.h"

extern TIM_HandleTypeDef htim4;

static contest_hal_servo_t g_camera_servo;

void app_stm32_servo_init(void)
{
    contest_hal_servo_init(&g_camera_servo, &htim4, TIM_CHANNEL_1, -90.0f, 90.0f, 25u, 125u);
    (void)contest_hal_servo_start(&g_camera_servo);
}

void app_stm32_servo_set_angle(float angle_deg)
{
    /* Use this with a button menu, vision result, or scan strategy. */
    contest_hal_servo_set_angle(&g_camera_servo, angle_deg);
}
