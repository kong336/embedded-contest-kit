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
- `mspm0g3507_register_level/`

`msp430fr2355_register_level/` was chosen first because this machine already has:

- `CCS` installed
- `cl430` available
- `MSP430FR2355` device headers available
- linker command files available

So it is the cleanest TI line to turn into a real local starter right now.

`mspm0g3507_register_level/` now extends that idea to a current `MSPM0` board:

- it uses the local `tiarmclang` toolchain
- it follows the local LaunchPad board metadata for `PA10/PA11` backchannel UART routing
- it keeps the code register-level because the full `MSPM0-SDK` tree is not yet present locally
