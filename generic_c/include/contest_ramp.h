#ifndef CONTEST_RAMP_H
#define CONTEST_RAMP_H

typedef struct {
    float value;
    float step_up;
    float step_down;
} contest_ramp_t;

void contest_ramp_init(contest_ramp_t *ramp, float initial_value, float step_up, float step_down);
float contest_ramp_update(contest_ramp_t *ramp, float target);

#endif
