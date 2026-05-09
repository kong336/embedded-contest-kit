#include "../include/contest_hal_diff_drive.h"

#include <stdint.h>

bool contest_hal_diff_drive_init(
    contest_hal_diff_drive_t *drive,
    contest_hal_motor_t *left_motor,
    contest_hal_motor_t *right_motor,
    float forward_gain,
    float turn_gain,
    float output_limit
)
{
    if (drive == 0 || left_motor == 0 || right_motor == 0) {
        return false;
    }

    drive->left_motor = left_motor;
    drive->right_motor = right_motor;
    return contest_diff_drive_init(&drive->mixer, forward_gain, turn_gain, output_limit);
}

void contest_hal_diff_drive_set_command(
    contest_hal_diff_drive_t *drive,
    float forward_command,
    float turn_command
)
{
    float left_output;
    float right_output;

    if (drive == 0 || drive->left_motor == 0 || drive->right_motor == 0) {
        return;
    }

    /* Keep steering math separate from the low-level motor direction and PWM helpers. */
    contest_diff_drive_mix(&drive->mixer, forward_command, turn_command, &left_output, &right_output);
    contest_hal_motor_set_signed_output(drive->left_motor, (int16_t)left_output);
    contest_hal_motor_set_signed_output(drive->right_motor, (int16_t)right_output);
}
