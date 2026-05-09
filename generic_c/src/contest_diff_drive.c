#include "contest_diff_drive.h"

#include "contest_common.h"

bool contest_diff_drive_init(
    contest_diff_drive_t *drive,
    float forward_gain,
    float turn_gain,
    float output_limit
)
{
    if (drive == 0 || output_limit <= 0.0f) {
        return false;
    }

    drive->forward_gain = forward_gain;
    drive->turn_gain = turn_gain;
    drive->output_limit = output_limit;
    return true;
}

void contest_diff_drive_mix(
    const contest_diff_drive_t *drive,
    float forward_command,
    float turn_command,
    float *left_output_out,
    float *right_output_out
)
{
    float left_output;
    float right_output;
    float max_abs;
    float scale;

    if (drive == 0 || left_output_out == 0 || right_output_out == 0 || drive->output_limit <= 0.0f) {
        return;
    }

    left_output = (forward_command * drive->forward_gain) - (turn_command * drive->turn_gain);
    right_output = (forward_command * drive->forward_gain) + (turn_command * drive->turn_gain);

    /*
     * Scale both channels together when the mixed result exceeds the allowed
     * magnitude so differential-drive behavior stays proportional.
     */
    max_abs = CONTEST_MAX(contest_absf(left_output), contest_absf(right_output));
    if (max_abs > drive->output_limit) {
        scale = drive->output_limit / max_abs;
        left_output *= scale;
        right_output *= scale;
    }

    *left_output_out = left_output;
    *right_output_out = right_output;
}
