# Upper Computer Guide

This page is a practical note for contest-style serial debugging, telemetry, and quick upper-computer bringup.

## What "upper computer" means here

In small contest projects, the upper computer is usually the PC-side helper that does one or more of these jobs:

- send commands to the board
- receive status and telemetry
- plot or log data while tuning
- act as a quick test harness before you build a full GUI
- flash or monitor vendor evaluation boards

## Four useful levels

### Level 1: plain text serial

Principle:
Print human-readable lines first. This is still the fastest way to verify boot, key state, sensor values, and control-loop timing.

Good for:

- board bringup
- confirming interrupt flow
- checking whether ADC or encoder values are alive

Typical line format:

```text
tick=1024 speed_rpm=315 pwm=421 angle_deg=1.8
```

### Level 2: CSV-style telemetry

Principle:
Keep one line per sample so external tools can plot or save data without understanding your firmware internals.

Good for:

- PID tuning
- line-following error observation
- balance-car angle or speed traces

Typical line format:

```text
315,320,421,1.8
```

You can pair this with:

- Python scripts
- Serial Studio dashboards
- other serial-plot tools commonly used in student projects

### Level 3: structured packet protocol

Principle:
Once you need reliable parameter writes, command IDs, or binary payloads, use a framed packet instead of raw text.

This repository already provides that layer:

- `generic_c/include/contest_packet.h`
- `generic_c/include/contest_crc16.h`
- `generic_c/include/contest_command_table.h`
- `stm32_hal/include/contest_hal_uart_packet.h`

The frame format is:

```text
0x55 0xAA length cmd payload... crc_low crc_high
```

The CRC is Modbus CRC16 over:

```text
length + cmd + payload
```

Use this level when you need:

- parameter tuning pages
- robust command dispatch
- MCU-to-PC binary links
- one board controlling another board

### Level 4: vendor or ecosystem tools

Principle:
Sometimes the fastest path is not your own GUI. Use the vendor tool when it already solves flashing, monitoring, or data viewing.

For TI-focused work, the common official tool chain is:

- CCS Terminal or Serial Console for quick serial monitoring
- GUI Composer for custom desktop-style dashboards
- UniFlash for standalone programming

## Tools included in this repository

### `host_tools/packet_console.py`

Use it when your firmware already speaks `contest_packet`.

What it does:

- lists serial ports
- sends framed commands with CRC
- prints decoded return frames with both hex and ASCII views
- supports `loop://` for no-hardware self-test if `pyserial` is installed

### `host_tools/serial_line_logger.py`

Use it when your firmware prints line-based telemetry.

What it does:

- reads serial text lines
- prints them with host timestamps
- optionally saves them to CSV
- can also use `loop://` for local script verification

## Recommended workflow

1. Bring the board up with plain text serial first.
2. Switch to CSV or line-based telemetry when tuning loops.
3. Move to `contest_packet` when you need structured commands.
4. Add a GUI only after the packet and telemetry formats are stable.

## Practical format suggestions

### For the first power-on day

- plain text serial
- one line per event
- no packet protocol yet

### For PID tuning

- fixed-rate CSV lines
- constant field order
- avoid verbose labels in high-rate loops

### For parameter adjustment

- `contest_packet`
- one command ID per operation
- one response frame for ACK or current state

## Common mistakes

- mixing debug text and binary packets on the same UART without a clear switch
- sending too much text inside a fast control loop
- making payload formats change every day during debugging
- starting with a heavy GUI before the data format is stable

## Related repository files

- `examples/uart_packet_demo.c`
- `examples/command_table_demo.c`
- `stm32_hal/examples/stm32_uart_packet_hal.c`
- `host_tools/README.md`
- `docs/ti_ecosystem_notes.md`

## Useful links

- pySerial docs: <https://pyserial.readthedocs.io/en/latest/pyserial_api.html>
- Serial Studio: <https://serial-studio.com/>
- CCS Theia IDE guide: <https://software-dl.ti.com/ccs/esd/documents/users_guide_ccs_theia/ccs_ide.html>
- GUI Composer guide: <https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/middleware/gui_composer/doc_guide/doc_guide-srcs/Users_Guide.html>
- UniFlash: <https://www.ti.com/tool/UNIFLASH>
