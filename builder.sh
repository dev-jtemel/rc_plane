#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"
MODE="PC"

OPTIONS="\
  Change-Mode \
  Compile \
  Flash \
  Run-SomController \
  Run-PcTelemetryRX \
  Debug \
  Compile-Test \
  Test \
  Read-Serial \
  Install-Dependencies \
  Run-Clang-Format \
  Documentation \
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
  echo "  1) Change-Mode            : Cycle between MCU, SOM and PC (default: PC)."
  echo "  2) Compile                : Compile the mode's code."
  echo "  3) Flash                  : Flash the modes code (via serial for MCU or ssh for SOM)."
  echo "  4) Run-SomController      : Run the desired code on the appropiate device."
  echo "  5) Run-PcTelemetryRX      : Run the desired code on the appropiate device."
  echo "  6) Debug                  : Run gdb on the PC code."
  echo "  7) Comple-Test            : Compile tests."
  echo "  8) Test                   : Run the tests for the appropiate device."
  echo "  9) Read-Serial            : Read the serial output of the microcontroller."
  echo "  10) Install-Dependencies   : Install the required dependencies."
  echo "  11) Run-Clang-Format      : Run clang-format on the project."
  echo "  12) Documentation         : Run doxygen on the project."
  echo "  13) Help                  : Display this message."
  echo "  14) Quit                  : Exit the builder script."
}

while getopts "d:i:h" opt; do
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
  echo "MODE = ${MODE} | DEV = ${DEV}"

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
    elif [ "$opt" = "Run-SomController" ];
    then
      [ $MODE == "PC" ] && {
        ./build/impl/src/SomController
        break
      }
      echo "Nothing to do on $MODE..."
    elif [ "$opt" = "Run-PcTelemetryRX" ];
    then
      [ $MODE == "PC" ] && {
        ./build/impl/src/PcTelemetryRX
        break
      }
      echo "Nothing to do on $MODE..."
    elif [ "$opt" = "Debug" ];
    then
      [ $MODE == "PC" ] && {
        gdb ./build/impl/src/SomController

        break
      }
      echo "Nothing to do on $MODE..."
    elif [ "$opt" = "Compile-Test" ];
    then
      [ $MODE == "PC" ] && {
        bash scripts/run-tests.sh "$ROOT_DIR" "True"
        break
      }
      echo "Nothing to do on $MODE..."
      break
    elif [ "$opt" = "Test" ];
    then
      [ $MODE == "PC" ] && {
        bash scripts/run-tests.sh "$ROOT_DIR"
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
        find "$ROOT_DIR/impl" -iname "*.hpp" -exec clang-format --verbose -style=file -i {} +
        find "$ROOT_DIR/impl" -iname "*.cpp" -exec clang-format --verbose -style=file -i {} +
        break
      }
      echo "Nothing to do on $MODE..."
      break
    elif [ "$opt" = "Documentation" ];
    then
      [ $MODE == "PC" ] && {
        doxygen Doxyfile
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
