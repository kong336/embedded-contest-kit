#ifndef CONTEST_PID_H
#define CONTEST_PID_H

#include <stdbool.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    float dt_s;
    float integral;
    float previous_error;
    float output_min;
    float output_max;
    float integral_min;
    float integral_max;
    bool first_update;
} contest_pid_t;

void contest_pid_init(
    contest_pid_t *pid,
    float kp,
    float ki,
    float kd,
    float dt_s,
    float output_min,
    float output_max
);
void contest_pid_set_integral_limits(contest_pid_t *pid, float integral_min, float integral_max);
void contest_pid_reset(contest_pid_t *pid);
float contest_pid_update(contest_pid_t *pid, float setpoint, float measurement);
float contest_pid_update_error(contest_pid_t *pid, float error);

#endif
