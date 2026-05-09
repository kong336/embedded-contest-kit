#ifndef CONTEST_DIFF_DRIVE_H
#define CONTEST_DIFF_DRIVE_H

#include <stdbool.h>

typedef struct {
    float forward_gain;
    float turn_gain;
    float output_limit;
} contest_diff_drive_t;

bool contest_diff_drive_init(
    contest_diff_drive_t *drive,
    float forward_gain,
    float turn_gain,
    float output_limit
);
void contest_diff_drive_mix(
    const contest_diff_drive_t *drive,
    float forward_command,
    float turn_command,
    float *left_output_out,
    float *right_output_out
);

#endif
