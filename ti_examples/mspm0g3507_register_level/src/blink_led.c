#include "contest_mspm0g3507_launchpad.h"

int main(void)
{
    bool led_is_on = false;

    /*
     * Start with the LaunchPad RGB LED because it proves three important
     * pieces at once: GPIO power gating, PINCM routing, and basic DOUT writes.
     */
    contest_mspm0g3507_rgb_led_init();

    while (1) {
        led_is_on = !led_is_on;
        contest_mspm0g3507_rgb_led_write(false, led_is_on, false);
        contest_mspm0g3507_delay_cycles_approx(1200000u);
    }
}
