# Host Tools

This folder contains simple PC-side helpers for contest-style serial debugging and upper-computer bringup.

## Why this folder exists

In many contest projects, the firmware side gets built first, but the PC side stays ad hoc. That slows down parameter tuning, command testing, and telemetry logging.

These scripts give you a clean starting point for:

- serial packet debugging
- command testing
- text telemetry logging
- upper-computer bringup before you write a full GUI

## Dependency

Install `pyserial` first:

```powershell
python -m pip install pyserial
```

Or:

```powershell
python -m pip install -r .\host_tools\requirements.txt
```

## Included tools

### `packet_console.py`

Use it when your board speaks the same framed protocol as `contest_packet`.

Protocol summary:

```text
0x55 0xAA length cmd payload... crc_low crc_high
```

Features:

- list serial ports
- send one-shot command frames
- open an interactive packet console
- print decoded RX frames in hex and ASCII
- support `loop://` for a no-hardware local self-test

Examples:

```powershell
python .\host_tools\packet_console.py --list-ports
python .\host_tools\packet_console.py --port COM6 --baud 115200 --cmd 0x10 --send-hex "01 02 03"
python .\host_tools\packet_console.py --port COM6 --baud 115200 --interactive
python .\host_tools\packet_console.py --port loop:// --cmd 0x31 --send-text "hello" --read-seconds 0.5
```

Interactive commands:

```text
help
sendhex 0x10 01 02 03
sendtext 0x20 hello world
quit
```

### `serial_line_logger.py`

Use it when the firmware prints one line of text per sample or event.

Features:

- print each line with a host timestamp
- optionally save all lines to CSV
- useful for CSV telemetry, startup logs, and quick protocol bringup
- support `loop://` for a no-hardware local self-test

Examples:

```powershell
python .\host_tools\serial_line_logger.py --port COM6 --baud 115200
python .\host_tools\serial_line_logger.py --port COM6 --baud 115200 --output .\logs\run01.csv
python .\host_tools\serial_line_logger.py --port loop:// --write-line "315,320,421,1.8" --duration 0.5
```

## How these scripts map to repository modules

- `packet_console.py` pairs with `contest_packet`, `contest_crc16`, `contest_ring_buffer`, and `contest_command_table`
- `serial_line_logger.py` pairs with any plain-text UART output, including CSV telemetry

See also:

- `examples/uart_packet_demo.c`
- `examples/command_table_demo.c`
- `stm32_hal/examples/stm32_uart_packet_hal.c`
- `docs/upper_computer_guide.md`
- `docs/ti_ecosystem_notes.md`
