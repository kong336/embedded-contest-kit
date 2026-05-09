#include "contest_mspm0g3507_launchpad.h"

int main(void)
{
    uint32_t heartbeat_count = 0u;

    /*
     * Keep the first serial example intentionally small:
     * - bring up UART0 on the LaunchPad backchannel pins
     * - print a banner
     * - pulse the green LED with a text heartbeat
     */
    contest_mspm0g3507_rgb_led_init();
    contest_mspm0g3507_uart0_backchannel_init_115200();

    contest_mspm0g3507_uart0_write_text(
        "MSPM0G3507 register-level UART ready.\n"
        "Backchannel UART0 is routed on PA10/PA11 at 115200 8N1.\n");

    while (1) {
        contest_mspm0g3507_rgb_led_write(false, true, false);
        contest_mspm0g3507_uart0_write_text("heartbeat ");
        contest_mspm0g3507_uart0_write_byte((uint8_t)('0' + (heartbeat_count % 10u)));
        contest_mspm0g3507_uart0_write_text("\n");

        heartbeat_count += 1u;
        contest_mspm0g3507_delay_cycles_approx(1200000u);

        contest_mspm0g3507_rgb_led_write(false, false, false);
        contest_mspm0g3507_delay_cycles_approx(1200000u);
    }
}
