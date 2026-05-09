# Contest Project Recipes

This page tells you which modules to copy first for common contest-style projects.

## Line-following car

### Portable modules

- `contest_line_sensor`
- `contest_moving_average`
- `contest_pid`
- `contest_ramp`

### STM32 HAL modules

- `contest_hal_adc_scan`
- `contest_hal_motor`

### Example entry points

- `examples/line_following_demo.c`
- `stm32_hal/examples/stm32_line_following_hal.c`

## Encoder speed loop car

### Portable modules

- `contest_encoder`
- `contest_low_pass`
- `contest_incremental_pi`
- `contest_ramp`
- `contest_periodic_task`

### STM32 HAL modules

- `contest_hal_encoder_timer`
- `contest_hal_motor`

### Example entry points

- `examples/speed_loop_demo.c`
- `stm32_hal/examples/stm32_speed_loop_hal.c`

## Differential-drive car

### Portable modules

- `contest_diff_drive`
- `contest_ramp`

### STM32 HAL modules

- `contest_hal_diff_drive`
- `contest_hal_motor`

### Example entry points

- `examples/diff_drive_demo.c`
- `stm32_hal/examples/stm32_diff_drive_hal.c`

## Button plus menu or mode switching

### Portable modules

- `contest_button`
- `contest_state_machine`
- `contest_periodic_task`

### STM32 HAL modules

- `contest_hal_button`
- `contest_hal_buzzer`

### Example entry points

- `examples/button_menu_demo.c`
- `stm32_hal/examples/stm32_button_hal.c`
- `stm32_hal/examples/stm32_buzzer_hal.c`

## Parameter tuning UI

### Portable modules

- `contest_menu`
- `contest_value_tuner`
- `contest_button`

### STM32 HAL modules

- `contest_hal_button`

### Example entry points

- `examples/value_tuner_demo.c`
- `stm32_hal/examples/stm32_param_menu_hal.c`

## IMU posture or balancing-car starter

### Portable modules

- `contest_complementary_angle`
- `contest_pid`
- `contest_low_pass`
- `contest_periodic_task`

### What you still need

- your actual MPU6050 or other IMU driver
- a fixed sample interrupt or fixed-tick loop

### Example entry point

- `examples/imu_angle_demo.c`

## Balance-car starter

### Portable modules

- `contest_complementary_angle`
- `contest_pid`
- `contest_incremental_pi`
- `contest_periodic_task`

### STM32 HAL modules

- `contest_hal_encoder_timer`
- `contest_hal_motor`

### What you still need

- your real IMU driver
- fall-detection and motor cutoff logic
- a fixed control tick

### Example entry points

- `examples/balance_car_demo.c`
- `stm32_hal/examples/stm32_balance_car_hal.c`

## Servo scan or camera pan

### Portable modules

- `contest_servo_map`
- `contest_ramp`

### STM32 HAL modules

- `contest_hal_servo`

### Example entry points

- `examples/servo_scan_demo.c`
- `stm32_hal/examples/stm32_servo_hal.c`

## UART command control

### Portable modules

- `contest_ring_buffer`
- `contest_packet`
- `contest_crc16`

### STM32 HAL modules

- `contest_hal_uart_packet`

### Example entry points

- `examples/uart_packet_demo.c`
- `stm32_hal/examples/stm32_uart_packet_hal.c`

## Upper-computer command table

### Portable modules

- `contest_command_table`
- `contest_packet`
- `contest_ring_buffer`

### Example entry point

- `examples/command_table_demo.c`

## OLED menu

### Portable modules

- `contest_menu`
- `contest_button`
- `contest_state_machine`

### STM32 HAL examples

- `stm32_hal/examples/stm32_menu_oled_hal.c`

### Example entry point

- `examples/menu_oled_demo.c`

## Ultrasonic distance detection

### Portable modules

- `contest_ultrasonic`
- `contest_low_pass`

### STM32 HAL example

- `stm32_hal/examples/stm32_ultrasonic_hal.c`

### Example entry point

- `examples/ultrasonic_demo.c`

## Status LED feedback

### Portable modules

- `contest_led_pattern`

### STM32 HAL modules

- `contest_hal_status_led`

### Example entry points

- `examples/led_pattern_demo.c`
- `stm32_hal/examples/stm32_status_led_hal.c`
