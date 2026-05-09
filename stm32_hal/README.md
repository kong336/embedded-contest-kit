# STM32 HAL Layer

This folder adapts the generic contest kit to STM32 HAL-style projects.

## What is included

- `include/contest_stm32_hal.h`: configurable HAL include bridge
- `include/contest_hal_adc_scan.h`: sequential ADC scan helper
- `include/contest_hal_motor.h`: PWM plus direction helper
- `include/contest_hal_uart_packet.h`: UART RX buffer plus packet parser helper
- `include/contest_hal_button.h`: GPIO button polling plus event helper
- `include/contest_hal_encoder_timer.h`: timer encoder sampling and speed conversion helper
- `include/contest_hal_buzzer.h`: active buzzer GPIO plus beep-pattern helper
- `include/contest_hal_diff_drive.h`: dual-motor differential drive helper
- `include/contest_hal_servo.h`: PWM servo helper with angle mapping
- `include/contest_hal_status_led.h`: GPIO status LED plus blink-pattern helper
- `examples/`: STM32 HAL-flavored example files

## Extra example templates

- `examples/stm32_menu_oled_hal.c`: button-driven menu skeleton for SSD1306 or SH1106 style displays
- `examples/stm32_ultrasonic_hal.c`: HC-SR04 style post-processing template after echo capture
- `examples/stm32_balance_car_hal.c`: balance-car control structure example
- `examples/stm32_diff_drive_hal.c`: forward plus turn drive-mixing example
- `examples/stm32_param_menu_hal.c`: OLED parameter tuning page structure example
- `examples/stm32_status_led_hal.c`: ready or warning blink-pattern example

## Design goal

The goal here is not to replace STM32CubeMX. The goal is to give you a clean reusable layer after CubeMX has already created handles such as:

- `ADC_HandleTypeDef`
- `UART_HandleTypeDef`
- `TIM_HandleTypeDef`

## Expected workflow

1. generate base project in CubeMX
2. copy the files from this folder
3. fill in your actual handles and channels
4. keep algorithm code in `generic_c`
5. keep board binding in `stm32_hal`
