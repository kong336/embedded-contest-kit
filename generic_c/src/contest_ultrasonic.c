#include "contest_ultrasonic.h"

bool contest_ultrasonic_init(
    contest_ultrasonic_t *sensor,
    float sound_speed_mps,
    float min_distance_cm,
    float max_distance_cm
)
{
    if (sensor == 0 || sound_speed_mps <= 0.0f || max_distance_cm <= min_distance_cm) {
        return false;
    }

    sensor->sound_speed_mps = sound_speed_mps;
    sensor->min_distance_cm = min_distance_cm;
    sensor->max_distance_cm = max_distance_cm;
    return true;
}

float contest_ultrasonic_echo_us_to_cm(const contest_ultrasonic_t *sensor, float echo_width_us)
{
    if (sensor == 0 || echo_width_us <= 0.0f) {
        return 0.0f;
    }

    /*
     * Echo time is the round-trip duration.
     * Convert microseconds to seconds, apply sound speed, then divide by two.
     */
    return (echo_width_us * sensor->sound_speed_mps * 100.0f) / 2000000.0f;
}

bool contest_ultrasonic_measure_cm(
    const contest_ultrasonic_t *sensor,
    float echo_width_us,
    float *distance_cm_out
)
{
    float distance_cm;

    if (sensor == 0 || distance_cm_out == 0) {
        return false;
    }

    distance_cm = contest_ultrasonic_echo_us_to_cm(sensor, echo_width_us);
    if (distance_cm < sensor->min_distance_cm || distance_cm > sensor->max_distance_cm) {
        return false;
    }

    *distance_cm_out = distance_cm;
    return true;
}
