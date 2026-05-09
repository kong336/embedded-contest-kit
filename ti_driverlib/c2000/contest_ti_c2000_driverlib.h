#ifndef CONTEST_TI_C2000_DRIVERLIB_H
#define CONTEST_TI_C2000_DRIVERLIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "driverlib.h"

static inline void contest_ti_c2000_gpio_make_output(uint32_t pin_number)
{
    GPIO_setDirectionMode(pin_number, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(pin_number, GPIO_PIN_TYPE_STD);
}

static inline void contest_ti_c2000_gpio_write(uint32_t pin_number, bool high_level)
{
    GPIO_writePin(pin_number, high_level ? 1U : 0U);
}

static inline void contest_ti_c2000_gpio_toggle(uint32_t pin_number)
{
    GPIO_togglePin(pin_number);
}

static inline void contest_ti_c2000_sci_write_char(uint32_t sci_base, uint16_t value)
{
    SCI_writeCharBlockingFIFO(sci_base, value);
}

static inline void contest_ti_c2000_sci_write_buffer(uint32_t sci_base, const uint8_t *data, size_t length)
{
    size_t i;

    if (length > 0u && data == 0) {
        return;
    }

    for (i = 0u; i < length; ++i) {
        SCI_writeCharBlockingFIFO(sci_base, data[i]);
    }
}

static inline void contest_ti_c2000_epwm_set_compare_a(uint32_t epwm_base, uint16_t compare_ticks)
{
    EPWM_setCounterCompareValue(epwm_base, EPWM_COUNTER_COMPARE_A, compare_ticks);
}

static inline void contest_ti_c2000_adc_force_soc(uint32_t adc_base, uint16_t soc_number)
{
    ADC_forceSOC(adc_base, soc_number);
}

static inline uint16_t contest_ti_c2000_adc_read_result(uint32_t result_base, uint16_t result_number)
{
    return ADC_readResult(result_base, result_number);
}

#endif
