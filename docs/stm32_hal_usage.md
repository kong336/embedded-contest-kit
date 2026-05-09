# STM32 HAL Usage Notes

This folder adds an STM32 HAL-facing layer on top of the generic contest kit.

## Why split generic code and HAL code

The generic code is:

- easier to test
- easier to reuse on Arduino, ESP32, or other MCUs
- focused on algorithms and protocol logic

The HAL layer is:

- board-facing
- peripheral-facing
- tied to STM32CubeMX handles and generated setup

Keeping them separate helps a lot in competitions because you can copy only the hardware layer when the board changes.

## Recommended project split

- `generic_c`: math, filtering, protocols, control
- `stm32_hal`: ADC, PWM, UART, encoder timers, buttons, buzzer, status LED, servo, GPIO wrappers
- `application`: your actual contest logic

## Typical flow

### Line following

1. HAL ADC reads grayscale sensor channels
2. generic line sensor code normalizes them
3. generic PID calculates correction
4. HAL PWM motor code updates left and right output

### UART communication

1. HAL UART interrupt receives bytes
2. HAL wrapper pushes bytes into ring buffer
3. generic packet parser extracts commands
4. application handles command content

### Speed loop

1. HAL timer encoder reads wheel pulses
2. generic encoder helper converts pulse delta into RPM
3. generic low-pass filter smooths measured speed
4. generic incremental PI produces PWM correction
5. HAL motor wrapper applies signed output

### Button control

1. HAL GPIO reads raw pin level
2. HAL button wrapper feeds the generic button helper
3. generic state machine changes application mode

### Servo and buzzer control

1. HAL PWM drives servo compare output
2. generic servo map converts desired angle into compare value
3. HAL GPIO buzzer wrapper plays timed success or warning patterns

### Differential-drive and status control

1. generic diff-drive mixer converts forward plus turn commands into left and right outputs
2. HAL motor wrappers apply the final signed PWM commands
3. HAL status LED wrapper exposes simple ready or warning blink patterns

### OLED menu and command control

1. HAL buttons generate clean click events
2. generic menu logic updates cursor and visible rows
3. your OLED driver only handles drawing
4. serial command tables keep packet parsing and command actions separated

### Ultrasonic and balancing control

1. timer capture or board-specific driver measures echo pulse width
2. generic ultrasonic helper converts echo time into distance
3. IMU driver provides gyro plus accelerometer inputs
4. generic complementary filter and control loops handle the reusable math

## What is still board-specific

- exact ADC channel ordering
- PWM timer and channel mapping
- encoder timer mapping and counts-per-revolution
- direction GPIO polarity
- button active level and pull-up or pull-down wiring
- servo timer period and compare range
- buzzer wiring polarity and whether it is active or passive
- status LED active level and pin mapping
- OLED driver implementation and font tables
- ultrasonic trigger and echo capture implementation
- IMU register driver and calibration
- interrupt callback registration
- CubeMX-generated initialization

So this HAL folder is meant to be a strong template, not a magical one-click full firmware.
