#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
ARDUINO_CLI="$ROOT_DIR/bin/arduino-cli"
FQBN="arduino:avr:mega"

compile_mcu() {
  $ARDUINO_CLI compile --fqbn ${FQBN} impl/src/mcu/ -v
}

echo "*************************"
echo "Compiling Microcontroller"
echo "*************************"
compile_mcu
echo "****"
echo "Done"
echo "****"
