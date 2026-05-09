#ifndef CONTEST_DEBOUNCE_H
#define CONTEST_DEBOUNCE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool stable_state;
    uint16_t threshold_ticks;
    uint16_t counter;
} contest_debounce_t;

void contest_debounce_init(contest_debounce_t *debounce, bool initial_state, uint16_t threshold_ticks);
bool contest_debounce_update(contest_debounce_t *debounce, bool raw_state);
bool contest_debounce_state(const contest_debounce_t *debounce);

#endif
