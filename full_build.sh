#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"

echo "*** COMPILING ***"
bash scripts/compile-mcu.sh "$ROOT_DIR"
bash scripts/compile-pc.sh "$ROOT_DIR"
bash scripts/compile-som.sh "$ROOT_DIR"

echo "*** FLASHING ***"
bash scripts/flash-mcu.sh "$ROOT_DIR" "$DEV"
