#ifndef CONTEST_HAL_ADC_SCAN_H
#define CONTEST_HAL_ADC_SCAN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "contest_stm32_hal.h"

typedef struct {
    ADC_HandleTypeDef *hadc;
    uint32_t timeout_ms;
} contest_hal_adc_scan_t;

bool contest_hal_adc_scan_init(contest_hal_adc_scan_t *scan, ADC_HandleTypeDef *hadc, uint32_t timeout_ms);
bool contest_hal_adc_scan_read(contest_hal_adc_scan_t *scan, uint16_t *samples_out, size_t count);

#endif
