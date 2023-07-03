#!/usr/bin/env bash

ROOT_DIR="$PWD"

echo "*** COMPILING ***"
bash scripts/install-dependencies.sh "$ROOT_DIR"
bash scripts/compile-mcu.sh "$ROOT_DIR"
bash scripts/compile-som.sh "$ROOT_DIR"
