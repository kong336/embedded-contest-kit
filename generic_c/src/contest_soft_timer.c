#include "contest_soft_timer.h"

void contest_soft_timer_init(contest_soft_timer_t *timer)
{
    if (timer == 0) {
        return;
    }

    timer->timeout_ticks = 0u;
    timer->elapsed_ticks = 0u;
    timer->active = false;
}

void contest_soft_timer_start(contest_soft_timer_t *timer, uint32_t timeout_ticks)
{
    if (timer == 0) {
        return;
    }

    timer->timeout_ticks = timeout_ticks;
    timer->elapsed_ticks = 0u;
    timer->active = true;
}

void contest_soft_timer_stop(contest_soft_timer_t *timer)
{
    if (timer == 0) {
        return;
    }

    timer->active = false;
    timer->elapsed_ticks = 0u;
}

void contest_soft_timer_tick(contest_soft_timer_t *timer)
{
    if (timer == 0 || !timer->active) {
        return;
    }

    /* Saturating tick count avoids wrapping while the timer is active. */
    if (timer->elapsed_ticks < timer->timeout_ticks) {
        timer->elapsed_ticks += 1u;
    }
}

bool contest_soft_timer_expired(const contest_soft_timer_t *timer)
{
    if (timer == 0 || !timer->active) {
        return false;
    }

    return timer->elapsed_ticks >= timer->timeout_ticks;
}
