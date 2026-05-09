#include "contest_low_pass.h"

#include "contest_common.h"

bool contest_low_pass_init(contest_low_pass_t *filter, float alpha, float initial_value)
{
    if (filter == 0) {
        return false;
    }

    filter->alpha = contest_clampf(alpha, 0.0f, 1.0f);
    filter->value = initial_value;
    filter->initialized = true;
    return true;
}

void contest_low_pass_reset(contest_low_pass_t *filter, float initial_value)
{
    if (filter == 0) {
        return;
    }

    filter->value = initial_value;
    filter->initialized = true;
}

float contest_low_pass_update(contest_low_pass_t *filter, float sample)
{
    if (filter == 0) {
        return sample;
    }

    if (!filter->initialized) {
        filter->value = sample;
        filter->initialized = true;
        return sample;
    }

    /* First-order low-pass: only move part of the way toward the new sample. */
    filter->value += filter->alpha * (sample - filter->value);
    return filter->value;
}
