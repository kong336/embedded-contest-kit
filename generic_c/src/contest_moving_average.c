#include "contest_moving_average.h"

bool contest_moving_average_init(contest_moving_average_t *filter, float *storage, size_t window)
{
    size_t i;

    if (filter == 0 || storage == 0 || window == 0u) {
        return false;
    }

    filter->buffer = storage;
    filter->window = window;
    filter->index = 0u;
    filter->count = 0u;
    filter->sum = 0.0f;

    for (i = 0; i < window; ++i) {
        filter->buffer[i] = 0.0f;
    }

    return true;
}

void contest_moving_average_reset(contest_moving_average_t *filter)
{
    size_t i;

    if (filter == 0 || filter->buffer == 0) {
        return;
    }

    filter->index = 0u;
    filter->count = 0u;
    filter->sum = 0.0f;

    for (i = 0; i < filter->window; ++i) {
        filter->buffer[i] = 0.0f;
    }
}

float contest_moving_average_update(contest_moving_average_t *filter, float sample)
{
    if (filter == 0 || filter->buffer == 0 || filter->window == 0u) {
        return sample;
    }

    if (filter->count < filter->window) {
        filter->count += 1u;
    } else {
        filter->sum -= filter->buffer[filter->index];
    }

    filter->buffer[filter->index] = sample;
    filter->sum += sample;
    filter->index = (filter->index + 1u) % filter->window;

    return filter->sum / (float)filter->count;
}
