#ifndef CONTEST_MOVING_AVERAGE_H
#define CONTEST_MOVING_AVERAGE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float *buffer;
    size_t window;
    size_t index;
    size_t count;
    float sum;
} contest_moving_average_t;

bool contest_moving_average_init(contest_moving_average_t *filter, float *storage, size_t window);
void contest_moving_average_reset(contest_moving_average_t *filter);
float contest_moving_average_update(contest_moving_average_t *filter, float sample);

#endif
