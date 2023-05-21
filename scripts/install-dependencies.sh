#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
ARDUINO_CLI="$ROOT_DIR/bin/arduino-cli"

arduino_cli() {
  # Installs into rc-plane/bin
  curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
  $ARDUINO_CLI config init
  $ARDUINO_CLI board list
  $ARDUINO_CLI core update-index
  $ARDUINO_CLI core install arduino:avr
  $ARDUINO_CLI core list
}

setup_permissions() {
  sudo usermod -a -G dialout ${USER}
  sudo usermod -a -G tty ${USER}
}

install_apt() {
  sudo apt install picocom
}

echo "**************************************"
echo "Installing and configuring arduino-cli"
echo "**************************************"
arduino_cli
echo "**************************************"
echo "Adding permissions"
echo "**************************************"
setup_permissions
echo "**************************************"
echo "Installing apt dependencies"
echo "**************************************"
install_apt
echo "****"
echo "Done"
echo "****"
