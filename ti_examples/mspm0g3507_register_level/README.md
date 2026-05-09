# MSPM0G3507 Register-Level Starter

This folder is a minimal board starter for the `LP-MSPM0G3507` LaunchPad using the TI ARM Clang toolchain that is already installed on this machine.

## What this starter is

This is the practical path to a usable local `MSPM0` example before a full `MSPM0-SDK` tree is available.

It uses:

- the local `tiarmclang` compiler
- a copied TI linker command file and startup source layout
- small register-level helpers for GPIO, IOMUX, and UART0

## What is included

- `include/contest_mspm0g3507_launchpad.h`: board helpers and LaunchPad pin assumptions
- `include/ti/devices/msp/msp.h`: tiny compatibility include so the TI startup source can compile
- `include/ti/devices/msp/m0p/mspm0g3507.h`: minimal device register definitions used by these examples
- `linker/mspm0g3507.cmd`: TI linker command file copied into the repository
- `src/startup_mspm0g350x_ticlang.c`: TI-style startup source for TIClang builds
- `src/blink_led.c`: RGB LED heartbeat example
- `src/uart_hello.c`: UART0 backchannel hello example
- `tools/build_example.ps1`: local build helper for this machine

## Principle

### Why register-level first

This machine clearly has:

- `CCS`
- `tiarmclang`
- `MSPM0G3507` target database entries
- board guidance for `LP-MSPM0G3507`

But it does not yet show a full local `MSPM0-SDK` installation with the normal `ti/devices/msp/...` header tree and DriverLib sources.

That makes a small register-level starter the most honest "works now" bridge.

### What this folder tries to prove

It is not trying to replace DriverLib.

It is proving that you can still:

- bring up GPIO
- drive the LaunchPad RGB LED
- route the XDS110 backchannel UART pins
- print boot text at `115200`

even before the complete SDK is installed.

## LaunchPad assumptions

These files follow the common `LP-MSPM0G3507` LaunchPad wiring reflected by the local TI board metadata:

- backchannel UART uses `UART0`
- `UART0 TX` is on `PA10`
- `UART0 RX` is on `PA11`
- `LED2 Green` is on `PB22`
- `LED2 Red` is on `PB26`
- `LED2 Blue` is on `PB13`
- `LED1` is on `PA0`, but this folder avoids it because `PA0` conflicts with the older `UART0 TX` alternate path

## Official reference entry points

- LaunchPad tool page: <https://www.ti.com/tool/LP-MSPM0G3507>
- MCU product page: <https://www.ti.com/product/MSPM0G3507>
- SDK page: <https://www.ti.com/tool/download/MSPM0-SDK>
- CCS Theia users guide: <https://software-dl.ti.com/ccs/esd/documents/users_guide_ccs_theia/ccs_ide.html>

For schematics, board files, user guide PDFs, and BOM:
open the LaunchPad tool page and use its downloads section.

## Build on this machine

### Blink

```powershell
powershell -ExecutionPolicy Bypass -File .\ti_examples\mspm0g3507_register_level\tools\build_example.ps1 -Example blink_led
```

### UART hello

```powershell
powershell -ExecutionPolicy Bypass -File .\ti_examples\mspm0g3507_register_level\tools\build_example.ps1 -Example uart_hello
```

Outputs go under:

```text
ti_examples/mspm0g3507_register_level/build/<example-name>/
```

## Example notes

### `blink_led.c`

Principle:

- configure the RGB LED pins as GPIO through `PINCM`
- enable GPIOB output for the selected color
- use `DOUTSET31_0` and `DOUTCLR31_0` for clean single-bit writes

Use it for:

- first power-on check
- confirming GPIO, IOMUX, and clock-default assumptions
- having a dead-simple LED probe before larger work

### `uart_hello.c`

Principle:

- route `PA10` and `PA11` to `UART0`
- select `BUSCLK` as the UART source
- use a simple `115200` divider for the default contest-friendly text console
- print a boot banner and periodic heartbeat line

Use it for:

- verifying the XDS110 virtual COM port path
- confirming your serial terminal settings
- getting a text heartbeat before adding sensors or control code

## Practical next step after this folder

After this starter works, the best next upgrade is:

1. install the full `MSPM0-SDK`
2. move the reusable parts into `ti_driverlib/mspm0/`
3. expand toward timers, ADC, PWM, SysConfig output, and DriverLib-based board code

## Local environment notes

This starter is based on the TI paths currently present on this machine:

- `D:\TI_CCS\ccs\tools\compiler\ti-cgt-armllvm_4.0.4.LTS`
- `D:\TI_CCS\ccs\theia\resources\ai\boards\LP-MSPM0G3507`
- `D:\TI_CCS\ccs\ccs_base\common\targetdb\devices\MSPM0G3507.xml`

If the compiler path moves, update `tools/build_example.ps1`.
