#ifndef CONTEST_SOFT_TIMER_H
#define CONTEST_SOFT_TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t timeout_ticks;
    uint32_t elapsed_ticks;
    bool active;
} contest_soft_timer_t;

void contest_soft_timer_init(contest_soft_timer_t *timer);
void contest_soft_timer_start(contest_soft_timer_t *timer, uint32_t timeout_ticks);
void contest_soft_timer_stop(contest_soft_timer_t *timer);
void contest_soft_timer_tick(contest_soft_timer_t *timer);
bool contest_soft_timer_expired(const contest_soft_timer_t *timer);

#endif
