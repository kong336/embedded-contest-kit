#include "../include/contest_hal_diff_drive.h"
#include "../include/contest_hal_motor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

static contest_hal_motor_t g_left_motor;
static contest_hal_motor_t g_right_motor;
static contest_hal_diff_drive_t g_drive;

void app_stm32_diff_drive_init(void)
{
    contest_hal_motor_init(&g_left_motor, &htim1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, 999u, false);
    contest_hal_motor_init(&g_right_motor, &htim2, TIM_CHANNEL_1, GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_3, 999u, false);
    (void)contest_hal_motor_start(&g_left_motor);
    (void)contest_hal_motor_start(&g_right_motor);

    contest_hal_diff_drive_init(&g_drive, &g_left_motor, &g_right_motor, 1.0f, 1.0f, 999.0f);
}

void app_stm32_diff_drive_step(float forward_command, float turn_command)
{
    /* This is a good fit for joystick control, line following, or vision steering offsets. */
    contest_hal_diff_drive_set_command(&g_drive, forward_command, turn_command);
}
