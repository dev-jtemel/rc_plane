#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"

echo "*** COMPILING ***"
bash scripts/install-dependencies.sh "$ROOT_DIR"
bash scripts/compile-mcu.sh "$ROOT_DIR"
bash scripts/compile-som.sh "$ROOT_DIR"
