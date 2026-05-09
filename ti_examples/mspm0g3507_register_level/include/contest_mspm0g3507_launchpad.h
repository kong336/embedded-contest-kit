#ifndef CONTEST_MSPM0G3507_LAUNCHPAD_H
#define CONTEST_MSPM0G3507_LAUNCHPAD_H

#include <stdbool.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>

/*
 * Board assumptions follow the local LP-MSPM0G3507 LaunchPad metadata:
 * - UART0 backchannel uses PA10 (TX) and PA11 (RX)
 * - RGB LED is on PB26/PB22/PB13
 * - PA0 is LED1, but we avoid it because it collides with a UART0 alternate
 */
#define CONTEST_MSPM0G3507_UART0_TX_PINCM_INDEX 21u
#define CONTEST_MSPM0G3507_UART0_RX_PINCM_INDEX 22u
#define CONTEST_MSPM0G3507_RGB_LED_RED_PINCM_INDEX 57u
#define CONTEST_MSPM0G3507_RGB_LED_GREEN_PINCM_INDEX 50u
#define CONTEST_MSPM0G3507_RGB_LED_BLUE_PINCM_INDEX 30u

#define CONTEST_MSPM0G3507_UART0_TX_BIT 10u
#define CONTEST_MSPM0G3507_UART0_RX_BIT 11u
#define CONTEST_MSPM0G3507_RGB_LED_RED_BIT 26u
#define CONTEST_MSPM0G3507_RGB_LED_GREEN_BIT 22u
#define CONTEST_MSPM0G3507_RGB_LED_BLUE_BIT 13u

#define CONTEST_MSPM0G3507_PINMUX_GPIO 1u
#define CONTEST_MSPM0G3507_PINMUX_UART0 2u

#define CONTEST_MSPM0G3507_UART0_BAUD_115200_IBRD 17u
#define CONTEST_MSPM0G3507_UART0_BAUD_115200_FBRD 23u

static inline void contest_mspm0g3507_delay_cycles_approx(uint32_t cycles)
{
    volatile uint32_t remaining = cycles / 4u;

    if (remaining == 0u) {
        remaining = 1u;
    }

    while (remaining > 0u) {
        remaining -= 1u;
    }
}

static inline void contest_mspm0g3507_configure_pincm(
    uint32_t pincm_index,
    uint32_t function_mode,
    bool input_enable)
{
    uint32_t pincm_value = (function_mode & CONTEST_MSPM0_IOMUX_PF_MASK) |
                           CONTEST_MSPM0_IOMUX_PC;

    if (input_enable) {
        pincm_value |= CONTEST_MSPM0_IOMUX_INENA;
    }

    contest_mspm0_write32(
        CONTEST_MSPM0_IOMUX_PINCM(pincm_index),
        pincm_value);
}

static inline void contest_mspm0g3507_gpio_enable(uint32_t gpio_base)
{
    contest_mspm0_write32(
        gpio_base + CONTEST_MSPM0_GPRCM_PWREN_OFFSET,
        CONTEST_MSPM0_GPRCM_ENABLE);

    /*
     * The targetdb XML confirms the GPIO modules sit behind the GPRCM power
     * gate. A tiny delay keeps the first write sequence conservative.
     */
    contest_mspm0g3507_delay_cycles_approx(128u);
}

static inline void contest_mspm0g3507_gpio_make_output(uint32_t gpio_base, uint32_t pin)
{
    contest_mspm0_write32(
        gpio_base + CONTEST_MSPM0_GPIO_DOESET31_0_OFFSET,
        (uint32_t)1u << pin);
}

static inline void contest_mspm0g3507_gpio_write(uint32_t gpio_base, uint32_t pin, bool high_level)
{
    uint32_t register_offset = high_level
        ? CONTEST_MSPM0_GPIO_DOUTSET31_0_OFFSET
        : CONTEST_MSPM0_GPIO_DOUTCLR31_0_OFFSET;

    contest_mspm0_write32(gpio_base + register_offset, (uint32_t)1u << pin);
}

static inline void contest_mspm0g3507_rgb_led_init(void)
{
    contest_mspm0g3507_gpio_enable(CONTEST_MSPM0_GPIOB_BASE);

    contest_mspm0g3507_configure_pincm(
        CONTEST_MSPM0G3507_RGB_LED_RED_PINCM_INDEX,
        CONTEST_MSPM0G3507_PINMUX_GPIO,
        false);
    contest_mspm0g3507_configure_pincm(
        CONTEST_MSPM0G3507_RGB_LED_GREEN_PINCM_INDEX,
        CONTEST_MSPM0G3507_PINMUX_GPIO,
        false);
    contest_mspm0g3507_configure_pincm(
        CONTEST_MSPM0G3507_RGB_LED_BLUE_PINCM_INDEX,
        CONTEST_MSPM0G3507_PINMUX_GPIO,
        false);

    contest_mspm0g3507_gpio_make_output(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_RED_BIT);
    contest_mspm0g3507_gpio_make_output(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_GREEN_BIT);
    contest_mspm0g3507_gpio_make_output(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_BLUE_BIT);

    contest_mspm0g3507_gpio_write(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_RED_BIT,
        false);
    contest_mspm0g3507_gpio_write(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_GREEN_BIT,
        false);
    contest_mspm0g3507_gpio_write(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_BLUE_BIT,
        false);
}

static inline void contest_mspm0g3507_rgb_led_write(bool red_on, bool green_on, bool blue_on)
{
    contest_mspm0g3507_gpio_write(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_RED_BIT,
        red_on);
    contest_mspm0g3507_gpio_write(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_GREEN_BIT,
        green_on);
    contest_mspm0g3507_gpio_write(
        CONTEST_MSPM0_GPIOB_BASE,
        CONTEST_MSPM0G3507_RGB_LED_BLUE_BIT,
        blue_on);
}

static inline void contest_mspm0g3507_uart0_backchannel_init_115200(void)
{
    contest_mspm0g3507_configure_pincm(
        CONTEST_MSPM0G3507_UART0_TX_PINCM_INDEX,
        CONTEST_MSPM0G3507_PINMUX_UART0,
        false);
    contest_mspm0g3507_configure_pincm(
        CONTEST_MSPM0G3507_UART0_RX_PINCM_INDEX,
        CONTEST_MSPM0G3507_PINMUX_UART0,
        true);

    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_GPRCM_PWREN_OFFSET,
        CONTEST_MSPM0_GPRCM_ENABLE);
    contest_mspm0g3507_delay_cycles_approx(256u);

    /*
     * Keep the first UART bringup intentionally plain:
     * - use BUSCLK directly
     * - keep divide-by-1
     * - use 8N1 without FIFO
     */
    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_CLKSEL_OFFSET,
        CONTEST_MSPM0_UART_CLKSEL_BUSCLK_SEL);
    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_CLKDIV_OFFSET,
        0u);
    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_CTL0_OFFSET,
        0u);
    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_IBRD_OFFSET,
        CONTEST_MSPM0G3507_UART0_BAUD_115200_IBRD);
    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_FBRD_OFFSET,
        CONTEST_MSPM0G3507_UART0_BAUD_115200_FBRD);
    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_LCRH_OFFSET,
        CONTEST_MSPM0_UART_LCRH_WLEN_8);
    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_CTL0_OFFSET,
        CONTEST_MSPM0_UART_CTL0_ENABLE |
        CONTEST_MSPM0_UART_CTL0_RXE |
        CONTEST_MSPM0_UART_CTL0_TXE);
}

static inline void contest_mspm0g3507_uart0_write_byte(uint8_t value)
{
    while ((contest_mspm0_read32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_STAT_OFFSET) &
        CONTEST_MSPM0_UART_STAT_TXFF) != 0u) {
    }

    contest_mspm0_write32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_TXDATA_OFFSET,
        (uint32_t)value);
}

static inline void contest_mspm0g3507_uart0_flush(void)
{
    while ((contest_mspm0_read32(
        CONTEST_MSPM0_UART0_BASE + CONTEST_MSPM0_UART_STAT_OFFSET) &
        CONTEST_MSPM0_UART_STAT_BUSY) != 0u) {
    }
}

static inline void contest_mspm0g3507_uart0_write_text(const char *text)
{
    if (text == 0) {
        return;
    }

    while (*text != '\0') {
        if (*text == '\n') {
            contest_mspm0g3507_uart0_write_byte('\r');
        }

        contest_mspm0g3507_uart0_write_byte((uint8_t)*text);
        text += 1;
    }

    contest_mspm0g3507_uart0_flush();
}

#endif
