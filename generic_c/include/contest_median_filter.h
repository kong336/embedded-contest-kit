#ifndef CONTEST_MEDIAN_FILTER_H
#define CONTEST_MEDIAN_FILTER_H

#include <stdbool.h>
#include <stddef.h>

#define CONTEST_MEDIAN_MAX_WINDOW 9u

typedef struct {
    float *buffer;
    size_t window;
    size_t index;
    size_t count;
} contest_median_filter_t;

bool contest_median_filter_init(contest_median_filter_t *filter, float *storage, size_t window);
void contest_median_filter_reset(contest_median_filter_t *filter);
float contest_median_filter_update(contest_median_filter_t *filter, float sample);

#endif
