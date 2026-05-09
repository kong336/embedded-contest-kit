#include "contest_median_filter.h"

static void contest_sort_small_array(float *values, size_t length)
{
    size_t i;

    for (i = 1u; i < length; ++i) {
        float key = values[i];
        size_t j = i;

        while (j > 0u && values[j - 1u] > key) {
            values[j] = values[j - 1u];
            j -= 1u;
        }

        values[j] = key;
    }
}

bool contest_median_filter_init(contest_median_filter_t *filter, float *storage, size_t window)
{
    size_t i;

    if (filter == 0 || storage == 0 || window == 0u || window > CONTEST_MEDIAN_MAX_WINDOW) {
        return false;
    }

    filter->buffer = storage;
    filter->window = window;
    filter->index = 0u;
    filter->count = 0u;

    for (i = 0; i < window; ++i) {
        filter->buffer[i] = 0.0f;
    }

    return true;
}

void contest_median_filter_reset(contest_median_filter_t *filter)
{
    size_t i;

    if (filter == 0 || filter->buffer == 0) {
        return;
    }

    filter->index = 0u;
    filter->count = 0u;

    for (i = 0; i < filter->window; ++i) {
        filter->buffer[i] = 0.0f;
    }
}

float contest_median_filter_update(contest_median_filter_t *filter, float sample)
{
    float sorted[CONTEST_MEDIAN_MAX_WINDOW];
    size_t i;
    size_t mid;

    if (filter == 0 || filter->buffer == 0 || filter->window == 0u) {
        return sample;
    }

    filter->buffer[filter->index] = sample;
    filter->index = (filter->index + 1u) % filter->window;
    if (filter->count < filter->window) {
        filter->count += 1u;
    }

    for (i = 0; i < filter->count; ++i) {
        sorted[i] = filter->buffer[i];
    }

    contest_sort_small_array(sorted, filter->count);
    mid = filter->count / 2u;

    if ((filter->count % 2u) == 0u) {
        return 0.5f * (sorted[mid - 1u] + sorted[mid]);
    }

    return sorted[mid];
}
