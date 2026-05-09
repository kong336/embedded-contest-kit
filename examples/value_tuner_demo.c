#include <stdbool.h>

#include "../generic_c/include/contest_value_tuner.h"

static contest_value_tuner_t g_kp_tuner;

void app_value_tuner_init(void)
{
    contest_value_tuner_init(&g_kp_tuner, "kp", 20.0f, 0.0f, 80.0f, 1.0f, 0.1f);
}

float app_value_tuner_on_key(int direction, bool fine_mode)
{
    /*
     * Typical usage:
     * - left or right key sets direction
     * - another key toggles fine or coarse mode
     */
    return contest_value_tuner_adjust(&g_kp_tuner, direction, fine_mode);
}
