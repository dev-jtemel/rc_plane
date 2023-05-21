#!/usr/bin/env bash

DEV="/dev/ttyACM0"

OPTIONS="\
  Compile-MCU \
  Flash-MCU \
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
  echo "  3) Install-Dependencies : Install the required dependencies."
  echo "  4) Help                 : Display this message."
  echo "  5) Quit                 : Exit the builder script."
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
      echo "Compile-MCU"
      break
    elif [ "$opt" = "Flash-MCU" ];
    then
      break
    elif [ "$opt" = "Install-Dependencies" ];
    then
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
