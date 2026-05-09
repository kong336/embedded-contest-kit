#ifndef CONTEST_STATE_MACHINE_H
#define CONTEST_STATE_MACHINE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t current_state;
    uint8_t previous_state;
    uint32_t state_ticks;
    bool entered;
} contest_state_machine_t;

void contest_state_machine_init(contest_state_machine_t *machine, uint8_t initial_state);
bool contest_state_machine_transition(contest_state_machine_t *machine, uint8_t new_state);
void contest_state_machine_tick(contest_state_machine_t *machine);
bool contest_state_machine_entered(contest_state_machine_t *machine);
uint32_t contest_state_machine_elapsed(const contest_state_machine_t *machine);

#endif
