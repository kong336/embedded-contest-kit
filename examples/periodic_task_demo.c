#include <stdbool.h>
#include <stdint.h>

#include "../generic_c/include/contest_periodic_task.h"

static contest_periodic_task_t g_fast_task;
static contest_periodic_task_t g_control_task;
static contest_periodic_task_t g_status_task;

void app_periodic_task_init(void)
{
    /* Assume the outer tick function is called every 1 ms. */
    contest_periodic_task_init(&g_fast_task, 1u);
    contest_periodic_task_init(&g_control_task, 10u);
    contest_periodic_task_init(&g_status_task, 100u);
}

void app_periodic_task_tick(void)
{
    contest_periodic_task_tick(&g_fast_task);
    contest_periodic_task_tick(&g_control_task);
    contest_periodic_task_tick(&g_status_task);
}

bool app_periodic_task_should_run_fast(void)
{
    return contest_periodic_task_consume(&g_fast_task);
}

bool app_periodic_task_should_run_control(void)
{
    return contest_periodic_task_consume(&g_control_task);
}

bool app_periodic_task_should_run_status(void)
{
    return contest_periodic_task_consume(&g_status_task);
}
