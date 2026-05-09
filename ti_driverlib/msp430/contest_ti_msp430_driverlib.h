#ifndef CONTEST_TI_MSP430_DRIVERLIB_H
#define CONTEST_TI_MSP430_DRIVERLIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/devices/msp430/driverlib/driverlib.h>

typedef struct {
    uint8_t port;
    uint16_t pin_mask;
} contest_ti_msp430_gpio_t;

static inline void contest_ti_msp430_gpio_make_output(const contest_ti_msp430_gpio_t *pin)
{
    if (pin == 0) {
        return;
    }

    GPIO_setAsOutputPin(pin->port, pin->pin_mask);
}

static inline void contest_ti_msp430_gpio_write(const contest_ti_msp430_gpio_t *pin, bool high_level)
{
    if (pin == 0) {
        return;
    }

    if (high_level) {
        GPIO_setOutputHighOnPin(pin->port, pin->pin_mask);
    } else {
        GPIO_setOutputLowOnPin(pin->port, pin->pin_mask);
    }
}

static inline void contest_ti_msp430_gpio_toggle(const contest_ti_msp430_gpio_t *pin)
{
    if (pin == 0) {
        return;
    }

    GPIO_toggleOutputOnPin(pin->port, pin->pin_mask);
}

static inline void contest_ti_msp430_uart_write_byte(uint16_t uart_base, uint8_t value)
{
    EUSCI_A_UART_transmitData(uart_base, value);
}

static inline void contest_ti_msp430_uart_write_buffer(uint16_t uart_base, const uint8_t *data, size_t length)
{
    size_t i;

    if (length > 0u && data == 0) {
        return;
    }

    for (i = 0u; i < length; ++i) {
        EUSCI_A_UART_transmitData(uart_base, data[i]);
    }
}

static inline void contest_ti_msp430_pwm_set_compare(
    uint16_t timer_base,
    uint_fast16_t compare_register,
    uint16_t compare_ticks
)
{
    Timer_A_setCompareValue(timer_base, compare_register, compare_ticks);
}

static inline uint16_t contest_ti_msp430_adc_read_single_blocking(uint16_t adc_base)
{
    ADC_startConversion(adc_base, ADC_SINGLECHANNEL);
    while (ADC_isBusy(adc_base)) {
    }

    return ADC_getResults(adc_base);
}

#endif
