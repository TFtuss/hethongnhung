#!/usr/bin/env bash
# flash.sh — helper to program a .bin via OpenOCD with sane defaults
# Usage: ./flash.sh [--file build/myproject.bin] [--serial <STLINK_SERIAL>] [--speed <kHz>]

set -euo pipefail
FILE="build/myproject.bin"
SERIAL=""
SPEED=100

usage(){
  echo "Usage: $0 [--file <path>] [--serial <STLINK_SERIAL>] [--speed <kHz>]"
  exit 1
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --file) FILE="$2"; shift 2;;
    --serial) SERIAL="$2"; shift 2;;
    --speed) SPEED="$2"; shift 2;;
    -h|--help) usage;;
    *) echo "Unknown arg: $1"; usage;;
  esac
done

if [ ! -f "$FILE" ]; then
  echo "Error: file not found: $FILE" >&2
  exit 2
fi

CMD=(openocd -c "set CPUTAPID 0x2ba01477" -f interface/stlink.cfg -f target/stm32f1x.cfg)
# Build the OpenOCD -c argument
OPENOCD_CMDS="reset_config none; adapter speed ${SPEED}; "
if [ -n "$SERIAL" ]; then
  OPENOCD_CMDS+="hla_serial ${SERIAL}; "
fi
OPENOCD_CMDS+="init; halt; program ${FILE} 0x08000000 verify; reset run; exit"

CMD+=( -c "$OPENOCD_CMDS" )

echo "Running: ${CMD[*]}"
exec "${CMD[@]}"
