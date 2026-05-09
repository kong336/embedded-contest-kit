#ifndef CONTEST_PERIODIC_TASK_H
#define CONTEST_PERIODIC_TASK_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t period_ticks;
    uint32_t elapsed_ticks;
    bool enabled;
    bool due;
} contest_periodic_task_t;

void contest_periodic_task_init(contest_periodic_task_t *task, uint32_t period_ticks);
void contest_periodic_task_start(contest_periodic_task_t *task, uint32_t period_ticks);
void contest_periodic_task_stop(contest_periodic_task_t *task);
void contest_periodic_task_reset(contest_periodic_task_t *task);
void contest_periodic_task_tick(contest_periodic_task_t *task);
bool contest_periodic_task_consume(contest_periodic_task_t *task);

#endif
