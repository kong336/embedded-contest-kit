#ifndef CONTEST_TI_MSPM0_DRIVERLIB_H
#define CONTEST_TI_MSPM0_DRIVERLIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/driverlib/dl_adc12.h>
#include <ti/driverlib/dl_gpio.h>
#include <ti/driverlib/dl_timera.h>
#include <ti/driverlib/dl_uart_main.h>

typedef struct {
    GPIO_Regs *gpio;
    uint32_t pins;
} contest_ti_mspm0_gpio_t;

static inline void contest_ti_mspm0_gpio_make_output(const contest_ti_mspm0_gpio_t *pin)
{
    if (pin == 0 || pin->gpio == 0) {
        return;
    }

    /* Keep pinmux and clock setup in SysConfig or the board init code. */
    DL_GPIO_enableOutput(pin->gpio, pin->pins);
}

static inline void contest_ti_mspm0_gpio_write(const contest_ti_mspm0_gpio_t *pin, bool high_level)
{
    if (pin == 0 || pin->gpio == 0) {
        return;
    }

    if (high_level) {
        DL_GPIO_setPins(pin->gpio, pin->pins);
    } else {
        DL_GPIO_clearPins(pin->gpio, pin->pins);
    }
}

static inline void contest_ti_mspm0_gpio_toggle(const contest_ti_mspm0_gpio_t *pin)
{
    if (pin == 0 || pin->gpio == 0) {
        return;
    }

    DL_GPIO_togglePins(pin->gpio, pin->pins);
}

static inline void contest_ti_mspm0_uart_write_byte(UART_Regs *uart, uint8_t value)
{
    if (uart == 0) {
        return;
    }

    DL_UART_Main_transmitDataBlocking(uart, value);
}

static inline void contest_ti_mspm0_uart_write_buffer(UART_Regs *uart, const uint8_t *data, size_t length)
{
    size_t i;

    if (uart == 0 || (length > 0u && data == 0)) {
        return;
    }

    for (i = 0u; i < length; ++i) {
        DL_UART_Main_transmitDataBlocking(uart, data[i]);
    }
}

static inline void contest_ti_mspm0_pwm_set_compare(
    GPTIMER_Regs *timer,
    DL_TIMER_CC_INDEX cc_index,
    uint32_t compare_ticks
)
{
    if (timer == 0) {
        return;
    }

    DL_Timer_setCaptureCompareValue(timer, compare_ticks, cc_index);
}

static inline uint16_t contest_ti_mspm0_adc_read_mem0_blocking(ADC12_Regs *adc)
{
    if (adc == 0) {
        return 0u;
    }

    DL_ADC12_startConversion(adc);
    while (DL_ADC12_getRawInterruptStatus(adc, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED) == 0u) {
    }

    DL_ADC12_clearInterruptStatus(adc, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);
    return DL_ADC12_getMemResult(adc, DL_ADC12_MEM_IDX_0);
}

#endif
