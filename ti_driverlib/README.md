# TI DriverLib Starters

This folder collects thin TI-family starter helpers for the boards and MCU lines that appear most often in contest-style materials.

## Why this folder exists

The generic reusable modules in this repository stay board-independent on purpose.

But when you jump into a TI project, you still need a small layer that wraps the family API you touch every day:

- LED or GPIO output
- UART send
- PWM compare update
- quick ADC reads

That is what these headers provide.

## Included families

- `mspm0/contest_ti_mspm0_driverlib.h`
- `msp430/contest_ti_msp430_driverlib.h`
- `c2000/contest_ti_c2000_driverlib.h`
- `tm4c/contest_ti_tm4c_driverlib.h`

## Design rule

These files are intentionally thin:

- they do not try to replace the TI SDK
- they do not hardcode one LaunchPad pin map
- they assume clocks, pinmux, and low-level peripheral init already exist in the real project

Use them as a small reusable layer above the official SDK.

## Recommended reading

- `docs/ti_board_reference.md`
- `docs/ti_driverlib_basics.md`
- `docs/ti_ecosystem_notes.md`
