#include "../include/contest_hal_adc_scan.h"

bool contest_hal_adc_scan_init(contest_hal_adc_scan_t *scan, ADC_HandleTypeDef *hadc, uint32_t timeout_ms)
{
    if (scan == 0 || hadc == 0) {
        return false;
    }

    /* Keep only the HAL handle and timeout here; channel setup still belongs to CubeMX. */
    scan->hadc = hadc;
    scan->timeout_ms = timeout_ms;
    return true;
}

bool contest_hal_adc_scan_read(contest_hal_adc_scan_t *scan, uint16_t *samples_out, size_t count)
{
    size_t i;

    if (scan == 0 || scan->hadc == 0 || samples_out == 0 || count == 0u) {
        return false;
    }

    /* Start one conversion sequence and read each converted channel in order. */
    if (HAL_ADC_Start(scan->hadc) != HAL_OK) {
        return false;
    }

    for (i = 0u; i < count; ++i) {
        if (HAL_ADC_PollForConversion(scan->hadc, scan->timeout_ms) != HAL_OK) {
            (void)HAL_ADC_Stop(scan->hadc);
            return false;
        }

        samples_out[i] = (uint16_t)HAL_ADC_GetValue(scan->hadc);
    }

    return HAL_ADC_Stop(scan->hadc) == HAL_OK;
}
