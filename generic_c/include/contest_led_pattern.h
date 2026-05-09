#ifndef CONTEST_LED_PATTERN_H
#define CONTEST_LED_PATTERN_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t on_ticks;
    uint16_t off_ticks;
    uint16_t repeat_target;
    uint16_t repeat_done;
    uint16_t phase_ticks;
    bool active;
    bool output_on;
} contest_led_pattern_t;

void contest_led_pattern_init(contest_led_pattern_t *pattern);
bool contest_led_pattern_start(
    contest_led_pattern_t *pattern,
    uint16_t on_ticks,
    uint16_t off_ticks,
    uint16_t repeat_target
);
void contest_led_pattern_stop(contest_led_pattern_t *pattern);
bool contest_led_pattern_tick(contest_led_pattern_t *pattern);
bool contest_led_pattern_output(const contest_led_pattern_t *pattern);

#endif
