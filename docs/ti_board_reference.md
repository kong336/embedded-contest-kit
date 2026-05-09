# TI Board Reference

This page is a quick index for the TI MCU boards and families that keep showing up in contest training, lab handouts, and older tutorial sets.

Status note:
The official links in this page were checked on 2026-05-09.

## How to use this page

When you pick a TI board, open these four things first:

1. the board tool page
2. the target MCU product page
3. the user guide or board manual
4. the matching SDK or software package

For schematics, BOM, layout files, and gerbers:
open the board tool page and look for the hardware design files section in the downloads area.

## Common TI boards worth keeping bookmarked

| Board | Common use in student projects | Target MCU line | What to open first |
| --- | --- | --- | --- |
| `LP-MSPM0G3507` | current general-purpose TI entry board, ADC/PWM/UART/CAN practice | `MSPM0G3507` | tool page, product page, MSPM0-SDK, board user guide |
| `MSP-EXP430FR2355` | low-power and mixed-signal teaching, ADC and UART basics | `MSP430FR2355` | tool page, product page, MSP430Ware, board user guide |
| `LAUNCHXL-F280039C` | motor control, digital power, real-time control tasks | `TMS320F280039C` | tool page, product page, C2000Ware, LaunchPad guide |
| `EK-TM4C123GXL` | older but still common Tiva or TM4C course material | `TM4C123GH6PM` | tool page, product page, TivaWare, user manual |
| `MSP-EXP430G2ET` | old beginner LaunchPad kits and legacy classroom examples | `MSP430G2553` class | tool page, product family pages, MSP430Ware, LaunchPad guide |

## Family notes

### MSPM0

Why it matters:

- this is the easiest current TI path if you want a modern 32-bit MCU workflow
- TI positions MSPM0 as a broad MCU platform with DriverLib, examples, SysConfig support, and middleware in one SDK

Good fit:

- general embedded control
- analog front-end practice
- UART, CAN, timer, PWM, ADC, comparator work

Best first board:

- `LP-MSPM0G3507`

Open these:

- board page: <https://www.ti.com/tool/LP-MSPM0G3507>
- MCU page: <https://www.ti.com/product/MSPM0G3507>
- SDK page: <https://www.ti.com/tool/download/MSPM0-SDK>
- GUI Composer guide inside the SDK docs: <https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/middleware/gui_composer/doc_guide/doc_guide-srcs/Users_Guide.html>

### MSP430

Why it matters:

- a lot of school material still uses MSP430 because the teaching ecosystem is huge
- FRAM-based devices such as `MSP430FR2355` are still practical for learning analog and low-power basics

Good fit:

- low-power measurement tasks
- small controllers
- analog and mixed-signal exercises

Best first boards:

- `MSP-EXP430FR2355`
- `MSP-EXP430G2ET` when you must follow older handouts

Open these:

- FR2355 board page: <https://www.ti.com/tool/MSP-EXP430FR2355>
- FR2355 MCU page: <https://www.ti.com/product/MSP430FR2355>
- older G2 LaunchPad page: <https://www.ti.com/tool/MSP-EXP430G2ET>
- software package: <https://www.ti.com/tool/MSPWARE>

### C2000

Why it matters:

- this is the TI family you keep seeing around motor drives, inverters, digital power, and fast control loops
- TI packages driver examples, board files, and control-oriented libraries in C2000Ware

Good fit:

- motor speed loops
- servo or inverter control
- current-loop and power-conversion labs

Best first board:

- `LAUNCHXL-F280039C`

Open these:

- board page: <https://www.ti.com/tool/LAUNCHXL-F280039C>
- MCU page: <https://www.ti.com/product/TMS320F280039C>
- software package: <https://www.ti.com/tool/C2000WARE>
- software guide: <https://software-dl.ti.com/C2000/docs/software_guide/_static/pdf/C2000_Software_Guide.pdf>

### TM4C or Tiva C

Why it still matters:

- many older course projects, serial tools, and forum posts still target the TM4C123 LaunchPad
- it is not the newest TI MCU line, but you will still run into it often enough that it is worth recognizing

Good fit:

- following legacy class notes
- quick ARM Cortex-M4F practice on older TI examples

Best first board:

- `EK-TM4C123GXL`

Open these:

- board page: <https://www.ti.com/tool/EK-TM4C123GXL>
- MCU page: <https://www.ti.com/product/TM4C123GH6PM>
- software package: <https://www.ti.com/tool/SW-TM4C>

## What "manual", "schematic", and "software" usually mean on TI pages

### Tool page

Usually contains:

- board overview
- downloads
- user guide
- hardware design files
- example software links

### Product page

Usually contains:

- datasheet
- errata
- technical reference manual
- package and pin information

### SDK or software page

Usually contains:

- DriverLib
- peripheral examples
- board support pieces
- API guides
- release notes

## Practical recommendation

If you only want to learn one current TI MCU line first, start with:

- `MSPM0` for broad embedded work
- `C2000` if your focus is motor control or power electronics

If your school material is old, keep these two legacy lines on your radar:

- `MSP430`
- `TM4C`
