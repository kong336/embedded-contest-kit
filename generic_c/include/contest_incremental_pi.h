#ifndef CONTEST_INCREMENTAL_PI_H
#define CONTEST_INCREMENTAL_PI_H

typedef struct {
    float kp;
    float ki;
    float dt_s;
    float output;
    float previous_error;
    float output_min;
    float output_max;
} contest_incremental_pi_t;

void contest_incremental_pi_init(
    contest_incremental_pi_t *controller,
    float kp,
    float ki,
    float dt_s,
    float initial_output,
    float output_min,
    float output_max
);
void contest_incremental_pi_reset(contest_incremental_pi_t *controller, float initial_output);
float contest_incremental_pi_update(contest_incremental_pi_t *controller, float setpoint, float measurement);
float contest_incremental_pi_update_error(contest_incremental_pi_t *controller, float error);

#endif
