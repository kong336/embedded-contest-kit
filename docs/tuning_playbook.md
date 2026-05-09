# Tuning Playbook

This page is about tuning common contest control loops without getting lost.

## Line-following tuning order

1. verify raw sensor order
2. verify normalized values
3. verify line error sign
4. tune base speed
5. tune `kp`
6. add a little `kd`
7. only then consider `ki`

### Practical hints

- if the car zigzags quickly, `kp` or `kd` is too high
- if the car drifts wide before correcting, `kp` is too low
- if sensor noise causes twitching, smooth the error first

## Speed-loop tuning order

1. verify encoder sign and RPM conversion
2. low-pass the measured speed if needed
3. start with `ki = 0`
4. raise `kp` until response becomes useful
5. add `ki` slowly to remove steady-state error

### Practical hints

- if speed hunts slowly, `ki` is too aggressive
- if PWM changes too violently, add target ramping
- if measured speed is noisy, fix feedback first before raising gains

## Balance-car tuning order

1. verify fused angle sign
2. verify motor direction for "fall forward means drive forward"
3. tune angle loop first
4. keep speed loop weak at first
5. add fall detection and output cut-off before aggressive tests

### Practical hints

- if it instantly shoots away, check sign conventions first
- if it chatters in place, `kd` may be too high or IMU noise too large
- if it corrects too late, `kp` is too low or sample period is too slow

## Menu-based parameter tuning tips

- keep one key for fine-mode toggle
- change only one parameter at a time
- show current value and default value clearly
- save a known-good profile before trying bold values

## Good practice during tuning

- log raw values before filtered values
- change one thing, test, then write notes
- always keep output clamp and stop command available
