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

  export ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
  list=$(cat $ROOT_DIR/ARDUINO_DEPENDENCIES.txt)
  $ARDUINO_CLI lib install --git-url $list
}

setup_permissions() {
  sudo usermod -a -G dialout ${USER}
  sudo usermod -a -G tty ${USER}
}

install_apt() {
  list=$(cat $ROOT_DIR/APT_DEPENDENCIES.txt)
  sudo apt install $list
}

echo "**************************************"
echo "Installing apt dependencies"
echo "**************************************"
install_apt
echo "**************************************"
echo "Adding permissions"
echo "**************************************"
setup_permissions
echo "**************************************"
echo "Installing and configuring arduino-cli"
echo "**************************************"
arduino_cli
echo "****"
echo "Done"
echo "****"
