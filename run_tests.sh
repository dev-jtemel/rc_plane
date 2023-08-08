#!/usr/bin/env bash

ROOT_DIR="$PWD"

bash ./scripts/run_tests.sh "$ROOT_DIR"
./build/common/test/rcplane_common_tests
