#include "../generic_c/include/contest_diff_drive.h"

static contest_diff_drive_t g_diff_drive;

void app_diff_drive_init(void)
{
    contest_diff_drive_init(&g_diff_drive, 1.0f, 1.0f, 255.0f);
}

void app_diff_drive_step(float forward_command, float turn_command, float *left_out, float *right_out)
{
    /* A positive turn command will slow the left wheel and speed up the right wheel. */
    contest_diff_drive_mix(&g_diff_drive, forward_command, turn_command, left_out, right_out);
}
