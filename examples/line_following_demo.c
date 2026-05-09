#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../generic_c/include/contest_line_sensor.h"
#include "../generic_c/include/contest_moving_average.h"
#include "../generic_c/include/contest_pid.h"
#include "../generic_c/include/contest_ramp.h"

/*
 * This file is a structure example.
 * Replace the board_* functions with STM32 HAL, Arduino, or ESP32 code.
 */

static void board_read_line_sensors(uint16_t *raw_out, size_t count)
{
    size_t i;

    for (i = 0u; i < count; ++i) {
        raw_out[i] = 0u;
    }
}

static void board_set_motor_targets(float left_target, float right_target)
{
    (void)left_target;
    (void)right_target;
}

static bool board_user_pressed_calibrate(void)
{
    return false;
}

int main(void)
{
    static const float weights[7] = {-3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f};
    float min_values[7];
    float max_values[7];
    float normalized[7];
    float error_filter_storage[5];
    uint16_t raw[7];
    float line_error = 0.0f;
    float filtered_error = 0.0f;
    float correction = 0.0f;
    float base_speed = 180.0f;
    float left_target;
    float right_target;
    contest_line_sensor_t sensors;
    contest_moving_average_t error_filter;
    contest_pid_t pid;
    contest_ramp_t left_ramp;
    contest_ramp_t right_ramp;

    contest_line_sensor_init(&sensors, min_values, max_values, weights, 7u);
    contest_moving_average_init(&error_filter, error_filter_storage, 5u);
    contest_pid_init(&pid, 28.0f, 0.2f, 5.0f, 0.01f, -120.0f, 120.0f);
    contest_pid_set_integral_limits(&pid, -30.0f, 30.0f);
    contest_ramp_init(&left_ramp, 0.0f, 6.0f, 8.0f);
    contest_ramp_init(&right_ramp, 0.0f, 6.0f, 8.0f);

    for (;;) {
        board_read_line_sensors(raw, 7u);

        if (board_user_pressed_calibrate()) {
            contest_line_sensor_calibration_sample(&sensors, raw);
            continue;
        }

        contest_line_sensor_normalize(&sensors, raw, true, normalized);
        if (contest_line_sensor_compute_error(&sensors, normalized, &line_error)) {
            filtered_error = contest_moving_average_update(&error_filter, line_error);
            correction = contest_pid_update_error(&pid, filtered_error);
        } else {
            contest_pid_reset(&pid);
            correction = 0.0f;
        }

        left_target = base_speed - correction;
        right_target = base_speed + correction;

        left_target = contest_ramp_update(&left_ramp, left_target);
        right_target = contest_ramp_update(&right_ramp, right_target);
        board_set_motor_targets(left_target, right_target);
    }
}
