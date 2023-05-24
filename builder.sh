#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"

OPTIONS="\
  Compile-MCU \
  Flash-MCU \
  Read-Serial-MCU \
  Compile-PC \
  Run-Flight-Deck \
  Install-Dependencies \
  Help \
  Quit\
"
usage() {
  echo "Usage:"
  echo "  ./builder.sh -d [dev]"
  echo ""
  echo "  where:"
  echo "    -d [dev] : The tty port the microcontroller is connected to."
  echo "               Default: /dev/ttyACM0"
  echo ""
  echo "COMMANDS:"
  echo "  1) Compile-MCU          : Compile the MCU code."
  echo "  2) Flash-MCU            : Flash the MCU code to the connected microcontroller."
  echo "  3) Read-Serial-MCU      : Read the serial output of the microcontroller."
  echo "  4) Compile-PC           : Compile the flight deck for the PC."
  echo "  5) Run-Flight-Deck      : Run the flight deck on the PC."
  echo "  6) Install-Dependencies : Install the required dependencies."
  echo "  7) Help                 : Display this message."
  echo "  8) Quit                 : Exit the builder script."
}

while getopts "d:h" opt; do
    case "$opt" in
        d) DEV="$OPTARG";
            ;;
        h) usage; exit 0
            ;;
        ?) usage; exit 1
            ;;
    esac
done

while true;
do
  echo "DEV = ${DEV}"

  select opt in ${OPTIONS};
  do
    if [ "$opt" = "Compile-MCU" ];
    then
      bash scripts/compile-mcu.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Flash-MCU" ];
    then
      bash scripts/flash-mcu.sh "$ROOT_DIR" "$DEV"
      break
    elif [ "$opt" = "Read-Serial-MCU" ];
    then
      bash scripts/read-serial-mcu.sh "$ROOT_DIR" "$DEV"
      break
    elif [ "$opt" = "Compile-PC" ];
    then
      bash scripts/compile-pc.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Run-Flight-Deck" ];
    then
      bash scripts/run-flight-deck.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Install-Dependencies" ];
    then
      bash scripts/install-dependencies.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Help" ];
    then
      usage
      break
    else
      break 2
    fi
  done
done
