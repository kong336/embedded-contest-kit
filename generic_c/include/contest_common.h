#ifndef CONTEST_COMMON_H
#define CONTEST_COMMON_H

#include <stdint.h>

#define CONTEST_ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#define CONTEST_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CONTEST_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline float contest_clampf(float value, float min_value, float max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

static inline int32_t contest_clampi32(int32_t value, int32_t min_value, int32_t max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

static inline float contest_absf(float value)
{
    return value < 0.0f ? -value : value;
}

static inline float contest_apply_deadband(float value, float deadband)
{
    if (contest_absf(value) <= deadband) {
        return 0.0f;
    }
    return value;
}

#endif
