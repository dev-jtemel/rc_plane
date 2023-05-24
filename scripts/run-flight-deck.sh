#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
BUILD_DIR="$ROOT_DIR/build"

run_pc() {
  $BUILD_DIR/pc/flight-deck
}

echo "*************************"
echo "Running Flight Deck"
echo "*************************"
run_pc
echo "****"
echo "Done"
echo "****"
