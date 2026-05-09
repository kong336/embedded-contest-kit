#!/usr/bin/env python3
"""
PC-side helper for the contest_packet framing format.
"""

from __future__ import annotations

import argparse
import shlex
import string
import sys
import threading
import time
from dataclasses import dataclass
from typing import Iterable, Optional

try:
    import serial
    from serial.tools import list_ports
except ImportError:  # pragma: no cover - handled at runtime for users without pyserial
    serial = None
    list_ports = None


HEAD1 = 0x55
HEAD2 = 0xAA
MAX_PAYLOAD = 64


@dataclass
class PacketFrame:
    cmd: int
    payload: bytes

    @property
    def length(self) -> int:
        return len(self.payload)


def require_pyserial() -> None:
    if serial is None:
        raise SystemExit(
            "pyserial is required. Install it with: python -m pip install -r .\\host_tools\\requirements.txt"
        )


def crc16_modbus(data: Iterable[int]) -> int:
    crc = 0xFFFF
    for value in data:
        crc ^= value & 0xFF
        for _ in range(8):
            if crc & 0x0001:
                crc = ((crc >> 1) ^ 0xA001) & 0xFFFF
            else:
                crc = (crc >> 1) & 0xFFFF
    return crc


def encode_frame(cmd: int, payload: bytes) -> bytes:
    if not 0 <= cmd <= 0xFF:
        raise ValueError("cmd must fit in one byte")
    if len(payload) > MAX_PAYLOAD:
        raise ValueError(f"payload too long, max is {MAX_PAYLOAD} bytes")

    body = bytes([len(payload), cmd]) + payload
    crc = crc16_modbus(body)
    return bytes([HEAD1, HEAD2]) + body + bytes([crc & 0xFF, (crc >> 8) & 0xFF])


class PacketParser:
    """
    Mirrors the C-side packet parser so the PC tool follows the exact same rules.
    """

    def __init__(self) -> None:
        self.reset()

    def reset(self) -> None:
        self.state = "head1"
        self.length = 0
        self.cmd = 0
        self.payload = bytearray()
        self.crc_low = 0

    def feed(self, value: int) -> Optional[PacketFrame]:
        byte = value & 0xFF

        if self.state == "head1":
            if byte == HEAD1:
                self.state = "head2"
            return None

        if self.state == "head2":
            if byte == HEAD2:
                self.state = "length"
            else:
                self.state = "head1"
            return None

        if self.state == "length":
            if byte > MAX_PAYLOAD:
                self.reset()
            else:
                self.length = byte
                self.payload.clear()
                self.state = "cmd"
            return None

        if self.state == "cmd":
            self.cmd = byte
            self.state = "crc_low" if self.length == 0 else "payload"
            return None

        if self.state == "payload":
            self.payload.append(byte)
            if len(self.payload) >= self.length:
                self.state = "crc_low"
            return None

        if self.state == "crc_low":
            self.crc_low = byte
            self.state = "crc_high"
            return None

        if self.state == "crc_high":
            body = bytes([self.length, self.cmd]) + bytes(self.payload)
            crc = crc16_modbus(body)
            frame = None
            if self.crc_low == (crc & 0xFF) and byte == ((crc >> 8) & 0xFF):
                frame = PacketFrame(self.cmd, bytes(self.payload))
            self.reset()
            return frame

        self.reset()
        return None


def format_hex(payload: bytes) -> str:
    return " ".join(f"{byte:02X}" for byte in payload) if payload else "(empty)"


def format_ascii(payload: bytes) -> str:
    if not payload:
        return ""
    printable = set(string.printable) - {"\r", "\n", "\t", "\x0b", "\x0c"}
    return "".join(chr(byte) if chr(byte) in printable else "." for byte in payload)


def parse_u8(text: str) -> int:
    value = int(text, 0)
    if not 0 <= value <= 0xFF:
        raise ValueError(f"value out of byte range: {text}")
    return value


def parse_hex_payload(text: str) -> bytes:
    cleaned = text.replace(",", " ").strip()
    if not cleaned:
        return b""
    tokens = cleaned.split()
    return bytes(parse_u8(token) for token in tokens)


def print_ports() -> None:
    require_pyserial()
    ports = list(list_ports.comports())
    if not ports:
        print("No serial ports found.")
        return
    for port in ports:
        description = port.description or "(no description)"
        print(f"{port.device}: {description}")


def send_frame(ser: "serial.SerialBase", cmd: int, payload: bytes) -> bytes:
    frame = encode_frame(cmd, payload)
    ser.write(frame)
    ser.flush()
    return frame


def reader_loop(ser: "serial.SerialBase", stop_event: threading.Event, print_lock: threading.Lock) -> None:
    parser = PacketParser()
    while not stop_event.is_set():
        data = ser.read(64)
        if not data:
            continue
        for value in data:
            frame = parser.feed(value)
            if frame is not None:
                timestamp = time.strftime("%H:%M:%S")
                with print_lock:
                    print(
                        f"[{timestamp}] RX cmd=0x{frame.cmd:02X} len={frame.length} "
                        f"hex={format_hex(frame.payload)} ascii={format_ascii(frame.payload)!r}",
                        flush=True,
                    )


def interactive_shell(
    ser: "serial.SerialBase",
    stop_event: threading.Event,
    print_lock: threading.Lock,
) -> None:
    help_text = (
        "Commands:\n"
        "  help\n"
        "  sendhex <cmd> <byte0> <byte1> ...\n"
        "  sendtext <cmd> <text>\n"
        "  quit\n"
    )

    with print_lock:
        print(help_text, flush=True)

    while not stop_event.is_set():
        try:
            line = input("> ").strip()
        except EOFError:
            break
        except KeyboardInterrupt:
            print()
            break

        if not line:
            continue

        parts = shlex.split(line)
        command = parts[0].lower()

        try:
            if command == "help":
                with print_lock:
                    print(help_text, flush=True)
                continue

            if command == "quit":
                break

            if command == "sendhex":
                if len(parts) < 2:
                    raise ValueError("usage: sendhex <cmd> <byte0> <byte1> ...")
                cmd = parse_u8(parts[1])
                payload = bytes(parse_u8(token) for token in parts[2:])
                frame = send_frame(ser, cmd, payload)
                with print_lock:
                    print(f"TX {format_hex(frame)}", flush=True)
                continue

            if command == "sendtext":
                if len(parts) < 3:
                    raise ValueError("usage: sendtext <cmd> <text>")
                cmd = parse_u8(parts[1])
                payload = " ".join(parts[2:]).encode("utf-8")
                frame = send_frame(ser, cmd, payload)
                with print_lock:
                    print(f"TX {format_hex(frame)}", flush=True)
                continue

            raise ValueError(f"unknown command: {command}")
        except ValueError as exc:
            with print_lock:
                print(f"Error: {exc}", flush=True)


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Serial console for the contest_packet protocol.")
    parser.add_argument("--port", help="Serial port name, for example COM6 or loop://")
    parser.add_argument("--baud", type=int, default=115200, help="Serial baud rate")
    parser.add_argument("--timeout", type=float, default=0.1, help="Read timeout in seconds")
    parser.add_argument("--list-ports", action="store_true", help="List available serial ports and exit")
    parser.add_argument("--cmd", help="One-shot command byte, for example 0x10")
    parser.add_argument("--send-hex", default="", help="One-shot payload bytes, for example '01 02 03'")
    parser.add_argument("--send-text", default="", help="One-shot UTF-8 payload text")
    parser.add_argument("--read-seconds", type=float, default=1.0, help="How long to keep reading after a one-shot send")
    parser.add_argument("--interactive", action="store_true", help="Open the interactive shell after connecting")
    return parser


def main() -> int:
    args = build_argument_parser().parse_args()

    if args.list_ports:
        print_ports()
        return 0

    if not args.port:
        raise SystemExit("--port is required unless --list-ports is used")

    require_pyserial()

    ser = serial.serial_for_url(args.port, baudrate=args.baud, timeout=args.timeout)
    stop_event = threading.Event()
    print_lock = threading.Lock()
    reader = threading.Thread(target=reader_loop, args=(ser, stop_event, print_lock), daemon=True)
    reader.start()

    try:
        if args.cmd is not None:
            cmd = parse_u8(args.cmd)
            if args.send_hex and args.send_text:
                raise SystemExit("Use either --send-hex or --send-text, not both")
            payload = parse_hex_payload(args.send_hex) if args.send_hex else args.send_text.encode("utf-8")
            frame = send_frame(ser, cmd, payload)
            with print_lock:
                print(f"TX {format_hex(frame)}", flush=True)
            if args.read_seconds > 0:
                time.sleep(args.read_seconds)

        if args.interactive:
            interactive_shell(ser, stop_event, print_lock)
    except KeyboardInterrupt:
        pass
    finally:
        stop_event.set()
        reader.join(timeout=1.0)
        ser.close()

    return 0


if __name__ == "__main__":
    sys.exit(main())
