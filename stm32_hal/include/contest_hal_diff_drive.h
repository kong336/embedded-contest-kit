#ifndef CONTEST_HAL_DIFF_DRIVE_H
#define CONTEST_HAL_DIFF_DRIVE_H

#include <stdbool.h>

#include "../../generic_c/include/contest_diff_drive.h"
#include "contest_hal_motor.h"

typedef struct {
    contest_hal_motor_t *left_motor;
    contest_hal_motor_t *right_motor;
    contest_diff_drive_t mixer;
} contest_hal_diff_drive_t;

bool contest_hal_diff_drive_init(
    contest_hal_diff_drive_t *drive,
    contest_hal_motor_t *left_motor,
    contest_hal_motor_t *right_motor,
    float forward_gain,
    float turn_gain,
    float output_limit
);
void contest_hal_diff_drive_set_command(
    contest_hal_diff_drive_t *drive,
    float forward_command,
    float turn_command
);

#endif
