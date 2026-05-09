#include "contest_ramp.h"

void contest_ramp_init(contest_ramp_t *ramp, float initial_value, float step_up, float step_down)
{
    if (ramp == 0) {
        return;
    }

    ramp->value = initial_value;
    ramp->step_up = step_up;
    ramp->step_down = step_down;
}

float contest_ramp_update(contest_ramp_t *ramp, float target)
{
    float diff;

    if (ramp == 0) {
        return target;
    }

    diff = target - ramp->value;
    if (diff > ramp->step_up) {
        ramp->value += ramp->step_up;
    } else if (diff < -ramp->step_down) {
        ramp->value -= ramp->step_down;
    } else {
        ramp->value = target;
    }

    return ramp->value;
}
