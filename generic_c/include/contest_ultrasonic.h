#ifndef CONTEST_ULTRASONIC_H
#define CONTEST_ULTRASONIC_H

#include <stdbool.h>

typedef struct {
    float sound_speed_mps;
    float min_distance_cm;
    float max_distance_cm;
} contest_ultrasonic_t;

bool contest_ultrasonic_init(
    contest_ultrasonic_t *sensor,
    float sound_speed_mps,
    float min_distance_cm,
    float max_distance_cm
);
float contest_ultrasonic_echo_us_to_cm(const contest_ultrasonic_t *sensor, float echo_width_us);
bool contest_ultrasonic_measure_cm(
    const contest_ultrasonic_t *sensor,
    float echo_width_us,
    float *distance_cm_out
);

#endif
