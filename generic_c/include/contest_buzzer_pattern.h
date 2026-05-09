#ifndef CONTEST_BUZZER_PATTERN_H
#define CONTEST_BUZZER_PATTERN_H

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
} contest_buzzer_pattern_t;

void contest_buzzer_pattern_init(contest_buzzer_pattern_t *pattern);
bool contest_buzzer_pattern_start(
    contest_buzzer_pattern_t *pattern,
    uint16_t on_ticks,
    uint16_t off_ticks,
    uint16_t repeat_target
);
void contest_buzzer_pattern_stop(contest_buzzer_pattern_t *pattern);
bool contest_buzzer_pattern_tick(contest_buzzer_pattern_t *pattern);
bool contest_buzzer_pattern_is_active(const contest_buzzer_pattern_t *pattern);
bool contest_buzzer_pattern_output(const contest_buzzer_pattern_t *pattern);

#endif
