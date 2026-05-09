# MSP430FR2355 Register-Level Starter

This folder is a minimal board starter for the `MSP-EXP430FR2355` LaunchPad using the device header and TI compiler that already exist on this machine.

## What this starter is

This is the fastest path to a working local TI example when the full `MSP430Ware DriverLib` package is not installed yet.

It uses:

- the `msp430fr2355.h` device header already present under CCS
- register-level peripheral setup
- the local `cl430` and `lnk430` tools

## What is included

- `include/contest_msp430fr2355_launchpad.h`: small helper functions and board pin definitions
- `src/blink_led.c`: LED heartbeat example
- `src/uart_hello.c`: backchannel UART hello example
- `tools/build_example.ps1`: local build helper for this machine

## Principle

### Why register-level first

Right now this machine clearly has:

- `CCS`
- `MSP430` compiler tools
- `MSP430FR2355` device header and linker files

But it does not yet show a full local `MSP430Ware DriverLib` tree.

That makes register-level code the most reliable "works today" starting point.

### Why this board first

`MSP430FR2355` is already supported locally enough to compile and link simple examples.

That makes it a better first TI target than trying to force `MSPM0` or `C2000` before their SDK trees are fully present.

## LaunchPad assumptions

These files assume the common `MSP-EXP430FR2355` LaunchPad wiring described in the official user guide:

- LED1 on `P1.0`
- backchannel application UART on `eUSCI_A1`
- `UCA1RXD` on `P4.2`
- `UCA1TXD` on `P4.3`

## Official reference entry points

- LaunchPad tool page: <https://www.ti.com/tool/MSP-EXP430FR2355>
- MCU product page: <https://www.ti.com/product/MSP430FR2355>
- LaunchPad user guide: <https://www.ti.com/lit/ug/slau680/slau680.pdf>
- software package page: <https://www.ti.com/tool/MSPWARE>

For board schematics, hardware design files, and BOM:
open the LaunchPad tool page and use the downloads section.

## Build on this machine

### Blink

```powershell
powershell -ExecutionPolicy Bypass -File .\ti_examples\msp430fr2355_register_level\tools\build_example.ps1 -Example blink_led
```

### UART hello

```powershell
powershell -ExecutionPolicy Bypass -File .\ti_examples\msp430fr2355_register_level\tools\build_example.ps1 -Example uart_hello
```

Outputs go under:

```text
ti_examples/msp430fr2355_register_level/build/<example-name>/
```

## Example notes

### `blink_led.c`

Principle:

- stop watchdog
- unlock GPIO from high-impedance startup state
- make `P1.0` an output
- toggle the LED in a simple software delay loop

Use it for:

- first power-on check
- confirming toolchain, clock default, and GPIO flow

### `uart_hello.c`

Principle:

- keep default `SMCLK` assumptions simple
- configure `eUSCI_A1` for a conservative `9600` baud setup
- print a boot banner and heartbeat lines over the LaunchPad backchannel UART

Use it for:

- verifying PC serial connection
- checking the eZ-FET application UART path
- giving yourself a stable text console before larger experiments

## Practical next step after this folder

After this starter works, the best next upgrade is:

1. install `MSP430Ware` if you want full DriverLib examples
2. keep simple reusable helpers in `ti_driverlib/msp430/`
3. only then expand toward ADC, timers, PWM, and more structured modules

## Local environment notes

This starter is based on the TI paths currently present on this machine:

- `D:\TI_CCS\ccs\tools\compiler\ti-cgt-msp430_21.6.1.LTS`
- `D:\TI_CCS\ccs\ccs_base\msp430\include`

If those paths move, update `tools/build_example.ps1`.
