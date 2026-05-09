#include "contest_periodic_task.h"

#include <limits.h>

void contest_periodic_task_init(contest_periodic_task_t *task, uint32_t period_ticks)
{
    if (task == 0) {
        return;
    }

    task->period_ticks = period_ticks;
    task->elapsed_ticks = 0u;
    task->enabled = period_ticks > 0u;
    task->due = false;
}

void contest_periodic_task_start(contest_periodic_task_t *task, uint32_t period_ticks)
{
    if (task == 0) {
        return;
    }

    task->period_ticks = period_ticks;
    task->elapsed_ticks = 0u;
    task->enabled = period_ticks > 0u;
    task->due = false;
}

void contest_periodic_task_stop(contest_periodic_task_t *task)
{
    if (task == 0) {
        return;
    }

    task->enabled = false;
    task->elapsed_ticks = 0u;
    task->due = false;
}

void contest_periodic_task_reset(contest_periodic_task_t *task)
{
    if (task == 0) {
        return;
    }

    task->elapsed_ticks = 0u;
    task->due = false;
}

void contest_periodic_task_tick(contest_periodic_task_t *task)
{
    if (task == 0 || !task->enabled || task->period_ticks == 0u) {
        return;
    }

    if (task->elapsed_ticks < UINT32_MAX) {
        task->elapsed_ticks += 1u;
    }

    if (task->elapsed_ticks >= task->period_ticks) {
        task->elapsed_ticks = 0u;
        task->due = true;
    }
}

bool contest_periodic_task_consume(contest_periodic_task_t *task)
{
    bool due;

    if (task == 0) {
        return false;
    }

    due = task->due;
    task->due = false;
    return due;
}
