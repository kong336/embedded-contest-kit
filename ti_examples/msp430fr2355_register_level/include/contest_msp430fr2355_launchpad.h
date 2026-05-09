#ifndef CONTEST_MSP430FR2355_LAUNCHPAD_H
#define CONTEST_MSP430FR2355_LAUNCHPAD_H

#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * Board assumptions follow the common MSP-EXP430FR2355 LaunchPad wiring:
 * - LED1 is connected to P1.0
 * - the eZ-FET backchannel UART is routed through eUSCI_A1 on P4.2/P4.3
 */
#define CONTEST_MSP430FR2355_LED1_BIT BIT0
#define CONTEST_MSP430FR2355_UART_RX_BIT BIT2
#define CONTEST_MSP430FR2355_UART_TX_BIT BIT3

static inline void contest_msp430fr2355_stop_watchdog(void)
{
    WDTCTL = WDTPW | WDTHOLD;
}

static inline void contest_msp430fr2355_unlock_gpio(void)
{
    /*
     * FRAM devices start with GPIO locked in high-impedance mode.
     * Clearing LOCKLPM5 makes the previously configured pin states take effect.
     */
    PM5CTL0 &= ~LOCKLPM5;
}

static inline void contest_msp430fr2355_led_init(void)
{
    P1OUT &= (uint8_t)~CONTEST_MSP430FR2355_LED1_BIT;
    P1DIR |= CONTEST_MSP430FR2355_LED1_BIT;
}

static inline void contest_msp430fr2355_led_write(bool high_level)
{
    if (high_level) {
        P1OUT |= CONTEST_MSP430FR2355_LED1_BIT;
    } else {
        P1OUT &= (uint8_t)~CONTEST_MSP430FR2355_LED1_BIT;
    }
}

static inline void contest_msp430fr2355_led_toggle(void)
{
    P1OUT ^= CONTEST_MSP430FR2355_LED1_BIT;
}

static inline void contest_msp430fr2355_delay_cycles_approx(uint32_t cycles)
{
    volatile uint32_t remaining = cycles / 4u;

    if (remaining == 0u) {
        remaining = 1u;
    }

    while (remaining > 0u) {
        remaining -= 1u;
    }
}

static inline void contest_msp430fr2355_uart_a1_init_9600_smclk_1mhz(void)
{
    /*
     * Keep the first serial bringup conservative:
     * - use the default 1 MHz SMCLK assumption
     * - choose a plain 9600 baud divider first
     * This is intentionally simple and easy to reason about during first bringup.
     */
    P4SEL0 |= (CONTEST_MSP430FR2355_UART_RX_BIT | CONTEST_MSP430FR2355_UART_TX_BIT);
    P4SEL1 &= (uint8_t)~(CONTEST_MSP430FR2355_UART_RX_BIT | CONTEST_MSP430FR2355_UART_TX_BIT);

    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1BRW = 104u;
    UCA1MCTLW = 0u;
    UCA1CTLW0 &= (uint16_t)~UCSWRST;
}

static inline void contest_msp430fr2355_uart_a1_write_byte(uint8_t value)
{
    while ((UCA1IFG & UCTXIFG) == 0u) {
    }

    UCA1TXBUF = value;
}

static inline void contest_msp430fr2355_uart_a1_write_text(const char *text)
{
    if (text == 0) {
        return;
    }

    while (*text != '\0') {
        if (*text == '\n') {
            contest_msp430fr2355_uart_a1_write_byte('\r');
        }

        contest_msp430fr2355_uart_a1_write_byte((uint8_t)*text);
        text += 1;
    }
}

#endif
