#include <stdbool.h>

#include "../generic_c/include/contest_led_pattern.h"

static contest_led_pattern_t g_status_led;

void app_led_pattern_init(void)
{
    contest_led_pattern_init(&g_status_led);
}

void app_led_pattern_start_warning(void)
{
    /* Three short blinks can serve as an attention or warning pattern. */
    contest_led_pattern_start(&g_status_led, 20u, 20u, 3u);
}

bool app_led_pattern_tick(void)
{
    return contest_led_pattern_tick(&g_status_led);
}
