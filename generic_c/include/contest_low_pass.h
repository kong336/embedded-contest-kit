#ifndef CONTEST_LOW_PASS_H
#define CONTEST_LOW_PASS_H

#include <stdbool.h>

typedef struct {
    float alpha;
    float value;
    bool initialized;
} contest_low_pass_t;

bool contest_low_pass_init(contest_low_pass_t *filter, float alpha, float initial_value);
void contest_low_pass_reset(contest_low_pass_t *filter, float initial_value);
float contest_low_pass_update(contest_low_pass_t *filter, float sample);

#endif
