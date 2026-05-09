#ifndef CONTEST_VALUE_TUNER_H
#define CONTEST_VALUE_TUNER_H

#include <stdbool.h>

typedef struct {
    const char *label;
    float value;
    float default_value;
    float min_value;
    float max_value;
    float coarse_step;
    float fine_step;
} contest_value_tuner_t;

bool contest_value_tuner_init(
    contest_value_tuner_t *tuner,
    const char *label,
    float default_value,
    float min_value,
    float max_value,
    float coarse_step,
    float fine_step
);
void contest_value_tuner_reset(contest_value_tuner_t *tuner);
float contest_value_tuner_adjust(contest_value_tuner_t *tuner, int direction, bool fine_mode);
bool contest_value_tuner_set(contest_value_tuner_t *tuner, float value);

#endif
