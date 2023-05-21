#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
DEV="$2"

read_serial_mcu() {
  sudo chmod a+rw $DEV
  sudo picocom -b 115200 $DEV
}

echo "*************************"
echo "Compiling Microcontroller"
echo "*************************"
read_serial_mcu
echo "****"
echo "Done"
echo "****"
