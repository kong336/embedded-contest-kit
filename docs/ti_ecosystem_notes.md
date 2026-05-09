# TI Ecosystem Notes

This page is meant to answer a very practical contest question: if you keep seeing TI boards, SDKs, and upper-computer tools in tutorials or training camps, what should you actually learn first?

Status note:
The official links in this page were checked on 2026-05-09.

## Why TI keeps showing up in contest materials

Texas Instruments says on its China university program page that TI is the sole sponsor of the China Undergraduate Electronics Design Contest from 2018 to 2027. The same page also highlights MSP430 and C2000 course cases and links TI EVM resources used in the contest ecosystem.

Practical takeaway:
Even if your daily board is STM32, it is still worth understanding the TI naming and tool chain because contest training material, school labs, and sponsor resources often reference it.

## The three TI lines worth recognizing first

### MSPM0

Representative board:

- `LP-MSPM0G3507`

Official facts:

- 80 MHz Arm Cortex-M0+ MCU on the LaunchPad
- 128 KB flash and 32 KB SRAM
- 12-bit ADC up to 4 Msps, DAC, comparators, op amps, and CAN-FD support

Practical fit:

- general embedded control
- analog-heavy sensing tasks
- UART, CAN, PWM, and timer practice
- a good current-era TI entry point if you want a board that feels closer to modern 32-bit MCU workflows

### MSP430

Representative board:

- `MSP-EXP430FR2355`

Official facts:

- 16-bit MCU running up to 24 MHz
- 32 KB FRAM and 4 KB RAM
- built-in DAC, op amps, PGA, comparators, and dual UART support

Practical fit:

- low-power training
- measurement and mixed-signal basics
- simple controllers and instrumentation exercises
- learning the older but still very common TI teaching line

### C2000

Representative board and device:

- `LAUNCHXL-F280039C`
- `TMS320F280039C`

Official facts:

- 120 MHz real-time control MCU
- 384 KB flash and 69 KB RAM
- control-oriented hardware such as CLA, FPU, TMU, multiple 12-bit ADCs, high-resolution PWM resources, and eQEP

Practical fit:

- motor control
- digital power
- inverter or servo control
- projects where deterministic real-time control matters more than general app structure

## Older boards you will still see in tutorials

### MSP430G2 LaunchPad

Why it still appears:

- many beginner tutorials and lab notes were built around the old low-cost LaunchPad line

Practical advice:

- use it when you are following an existing school handout
- for new self-directed work, prefer current MSPM0 or an FRAM-based MSP430 board

### TM4C123 LaunchPad

Representative board:

- `EK-TM4C123GXL`

Official facts:

- 80 MHz Arm Cortex-M4F MCU
- 256 KB flash and 32 KB SRAM on the LaunchPad target device
- on-board ICDI debug interface and USB device support

Practical advice:

- still useful if your tutorial set or older upper-computer examples target TivaWare
- not the first TI family I would recommend for brand-new learning in 2026 unless your materials already depend on it

## A simple selection rule

### If the task is broad embedded control

Start with:

- the exact board required by the contest or school
- otherwise `LP-MSPM0G3507` if you want a current TI 32-bit path

### If the task is low-power or analog teaching material

Start with:

- `MSP-EXP430FR2355`

### If the task is motor control or power electronics

Start with:

- `LAUNCHXL-F280039C`

## TI upper-computer and support tools

### CCS

Use it for:

- build
- debug
- register view
- quick serial monitoring inside the TI IDE

Notes:
Older docs often call this Terminal View. Current CCS Theia docs also describe a Serial Console workflow.

### GUI Composer

Use it for:

- making a PC-side control or visualization panel
- TI demo dashboards
- quick custom GUIs without writing a full desktop app from zero

### UniFlash

Use it for:

- standalone board flashing
- mass programming
- cases where you do not want the full IDE open

## If your main codebase is STM32 today

The easiest mental map is:

- keep your reusable logic style the same
- treat TI SDK or driver code as the new board-binding layer
- keep your protocol, filter, PID, and scheduling logic portable

That is exactly why this repository keeps the reusable pieces in `generic_c/`.

## Suggested learning order

1. GPIO, timer, PWM, UART, and ADC on the exact TI LaunchPad you have
2. One line-based serial telemetry demo
3. One `contest_packet` command-and-response demo
4. One closed-loop example such as speed, servo, or angle control
5. Only then move to a heavier GUI or upper-computer panel

## Official links

- TI China university program: <https://www.ti.com/about-ti/china-university-program.html>
- LP-MSPM0G3507: <https://www.ti.com/tool/LP-MSPM0G3507>
- MSP-EXP430FR2355: <https://www.ti.com/tool/MSP-EXP430FR2355>
- LAUNCHXL-F280039C: <https://www.ti.com/tool/LAUNCHXL-F280039C>
- TMS320F280039C: <https://www.ti.com/product/TMS320F280039C>
- EK-TM4C123GXL: <https://www.ti.com/tool/EK-TM4C123GXL>
- MSP-EXP430G2ET: <https://www.ti.com/tool/MSP-EXP430G2ET>
- CCS Theia IDE guide: <https://software-dl.ti.com/ccs/esd/documents/users_guide_ccs_theia/ccs_ide.html>
- CCS Terminal View doc: <https://software-dl.ti.com/ccs/esd/documents/ccs_terminal_view.html>
- GUI Composer guide: <https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/middleware/gui_composer/doc_guide/doc_guide-srcs/Users_Guide.html>
- UniFlash: <https://www.ti.com/tool/UNIFLASH>
