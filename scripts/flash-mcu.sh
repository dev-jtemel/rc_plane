#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
DEV="$2"
ARDUINO_CLI="$ROOT_DIR/bin/arduino-cli"
FQBN="arduino:avr:mega"

flash_mcu() {
  sudo chmod a+rw $DEV
  $ARDUINO_CLI upload -p ${DEV} --fqbn ${FQBN} mcu/ -v
}

echo "*************************"
echo "Compiling Microcontroller"
echo "*************************"
flash_mcu
echo "****"
echo "Done"
echo "****"
