# Contest Fast Start

This page is for the real competition situation:

- you already know the rough topic direction
- you can finish the wiring quickly
- you do not want to rebuild the firmware architecture from zero on site

The goal of this repository is not "zero code forever."

The real goal is:

- reuse the loop skeleton
- reuse the module split
- reuse common control and communication pieces
- reduce contest-time edits to pin mapping, parameters, and a few hardware hooks

## Realistic expectation

If you prepare around one familiar board and one familiar sensor stack in advance, this kit can often let you reuse most of the firmware structure.

What is realistic in a contest is usually:

- 70% to 90% direct reuse for architecture and helper modules
- 10% to 30% small edits for pins, timing, drivers, and final control logic

What is not realistic is:

- solder any random hardware
- skip all board bring-up
- never touch code at all

You still need to confirm that the real board, motor driver, sensors, and timing all match your assumptions.

## What you can reuse almost directly

- 1 ms tick plus superloop structure
- state machine layout such as stop, run, tune, and fault
- button handling and mode switching
- status LED and buzzer behavior
- UART packet framing and host-side debugging flow
- reusable control helpers such as PID, ramp, filtering, encoder math, and menu logic

## What you must still edit

- pin mapping
- timer and UART handle selection
- ADC channel order
- encoder counts-per-revolution
- motor polarity
- sensor direction
- parameter ranges and default targets

These are the normal "last mile" edits. They are small, but they cannot be guessed safely by a generic template.

## What you cannot avoid

- CubeMX, CCS, or SysConfig generated initialization
- the real driver for your IMU, OLED, ADC front end, or special sensor
- calibration
- safety cutoff logic
- verifying that your control tick is actually fixed

Those parts depend too much on the actual board and topic.

## Best low-stress contest route

1. Standardize one main board before the contest.
2. Pre-verify LED, UART, button, motor PWM, encoder, and ADC on that board.
3. Keep one known-good base project for that board only.
4. Copy the reusable modules from this repository into that known-good base.
5. During the contest, only replace the sensor hook, actuator hook, and a few parameters first.
6. Do not start by writing all application logic from scratch.

## Fast file map

If you only have a short amount of time, start here:

- `examples/contest_app_template.c`
- `stm32_hal/examples/stm32_contest_app_template_hal.c`
- `docs/project_recipes.md`
- `docs/bringup_checklist.md`

## How to use the generic contest template

`examples/contest_app_template.c` is the portable skeleton.

It already separates:

- input polling
- control update
- telemetry update
- status update

You only need to replace a few hardware hooks:

- `app_hw_read_mode_button_level`
- `app_hw_read_fault_line`
- `app_hw_sample_sensors`
- `app_hw_apply_outputs`
- `app_hw_send_telemetry`

## How to use the STM32 HAL template

`stm32_hal/examples/stm32_contest_app_template_hal.c` is the STM32-facing version.

It shows the usual competition split:

- HAL handles GPIO, UART, and LED output
- generic modules handle state, timing, and reusable control flow
- your board project fills in sensor acquisition and actuator write-back

Usually you only need to edit:

- the button pin
- the status LED pin
- the UART handle
- the sensor read function
- the motor output function

## Which level should you start from

Use the portable template when:

- you want to understand the architecture first
- you may port the same logic to STM32, TI, ESP32, or another MCU

Use the STM32 HAL template when:

- you already have a CubeMX project
- you want the fastest route to a working board project

## Good preparation before the contest

Prepare three base projects in advance:

1. Motion-control base: PWM, encoder, speed loop, stop logic
2. Sensor-acquisition base: ADC, UART log, OLED or serial parameter display
3. Communication base: packet protocol, upper-computer commands, telemetry output

If those three are ready, then many contest topics become "combine and trim" instead of "invent everything from zero."
