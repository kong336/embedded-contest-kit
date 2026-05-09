#include "contest_led_pattern.h"

#include <limits.h>

void contest_led_pattern_init(contest_led_pattern_t *pattern)
{
    if (pattern == 0) {
        return;
    }

    pattern->on_ticks = 0u;
    pattern->off_ticks = 0u;
    pattern->repeat_target = 0u;
    pattern->repeat_done = 0u;
    pattern->phase_ticks = 0u;
    pattern->active = false;
    pattern->output_on = false;
}

bool contest_led_pattern_start(
    contest_led_pattern_t *pattern,
    uint16_t on_ticks,
    uint16_t off_ticks,
    uint16_t repeat_target
)
{
    if (pattern == 0 || on_ticks == 0u) {
        return false;
    }

    pattern->on_ticks = on_ticks;
    pattern->off_ticks = off_ticks;
    pattern->repeat_target = repeat_target;
    pattern->repeat_done = 0u;
    pattern->phase_ticks = 0u;
    pattern->active = true;
    pattern->output_on = true;
    return true;
}

void contest_led_pattern_stop(contest_led_pattern_t *pattern)
{
    if (pattern == 0) {
        return;
    }

    pattern->repeat_done = 0u;
    pattern->phase_ticks = 0u;
    pattern->active = false;
    pattern->output_on = false;
}

bool contest_led_pattern_tick(contest_led_pattern_t *pattern)
{
    if (pattern == 0 || !pattern->active) {
        return false;
    }

    if (pattern->phase_ticks < UINT16_MAX) {
        pattern->phase_ticks += 1u;
    }

    if (pattern->output_on) {
        if (pattern->phase_ticks >= pattern->on_ticks) {
            pattern->phase_ticks = 0u;
            if (pattern->off_ticks == 0u) {
                if (pattern->repeat_target > 0u) {
                    pattern->repeat_done += 1u;
                    if (pattern->repeat_done >= pattern->repeat_target) {
                        contest_led_pattern_stop(pattern);
                        return false;
                    }
                }
            } else {
                pattern->output_on = false;
            }
        }
    } else if (pattern->phase_ticks >= pattern->off_ticks) {
        pattern->phase_ticks = 0u;

        if (pattern->repeat_target > 0u) {
            pattern->repeat_done += 1u;
            if (pattern->repeat_done >= pattern->repeat_target) {
                contest_led_pattern_stop(pattern);
                return false;
            }
        }

        pattern->output_on = true;
    }

    return pattern->output_on;
}

bool contest_led_pattern_output(const contest_led_pattern_t *pattern)
{
    if (pattern == 0) {
        return false;
    }

    return pattern->output_on;
}
