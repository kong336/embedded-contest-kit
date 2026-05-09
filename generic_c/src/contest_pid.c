#include "contest_pid.h"

#include "contest_common.h"

void contest_pid_init(
    contest_pid_t *pid,
    float kp,
    float ki,
    float kd,
    float dt_s,
    float output_min,
    float output_max
)
{
    if (pid == 0) {
        return;
    }

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->dt_s = dt_s;
    pid->integral = 0.0f;
    pid->previous_error = 0.0f;
    pid->output_min = output_min;
    pid->output_max = output_max;
    pid->integral_min = output_min;
    pid->integral_max = output_max;
    pid->first_update = true;
}

void contest_pid_set_integral_limits(contest_pid_t *pid, float integral_min, float integral_max)
{
    if (pid == 0) {
        return;
    }

    pid->integral_min = integral_min;
    pid->integral_max = integral_max;
}

void contest_pid_reset(contest_pid_t *pid)
{
    if (pid == 0) {
        return;
    }

    pid->integral = 0.0f;
    pid->previous_error = 0.0f;
    pid->first_update = true;
}

float contest_pid_update(contest_pid_t *pid, float setpoint, float measurement)
{
    return contest_pid_update_error(pid, setpoint - measurement);
}

float contest_pid_update_error(contest_pid_t *pid, float error)
{
    float derivative = 0.0f;
    float output;

    if (pid == 0 || pid->dt_s <= 0.0f) {
        return 0.0f;
    }

    /* Accumulate integral in time domain, then clamp it to avoid windup. */
    pid->integral += error * pid->dt_s;
    pid->integral = contest_clampf(pid->integral, pid->integral_min, pid->integral_max);

    if (!pid->first_update) {
        /* Derivative is based on error change speed between two samples. */
        derivative = (error - pid->previous_error) / pid->dt_s;
    }

    output = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    /* Output limiting keeps the controller inside actuator range. */
    output = contest_clampf(output, pid->output_min, pid->output_max);

    pid->previous_error = error;
    pid->first_update = false;
    return output;
}
