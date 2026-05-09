# Contest Kit Principles

This page explains the core idea behind each reusable module in the contest kit.

## contest_pid

### Principle

PID uses:

- proportional term to react to current error
- integral term to remove steady-state bias
- derivative term to suppress overly fast change

### When to use

- speed control
- angle correction
- line-following steering correction

### Common mistakes

- too much `ki`, causing slow oscillation and windup
- too much `kd`, causing jitter from noisy sensors
- forgetting output limits and integral limits

## contest_moving_average

### Principle

Take the average of the last N samples to smooth high-frequency noise.

### When to use

- ADC smoothing
- line sensor error smoothing
- simple speed estimate smoothing

### Tradeoff

- more stable
- but slower response

## contest_low_pass

### Principle

Use a first-order low-pass equation so the output only moves part of the way toward the new sample each update.

### When to use

- speed estimate smoothing
- analog sensor smoothing
- target command softening before a control loop

### Tradeoff

- simpler and lighter than a long moving average
- but the response depends on tuning `alpha`

## contest_complementary_angle

### Principle

Use the gyro for fast short-term change and the accelerometer for slow long-term correction, then blend them together.

### When to use

- balancing car angle estimate
- tilt estimation
- IMU-based posture correction

## contest_median_filter

### Principle

Sort a small sample window and pick the median. This removes isolated spikes better than moving average.

### When to use

- ultrasonic distance spikes
- occasional bad ADC samples
- noisy communication-derived values

## contest_debounce

### Principle

Mechanical keys bounce for a short time. Debounce waits until the signal stays consistent for enough ticks.

### When to use

- buttons
- limit switches
- mode toggle inputs

## contest_button

### Principle

Build a higher-level event layer on top of a debounced signal, so one raw key can become press, release, click, and long-press events.

### When to use

- menu switching
- mode selection
- one-button UI on simple MCU boards

## contest_buzzer_pattern

### Principle

Model buzzer feedback as a timed on/off sequence so your application logic only says "play success sound" instead of manually flipping pins.

### When to use

- start prompt
- error warning
- mode-change feedback

## contest_menu

### Principle

Keep menu cursor movement, visible-window scrolling, and selected item state inside one small object instead of scattering them across OLED drawing code.

### When to use

- OLED menu pages
- parameter tuning pages
- quick mode selection on small screens

## contest_ramp

### Principle

Do not jump to a target immediately. Move toward it with a limited step size.

### When to use

- motor setpoint smoothing
- servo target smoothing
- speed changes after state transitions

## contest_ring_buffer

### Principle

Use a circular buffer to decouple data production and data consumption.

### When to use

- UART RX interrupt to main-loop handoff
- sensor stream buffering
- small command queues

## contest_crc16

### Principle

CRC adds a compact error-detection code to data frames. If a byte changes in transmission, CRC usually changes too.

### When to use

- UART packets
- Modbus-like links
- simple wired communication between boards

## contest_encoder

### Principle

Use count difference over sample time to estimate rotational speed, then convert to RPM or linear speed.

### When to use

- DC motor speed loop
- wheel odometry
- motion feedback

## contest_incremental_pi

### Principle

Incremental PI updates the controller output by a delta value each cycle instead of recalculating the whole output from scratch.

### When to use

- motor speed loops
- PWM command adjustment
- cascaded control where the outer loop gives a speed target

### Why it is useful

It is common in motor-control examples because the result maps naturally onto "increase or decrease PWM a little."

## contest_periodic_task

### Principle

Run one fast system tick and let small periodic-task objects tell you when 1 ms, 10 ms, or 100 ms jobs are due.

### When to use

- superloop task scheduling
- sensor polling
- status LED or buzzer timing

## contest_line_sensor

### Principle

1. collect calibration min/max
2. normalize each sensor channel
3. use weighted average to estimate where the line is relative to the sensor array

### When to use

- line following
- edge tracking
- grayscale sensor arrays

### Why weighted error is useful

Instead of only asking "which sensor sees the line", weighted error gives a continuous position estimate. That makes PID much easier.

## contest_servo_map

### Principle

Map a desired mechanical angle onto the timer compare range that your servo expects.

### When to use

- camera pan
- steering servo
- grabber or small arm positioning

## contest_ultrasonic

### Principle

Convert echo pulse width into one-way distance using sound speed, then reject values that are outside the sensor's practical range.

### When to use

- obstacle avoidance
- simple front-distance detection
- near-field wall tracking

## contest_command_table

### Principle

Map command IDs to handler functions with a table so packet parsing and application actions stay cleanly separated.

### When to use

- upper-computer serial command control
- MCU-to-MCU command packets
- debug shells and parameter updates

## contest_packet

### Principle

Wrap communication data into:

- frame header
- length
- command
- payload
- CRC

That makes parsing much more reliable than reading random raw bytes.

### When to use

- MCU-to-MCU links
- upper-computer serial tools
- sensor modules with custom protocols

## contest_soft_timer

### Principle

Count ticks in software and compare against a timeout threshold.

### When to use

- timeout protection
- delayed actions
- periodic tasks inside a main loop

## contest_state_machine

### Principle

Represent application flow as named states and explicit transitions instead of many scattered flags.

### When to use

- startup sequence
- run, stop, and fault flow
- menu pages
- task mode switching in contests

## How the modules fit together

Typical line-following stack:

1. ADC reads sensor values
2. line sensor module normalizes and computes error
3. moving average smooths error
4. PID computes steering correction
5. ramp limits target change
6. motor driver applies left/right outputs

Typical UART stack:

1. UART RX ISR pushes bytes into ring buffer
2. main loop pops bytes
3. packet parser reconstructs valid frames
4. application logic handles decoded command

Typical speed-control stack:

1. encoder counts are sampled
2. speed is converted into RPM
3. low-pass filter smooths measured speed
4. ramp shapes the speed target
5. incremental PI updates PWM output

Typical button-menu stack:

1. GPIO or scan code produces raw key level
2. button helper debounces it
3. click or long-press event is generated
4. state machine changes current mode

Typical OLED menu stack:

1. buttons generate click events
2. menu helper updates cursor and visible window
3. OLED driver renders the visible labels
4. application logic handles the selected item

Typical balance-car stack:

1. IMU gives gyro rate and accelerometer angle
2. complementary filter fuses them into one stable body angle
3. speed loop adjusts the desired balance point
4. angle loop drives motor PWM
