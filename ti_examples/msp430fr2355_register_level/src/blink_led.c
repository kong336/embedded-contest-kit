#include "../include/contest_msp430fr2355_launchpad.h"

int main(void)
{
    contest_msp430fr2355_stop_watchdog();
    contest_msp430fr2355_led_init();
    contest_msp430fr2355_unlock_gpio();

    for (;;) {
        contest_msp430fr2355_led_toggle();

        /*
         * With the common default 1 MHz clock assumption, one million cycles
         * is roughly one second and keeps the first hardware check obvious.
         */
        contest_msp430fr2355_delay_cycles_approx(1000000u);
    }
}
