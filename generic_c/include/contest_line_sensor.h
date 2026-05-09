#ifndef CONTEST_LINE_SENSOR_H
#define CONTEST_LINE_SENSOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    float *min_values;
    float *max_values;
    const float *weights;
    size_t count;
} contest_line_sensor_t;

bool contest_line_sensor_init(
    contest_line_sensor_t *sensor,
    float *min_values,
    float *max_values,
    const float *weights,
    size_t count
);
void contest_line_sensor_calibration_reset(contest_line_sensor_t *sensor);
void contest_line_sensor_calibration_sample(contest_line_sensor_t *sensor, const uint16_t *raw);
void contest_line_sensor_normalize(
    const contest_line_sensor_t *sensor,
    const uint16_t *raw,
    bool line_is_dark,
    float *normalized_out
);
bool contest_line_sensor_compute_error(
    const contest_line_sensor_t *sensor,
    const float *normalized,
    float *error_out
);

#endif
