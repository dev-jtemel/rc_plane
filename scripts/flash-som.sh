#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
DEV="$2"

flash_som() {
  ssh pi@192.168.0.13
}

echo "*************************"
echo "Flashing SOM"
echo "*************************"
flash_som
echo "****"
echo "Done"
echo "****"
