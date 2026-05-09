#ifndef CONTEST_STM32_HAL_H
#define CONTEST_STM32_HAL_H

/*
 * Override this macro from your build system if your project uses a different
 * HAL device header, for example:
 * - "stm32f1xx_hal.h"
 * - "stm32f4xx_hal.h"
 * - "stm32g4xx_hal.h"
 */
#ifndef CONTEST_STM32_HAL_HEADER
#define CONTEST_STM32_HAL_HEADER "stm32f1xx_hal.h"
#endif

#include CONTEST_STM32_HAL_HEADER

#endif
