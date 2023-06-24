#!/usr/bin/env bash

ROOT_DIR="$PWD"
DEV="/dev/ttyACM0"
SOMIP="192.168.0.30"
SIMULATION="OFF"
MODE="SOM"

OPTIONS="\
  Change-Mode \
  Compile \
  Flash \
  Run \
  Run-Test \
  GPS-Fake \
  Read-Serial \
  Install-Dependencies \
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
  echo "  1) Change-Mode          : Cycle between MCU, SOM and PC (default: SOM)."
  echo "  2) Compile              : Compile the mode's code."
  echo "  3) Flash                : Flash the modes code (via serial for MCU or ssh for SOM)."
  echo "  4) Run                  : Run the desired code on the appropiate device."
  echo "  5) Run-Test             : Run the tests for the appropiate device."
  echo "  6) GPS-Fake             : Start a simulated GPSd instance."
  echo "  7) Read-Serial          : Read the serial output of the microcontroller."
  echo "  8) Install-Dependencies : Install the required dependencies."
  echo "  9) Help                 : Display this message."
  echo "  10) Quit                : Exit the builder script."
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
      [ $MODE == "SOM" ] && {
        ssh pi@${SOMIP} 'cd rc_plane && git pull && ./scripts/compile-som.sh /home/pi/rc_plane'
        break
      }
      [ $MODE == "PC" ] && {
        bash scripts/compile-som.sh "$ROOT_DIR"
        break
      }
      [ $MODE == "MCU" ] && {
        bash scripts/compile-mcu.sh "$ROOT_DIR"
        break
      }
    elif [ "$opt" = "Flash" ];
    then
      [ $MODE == "SOM" ] && {
        echo "Copying to bin..."
        ssh pi@${SOMIP} 'cp rc_plane/build_som/som/som-controller bin'
        break
      }
      [ $MODE == "MCU" ] && {
        bash scripts/flash-mcu.sh "$ROOT_DIR" "$DEV"
        break
      }
      echo "Nothing to do on PC...."
    elif [ "$opt" = "Run" ];
    then
      [ $MODE == "SOM" ] && {
        ssh -t pi@${SOMIP} '~/bin/som-controller'
        break
      }
      [ $MODE == "PC" ] && {
        ./build_som/som/som-controller
        break
      }
      echo "Nothing to do on $MODE..."
    elif [ "$opt" = "Run-Test" ];
    then
      [ $MODE == "SOM" ] && {
        ssh pi@${SOMIP} 'rc_plane/build/common/test/rcplane_common_tests'
        break
      }
      [ $MODE == "PC" ] && {
        ./build/common/test/rcplane_common_tests
        break
      }
      echo "Nothing to do on $MODE..."
      break
    elif [ "$opt" = "GPS-Fake" ];
    then
      [ $MODE == "SOM" ] && {
        ssh pi@${SOMIP} 'killall gpsfake ; gpsfake -P 2000 -S rc_plane/logs/gps.nmea'
        break
      }
      [ $MODE == "PC" ] && {
        killall gpsfake ; sudo gpsfake -P 2000 -S ./logs/gps.nmea
        break
      }
      [ $MODE == "MCU" ] && {
        echo "Nothing to do on $MODE..."
        break
      }
    elif [ "$opt" = "Read-Serial" ];
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
