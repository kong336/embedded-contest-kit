#include "contest_debounce.h"

void contest_debounce_init(contest_debounce_t *debounce, bool initial_state, uint16_t threshold_ticks)
{
    if (debounce == 0) {
        return;
    }

    debounce->stable_state = initial_state;
    debounce->threshold_ticks = threshold_ticks;
    debounce->counter = 0u;
}

bool contest_debounce_update(contest_debounce_t *debounce, bool raw_state)
{
    if (debounce == 0) {
        return raw_state;
    }

    if (raw_state == debounce->stable_state) {
        debounce->counter = 0u;
        return debounce->stable_state;
    }

    debounce->counter += 1u;
    if (debounce->counter >= debounce->threshold_ticks) {
        debounce->stable_state = raw_state;
        debounce->counter = 0u;
    }

    return debounce->stable_state;
}

bool contest_debounce_state(const contest_debounce_t *debounce)
{
    return debounce == 0 ? false : debounce->stable_state;
}
