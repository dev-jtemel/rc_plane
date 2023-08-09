#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"
SOMIP="192.168.0.30"
MODE="PC"

OPTIONS="\
  Change-Mode \
  Compile \
  Flash \
  Run \
  Test \
  GPS-Fake \
  Read-Serial \
  Install-Dependencies \
  Run-Clang-Format \
  Help \
  Quit\
"
usage() {
  echo "Usage:"
  echo "  ./builder.sh -d [dev] -i [IP]"
  echo ""
  echo "  where:"
  echo "    -d [dev] : The tty port the microcontroller is connected to."
  echo "               Default: /dev/ttyACM0"
  echo "    -i       : SoM IP address."
  echo "               Default: 192.168.0.13"
  echo ""
  echo "COMMANDS:"
  echo "  1) Change-Mode          : Cycle between MCU, SOM and PC (default: PC)."
  echo "  2) Compile              : Compile the mode's code."
  echo "  3) Flash                : Flash the modes code (via serial for MCU or ssh for SOM)."
  echo "  4) Run                  : Run the desired code on the appropiate device."
  echo "  5) Test                 : Compile and run the tests for the appropiate device."
  echo "  6) GPS-Fake             : Start a simulated GPSd instance."
  echo "  7) Read-Serial          : Read the serial output of the microcontroller."
  echo "  8) Install-Dependencies : Install the required dependencies."
  echo "  9) Run-Clang-Format     : Run clang-format on the project."
  echo "  10) Help                : Display this message."
  echo "  11) Quit                : Exit the builder script."
}

while getopts "d:i:h" opt; do
    case "$opt" in
        d) DEV="$OPTARG";
            ;;
        i) SOMIP="$OPTARG";
            ;;
        h) usage; exit 0
            ;;
        ?) usage; exit 1
            ;;
    esac
done

while true;
do
  echo "MODE = ${MODE} | DEV = ${DEV} | SOMIP = ${SOMIP}"

  select opt in ${OPTIONS};
  do
    if [ "$opt" = "Change-Mode" ];
    then
      [ $MODE == "SOM" ] && MODE="PC" && break
      [ $MODE == "PC" ] && MODE="MCU" && break
      [ $MODE == "MCU" ] && MODE="SOM" && break
    elif [ "$opt" = "Compile" ];
    then
      [ $MODE == "PC" ] && {
        bash scripts/compile-som.sh "$ROOT_DIR"
        break
      }
      [ $MODE == "MCU" ] && {
        bash scripts/compile-mcu.sh "$ROOT_DIR"
        break
      }
      echo "Nothing to do on ${MODE}...."
    elif [ "$opt" = "Flash" ];
    then
      [ $MODE == "MCU" ] && {
        bash scripts/flash-mcu.sh "$ROOT_DIR" "$DEV"
        break
      }
      echo "Nothing to do on PC...."
    elif [ "$opt" = "Run" ];
    then
      [ $MODE == "PC" ] && {
        ./build/som/som-controller
        break
      }
      echo "Nothing to do on $MODE..."
    elif [ "$opt" = "Test" ];
    then
      [ $MODE == "PC" ] && {
        bash scripts/run-tests.sh "$ROOT_DIR"
        break
      }
      echo "Nothing to do on $MODE..."
      break
    elif [ "$opt" = "GPS-Fake" ];
    then
      [ $MODE == "PC" ] && {
        killall gpsfake ; sudo gpsfake -P 2000 -S ./logs/gps.nmea
        break
      }
      echo "Nothing to do on $MODE..."
      break
    elif [ "$opt" = "Read-Serial" ];
    then
      bash scripts/read-serial-mcu.sh "$ROOT_DIR" "$DEV"
      break
    elif [ "$opt" = "Install-Dependencies" ];
    then
      bash scripts/install-dependencies.sh "$ROOT_DIR" "True"
      break
    elif [ "$opt" = "Run-Clang-Format" ];
    then
      [ $MODE == "PC" ] && {
        find "$ROOT_DIR" -iname "*.hpp" -o -iname "*.cpp" -exec clang-format -i -style=file {} +
        break
      }
      echo "Nothing to do on $MODE..."
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
