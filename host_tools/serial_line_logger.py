#!/usr/bin/env python3
"""
Simple line-based serial logger for bringup and telemetry capture.
"""

from __future__ import annotations

import argparse
import csv
import sys
import time
from datetime import datetime
from pathlib import Path

try:
    import serial
except ImportError:  # pragma: no cover - handled at runtime for users without pyserial
    serial = None


def require_pyserial() -> None:
    if serial is None:
        raise SystemExit(
            "pyserial is required. Install it with: python -m pip install -r .\\host_tools\\requirements.txt"
        )


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Read line-based serial telemetry and optionally save it to CSV.")
    parser.add_argument("--port", required=True, help="Serial port name, for example COM6 or loop://")
    parser.add_argument("--baud", type=int, default=115200, help="Serial baud rate")
    parser.add_argument("--timeout", type=float, default=0.1, help="Read timeout in seconds")
    parser.add_argument("--encoding", default="utf-8", help="Text encoding used by the device")
    parser.add_argument("--output", help="Optional CSV file path")
    parser.add_argument("--append", action="store_true", help="Append to the CSV file instead of overwriting it")
    parser.add_argument("--duration", type=float, default=0.0, help="Optional maximum runtime in seconds")
    parser.add_argument("--write-line", default="", help="Optional line to send once after opening the port")
    return parser


def main() -> int:
    args = build_argument_parser().parse_args()
    require_pyserial()

    csv_file = None
    csv_writer = None
    if args.output:
        output_path = Path(args.output)
        output_path.parent.mkdir(parents=True, exist_ok=True)
        mode = "a" if args.append else "w"
        csv_file = output_path.open(mode, newline="", encoding="utf-8")
        csv_writer = csv.writer(csv_file)
        if not args.append:
            csv_writer.writerow(["host_time_iso", "line"])

    ser = serial.serial_for_url(args.port, baudrate=args.baud, timeout=args.timeout)

    try:
        if args.write_line:
            # Sending one line on open makes loop:// self-tests and echo-style debug easier.
            ser.write((args.write_line + "\n").encode(args.encoding))
            ser.flush()

        deadline = time.monotonic() + args.duration if args.duration > 0 else None
        buffer = bytearray()

        while True:
            if deadline is not None and time.monotonic() >= deadline:
                break

            chunk = ser.read(256)
            if not chunk:
                continue

            for byte in chunk:
                if byte in (0x0A, 0x0D):
                    if not buffer:
                        continue

                    line = buffer.decode(args.encoding, errors="replace")
                    timestamp = datetime.now().isoformat(timespec="milliseconds")
                    print(f"[{timestamp}] {line}", flush=True)
                    if csv_writer is not None:
                        csv_writer.writerow([timestamp, line])
                        csv_file.flush()
                    buffer.clear()
                    continue

                buffer.append(byte)
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()
        if csv_file is not None:
            csv_file.close()

    return 0


if __name__ == "__main__":
    sys.exit(main())
