#include "contest_value_tuner.h"

#include "contest_common.h"

bool contest_value_tuner_init(
    contest_value_tuner_t *tuner,
    const char *label,
    float default_value,
    float min_value,
    float max_value,
    float coarse_step,
    float fine_step
)
{
    if (tuner == 0 || max_value < min_value || coarse_step <= 0.0f || fine_step <= 0.0f) {
        return false;
    }

    tuner->label = label;
    tuner->default_value = contest_clampf(default_value, min_value, max_value);
    tuner->value = tuner->default_value;
    tuner->min_value = min_value;
    tuner->max_value = max_value;
    tuner->coarse_step = coarse_step;
    tuner->fine_step = fine_step;
    return true;
}

void contest_value_tuner_reset(contest_value_tuner_t *tuner)
{
    if (tuner == 0) {
        return;
    }

    tuner->value = tuner->default_value;
}

float contest_value_tuner_adjust(contest_value_tuner_t *tuner, int direction, bool fine_mode)
{
    float step;

    if (tuner == 0 || direction == 0) {
        return 0.0f;
    }

    step = fine_mode ? tuner->fine_step : tuner->coarse_step;
    tuner->value += (float)direction * step;
    tuner->value = contest_clampf(tuner->value, tuner->min_value, tuner->max_value);
    return tuner->value;
}

bool contest_value_tuner_set(contest_value_tuner_t *tuner, float value)
{
    if (tuner == 0) {
        return false;
    }

    tuner->value = contest_clampf(value, tuner->min_value, tuner->max_value);
    return true;
}
