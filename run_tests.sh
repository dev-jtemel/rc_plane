#!/usr/bin/env bash

ROOT_DIR="$PWD"

bash scripts/install-dependencies.sh "$ROOT_DIR"
bash scripts/run-tests.sh "$ROOT_DIR" "False"
