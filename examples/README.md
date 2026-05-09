# Examples

These examples are intentionally incomplete at the hardware layer. They show how the reusable modules fit together without assuming a specific MCU family.

- `line_following_demo.c`: line-sensor normalization, PID, and ramp-limited motor target flow
- `uart_packet_demo.c`: ring buffer plus framed packet parsing for UART-style links
- `speed_loop_demo.c`: encoder speed estimation, low-pass filtering, ramped target, and incremental PI output
- `button_menu_demo.c`: button click or long-press event flow into a simple state machine
- `periodic_task_demo.c`: superloop periodic job scheduling with 1 ms, 10 ms, and 100 ms style tasks
- `imu_angle_demo.c`: complementary angle fusion from gyro rate plus accelerometer angle
- `buzzer_pattern_demo.c`: timed beep pattern state logic
- `servo_scan_demo.c`: servo angle smoothing and compare-value mapping
- `command_table_demo.c`: command ID dispatch table for upper-computer or board-to-board control
- `menu_oled_demo.c`: generic menu state for a small OLED UI
- `ultrasonic_demo.c`: HC-SR04 style echo-time to distance conversion with filtering
- `balance_car_demo.c`: balance-car style angle fusion plus speed and angle loop skeleton
