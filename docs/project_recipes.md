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
