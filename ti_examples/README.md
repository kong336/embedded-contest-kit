# TI Examples

This folder holds board-level TI starter examples.

## Why this folder is separate from `ti_driverlib/`

`ti_driverlib/` is the reusable thin-wrapper layer.

`ti_examples/` is the board-facing place for:

- exact board pin assumptions
- build commands
- starter project structure
- minimal bringup examples you can compile and test first

## Current examples

- `msp430fr2355_register_level/`

That example was chosen first because this machine already has:

- `CCS` installed
- `cl430` available
- `MSP430FR2355` device headers available
- linker command files available

So it is the cleanest TI line to turn into a real local starter right now.
