#include "contest_state_machine.h"

#include <limits.h>

void contest_state_machine_init(contest_state_machine_t *machine, uint8_t initial_state)
{
    if (machine == 0) {
        return;
    }

    machine->current_state = initial_state;
    machine->previous_state = initial_state;
    machine->state_ticks = 0u;
    machine->entered = true;
}

bool contest_state_machine_transition(contest_state_machine_t *machine, uint8_t new_state)
{
    if (machine == 0) {
        return false;
    }

    if (machine->current_state == new_state) {
        return false;
    }

    machine->previous_state = machine->current_state;
    machine->current_state = new_state;
    machine->state_ticks = 0u;
    machine->entered = true;
    return true;
}

void contest_state_machine_tick(contest_state_machine_t *machine)
{
    if (machine == 0) {
        return;
    }

    if (machine->state_ticks < UINT32_MAX) {
        machine->state_ticks += 1u;
    }
}

bool contest_state_machine_entered(contest_state_machine_t *machine)
{
    bool entered;

    if (machine == 0) {
        return false;
    }

    /* Read-once entry flag keeps one-time state setup code simple in the main loop. */
    entered = machine->entered;
    machine->entered = false;
    return entered;
}

uint32_t contest_state_machine_elapsed(const contest_state_machine_t *machine)
{
    if (machine == 0) {
        return 0u;
    }

    return machine->state_ticks;
}
