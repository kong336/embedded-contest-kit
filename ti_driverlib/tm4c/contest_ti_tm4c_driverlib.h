#ifndef CONTEST_TI_TM4C_DRIVERLIB_H
#define CONTEST_TI_TM4C_DRIVERLIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

static inline void contest_ti_tm4c_enable_peripheral(uint32_t peripheral_id)
{
    SysCtlPeripheralEnable(peripheral_id);
    while (!SysCtlPeripheralReady(peripheral_id)) {
    }
}

static inline void contest_ti_tm4c_gpio_make_output(uint32_t gpio_base, uint8_t pin_mask)
{
    GPIOPinTypeGPIOOutput(gpio_base, pin_mask);
}

static inline void contest_ti_tm4c_gpio_write(uint32_t gpio_base, uint8_t pin_mask, bool high_level)
{
    GPIOPinWrite(gpio_base, pin_mask, high_level ? pin_mask : 0u);
}

static inline void contest_ti_tm4c_gpio_toggle(uint32_t gpio_base, uint8_t pin_mask)
{
    uint8_t current_state;

    current_state = (uint8_t)GPIOPinRead(gpio_base, pin_mask);
    GPIOPinWrite(gpio_base, pin_mask, current_state == 0u ? pin_mask : 0u);
}

static inline void contest_ti_tm4c_uart_write_char(uint32_t uart_base, uint8_t value)
{
    UARTCharPut(uart_base, value);
}

static inline void contest_ti_tm4c_uart_write_buffer(uint32_t uart_base, const uint8_t *data, size_t length)
{
    size_t i;

    if (length > 0u && data == 0) {
        return;
    }

    for (i = 0u; i < length; ++i) {
        UARTCharPut(uart_base, data[i]);
    }
}

static inline void contest_ti_tm4c_pwm_set_pulse_width(
    uint32_t pwm_base,
    uint32_t pwm_output,
    uint32_t pulse_width_ticks
)
{
    PWMPulseWidthSet(pwm_base, pwm_output, pulse_width_ticks);
}

static inline uint32_t contest_ti_tm4c_adc_read_sequence3_blocking(uint32_t adc_base)
{
    uint32_t value = 0u;

    ADCProcessorTrigger(adc_base, 3u);
    while (!ADCIntStatus(adc_base, 3u, false)) {
    }

    ADCIntClear(adc_base, 3u);
    ADCSequenceDataGet(adc_base, 3u, &value);
    return value;
}

#endif
