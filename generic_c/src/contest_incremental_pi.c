#include "contest_incremental_pi.h"

#include "contest_common.h"

void contest_incremental_pi_init(
    contest_incremental_pi_t *controller,
    float kp,
    float ki,
    float dt_s,
    float initial_output,
    float output_min,
    float output_max
)
{
    if (controller == 0) {
        return;
    }

    controller->kp = kp;
    controller->ki = ki;
    controller->dt_s = dt_s;
    controller->output = initial_output;
    controller->previous_error = 0.0f;
    controller->output_min = output_min;
    controller->output_max = output_max;
}

void contest_incremental_pi_reset(contest_incremental_pi_t *controller, float initial_output)
{
    if (controller == 0) {
        return;
    }

    controller->output = initial_output;
    controller->previous_error = 0.0f;
}

float contest_incremental_pi_update(contest_incremental_pi_t *controller, float setpoint, float measurement)
{
    return contest_incremental_pi_update_error(controller, setpoint - measurement);
}

float contest_incremental_pi_update_error(contest_incremental_pi_t *controller, float error)
{
    float delta_output;

    if (controller == 0 || controller->dt_s <= 0.0f) {
        return 0.0f;
    }

    /*
     * Incremental PI changes the command by a delta each cycle.
     * That is a very common fit for motor PWM speed loops.
     */
    delta_output = (controller->kp * (error - controller->previous_error)) + (controller->ki * error * controller->dt_s);
    controller->output += delta_output;
    controller->output = contest_clampf(controller->output, controller->output_min, controller->output_max);
    controller->previous_error = error;
    return controller->output;
}
