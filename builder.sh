#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"

OPTIONS="\
  Compile-MCU \
  Compile-SOM \
  Compile-PC \
  Flash-MCU \
  Run-Simulator \
  Run-Flight-Deck \
  Read-Serial-MCU \
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
  echo "  2) Compile-SOM          : Compile the SOM code."
  echo "  3) Compile-PC           : Compile the flight deck for the PC."
  echo "  4) Flash-MCU            : Flash the MCU code to the connected microcontroller."
  echo "  5) Run-Simulator        : Run the simulator on the SOM."
  echo "  6) Run-Flight-Deck      : Run the flight deck on the PC."
  echo "  7) Read-Serial-MCU      : Read the serial output of the microcontroller."
  echo "  8) Install-Dependencies : Install the required dependencies."
  echo "  9) Help                 : Display this message."
  echo "  10) Quit                : Exit the builder script."
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
    elif [ "$opt" = "Compile-SOM" ];
    then
      bash scripts/compile-som.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Compile-PC" ];
    then
      bash scripts/compile-pc.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Flash-MCU" ];
    then
      bash scripts/flash-mcu.sh "$ROOT_DIR" "$DEV"
      break
    elif [ "$opt" = "Run-Simulator" ];
    then
      bash scripts/run-simulator.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Run-Flight-Deck" ];
    then
      bash scripts/run-flight-deck.sh "$ROOT_DIR"
      break
    elif [ "$opt" = "Read-Serial-MCU" ];
    then
      bash scripts/read-serial-mcu.sh "$ROOT_DIR" "$DEV"
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
