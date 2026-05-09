#include "contest_line_sensor.h"

#include "contest_common.h"

bool contest_line_sensor_init(
    contest_line_sensor_t *sensor,
    float *min_values,
    float *max_values,
    const float *weights,
    size_t count
)
{
    if (sensor == 0 || min_values == 0 || max_values == 0 || weights == 0 || count == 0u) {
        return false;
    }

    sensor->min_values = min_values;
    sensor->max_values = max_values;
    sensor->weights = weights;
    sensor->count = count;
    contest_line_sensor_calibration_reset(sensor);
    return true;
}

void contest_line_sensor_calibration_reset(contest_line_sensor_t *sensor)
{
    size_t i;

    if (sensor == 0) {
        return;
    }

    for (i = 0; i < sensor->count; ++i) {
        sensor->min_values[i] = 1.0e9f;
        sensor->max_values[i] = -1.0e9f;
    }
}

void contest_line_sensor_calibration_sample(contest_line_sensor_t *sensor, const uint16_t *raw)
{
    size_t i;

    if (sensor == 0 || raw == 0) {
        return;
    }

    for (i = 0; i < sensor->count; ++i) {
        float value = (float)raw[i];
        if (value < sensor->min_values[i]) {
            sensor->min_values[i] = value;
        }
        if (value > sensor->max_values[i]) {
            sensor->max_values[i] = value;
        }
    }
}

void contest_line_sensor_normalize(
    const contest_line_sensor_t *sensor,
    const uint16_t *raw,
    bool line_is_dark,
    float *normalized_out
)
{
    size_t i;

    if (sensor == 0 || raw == 0 || normalized_out == 0) {
        return;
    }

    for (i = 0; i < sensor->count; ++i) {
        float span = sensor->max_values[i] - sensor->min_values[i];
        float normalized = 0.0f;

        if (span > 1.0e-6f) {
            /* Map calibrated raw value into the 0..1 interval. */
            normalized = ((float)raw[i] - sensor->min_values[i]) / span;
            normalized = contest_clampf(normalized, 0.0f, 1.0f);
        }

        /* For dark-line tracking, invert the brightness-style normalized value. */
        normalized_out[i] = line_is_dark ? (1.0f - normalized) : normalized;
    }
}

bool contest_line_sensor_compute_error(
    const contest_line_sensor_t *sensor,
    const float *normalized,
    float *error_out
)
{
    size_t i;
    float weighted_sum = 0.0f;
    float strength_sum = 0.0f;

    if (sensor == 0 || normalized == 0 || error_out == 0) {
        return false;
    }

    for (i = 0; i < sensor->count; ++i) {
        /* Weighted average gives a continuous line position estimate. */
        weighted_sum += sensor->weights[i] * normalized[i];
        strength_sum += normalized[i];
    }

    if (strength_sum <= 1.0e-6f) {
        return false;
    }

    *error_out = weighted_sum / strength_sum;
    return true;
}
