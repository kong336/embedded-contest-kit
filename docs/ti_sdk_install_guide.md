# TI SDK Install Guide

This page is the short answer to the common confusion around:

- `CCS`
- `SDK`
- `DriverLib`
- `SysConfig`
- `UniFlash`

It is written for the contest or practice workflow this repository targets.

## The five names people mix together

### CCS

`Code Composer Studio`, usually shortened to `CCS`, is the TI IDE and toolchain bundle.

Think of it as:

- the editor and project manager
- the TI compiler front end
- debugger integration
- target database and flash or debug support

On this machine, CCS is already installed.

## SDK

`SDK` means the vendor software package for a board or MCU family.

For TI it usually contains:

- device headers
- `DriverLib`
- example projects
- middleware
- documentation
- `SysConfig` board or peripheral metadata

Examples:

- `MSPM0-SDK` for `MSPM0`
- `MSP430Ware` for `MSP430`
- `C2000Ware` for `C2000`
- `TivaWare` for `TM4C`

## DriverLib

`DriverLib` is the TI peripheral abstraction library inside the SDK.

It sits between raw registers and your application code.

Typical use:

- raw register code is fastest for first bringup or when the SDK is missing
- DriverLib is better when you want readable initialization and reusable project code

## SysConfig

`SysConfig` is TI's configuration tool.

It helps you generate:

- pin mux setup
- peripheral instance setup
- board aliases
- startup glue for SDK-style projects

For `MSPM0`, this matters a lot because many official examples expect a `SysConfig`-generated file set.

## UniFlash and DSLite

These are TI programming tools.

- `UniFlash`: GUI-focused flashing tool
- `DSLite`: command-line flashing tool used by CCS under the hood

Use them when you already have an `.out` file and want to program the board without opening the full IDE.

## What is present on this machine right now

Local checks already show:

- `CCS` is installed
- `tiarmclang` is available for `MSPM0`
- `cl430` is available for `MSP430`
- `C2000` compiler tools are present
- `DSLite` is present

Practical catch:

- the full `MSPM0-SDK` tree is not visible locally yet
- that is why this repository currently keeps the `MSPM0G3507` starter at register level

## What this repository does with TI code

### `ti_examples/`

Use this folder for board-facing first bringup:

- exact LaunchPad assumptions
- build scripts
- power-on GPIO or UART tests
- minimum local examples that compile now

Current examples:

- `msp430fr2355_register_level`
- `mspm0g3507_register_level`

### `ti_driverlib/`

Use this folder for reusable TI-family helper code after the SDK or DriverLib layer is in place.

That is where the code should move once the board is stable and you want cleaner reuse.

## Recommended install order

### If you are starting with MSPM0

1. Keep `CCS` installed.
2. Install `MSPM0-SDK`.
3. Confirm `SysConfig` can open `LP-MSPM0G3507` examples.
4. Use this repository's register-level starter to sanity-check the board first.
5. Move to DriverLib and SysConfig-generated projects after the backchannel UART and LEDs work.

### If you are starting with MSP430

1. Keep `CCS` installed.
2. Install `MSP430Ware` if you want the full example set.
3. Use the `MSP430FR2355` register-level starter here for first bringup.
4. Move reusable helpers into `ti_driverlib/msp430/` after the board path is stable.

### If you are starting with C2000

1. Keep `CCS` installed.
2. Install `C2000Ware`.
3. Expect more focus on motor control, PWM, ADC timing, and real-time control examples.
4. Treat the board examples and SDK docs as required reading before abstraction.

## Practical reading order inside this repository

1. `docs/ti_ecosystem_notes.md`
2. `docs/ti_board_reference.md`
3. `docs/ti_driverlib_basics.md`
4. `ti_examples/README.md`
5. the specific board example folder you are actually using

## Official entry points

- `MSPM0-SDK`: <https://www.ti.com/tool/download/MSPM0-SDK>
- `MSP430Ware`: <https://www.ti.com/tool/MSPWARE>
- `C2000Ware`: <https://www.ti.com/tool/C2000WARE>
- `TivaWare`: <https://www.ti.com/tool/SW-TM4C>
- `UniFlash`: <https://www.ti.com/tool/UNIFLASH>
- `CCS guide`: <https://software-dl.ti.com/ccs/esd/documents/users_guide_ccs_theia/ccs_ide.html>

## Quick mental model

If you want the shortest version:

- `CCS` is the workbench
- `SDK` is the software package
- `DriverLib` is the peripheral helper library inside the SDK
- `SysConfig` is the generator and configurator
- `UniFlash` or `DSLite` is for programming the target
