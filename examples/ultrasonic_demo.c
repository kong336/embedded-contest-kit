#include <stdbool.h>

#include "../generic_c/include/contest_low_pass.h"
#include "../generic_c/include/contest_ultrasonic.h"

static contest_ultrasonic_t g_ultrasonic;
static contest_low_pass_t g_distance_filter;

void app_ultrasonic_demo_init(void)
{
    contest_ultrasonic_init(&g_ultrasonic, 343.0f, 2.0f, 400.0f);
    contest_low_pass_init(&g_distance_filter, 0.30f, 0.0f);
}

bool app_ultrasonic_demo_step(float echo_width_us, float *filtered_distance_cm_out)
{
    float distance_cm;

    if (filtered_distance_cm_out == 0) {
        return false;
    }

    if (!contest_ultrasonic_measure_cm(&g_ultrasonic, echo_width_us, &distance_cm)) {
        return false;
    }

    *filtered_distance_cm_out = contest_low_pass_update(&g_distance_filter, distance_cm);
    return true;
}
