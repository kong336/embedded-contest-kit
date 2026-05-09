#include "../include/contest_msp430fr2355_launchpad.h"

static void application_print_banner(void)
{
    contest_msp430fr2355_uart_a1_write_text("MSP430FR2355 boot ok\n");
    contest_msp430fr2355_uart_a1_write_text("UART A1 on P4.2/P4.3 is alive\n");
}

int main(void)
{
    contest_msp430fr2355_stop_watchdog();
    contest_msp430fr2355_led_init();
    contest_msp430fr2355_uart_a1_init_9600_smclk_1mhz();
    contest_msp430fr2355_unlock_gpio();

    application_print_banner();

    for (;;) {
        contest_msp430fr2355_led_toggle();
        contest_msp430fr2355_uart_a1_write_text("heartbeat\n");

        /*
         * The LED and serial heartbeat share the same simple timing source
         * so the very first bringup stays easy to inspect.
         */
        contest_msp430fr2355_delay_cycles_approx(1000000u);
    }
}
