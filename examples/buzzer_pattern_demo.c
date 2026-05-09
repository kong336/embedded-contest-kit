#include <stdbool.h>

#include "../generic_c/include/contest_buzzer_pattern.h"

static contest_buzzer_pattern_t g_buzzer_pattern;

void app_buzzer_pattern_init(void)
{
    contest_buzzer_pattern_init(&g_buzzer_pattern);
}

void app_buzzer_pattern_start_success(void)
{
    /* Two short beeps often work well as a "done" or "armed" sound. */
    contest_buzzer_pattern_start(&g_buzzer_pattern, 30u, 20u, 2u);
}

bool app_buzzer_pattern_tick(void)
{
    return contest_buzzer_pattern_tick(&g_buzzer_pattern);
}
